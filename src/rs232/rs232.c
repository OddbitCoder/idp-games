#include <stdio.h>
#include <stdlib.h>
#include <rs232.h>

const uint8_t _sio_sio_init_str[] = {
	10,   // init string size
	0x30, // write into WR0: error reset, select WR0
	0x18, // write into WR0: channel reset
	0x04, // write into WR0: select WR4
	0x44, // write into WR4: clk*16, 1 stop bit, no parity (x16 = 9600 bauds)
	0x05, // write into WR0: select WR5
	0xE8, // write into WR5: DTR active, TX 8 bit, BREAK off, TX on, RTS inactive // TODO: change DTR to inactive
	0x01, // write into WR0: select WR1
	0x04, // write into WR1: status affects vector, no interrupts 
	0x03, // write into WR0: select WR3
	0xC1  // write into WR3: RX 8 bit, auto enable off, RX on 
	// NOTE: I think it's ok if auto enable is always off, even in the RTS/CTS mode (because we explicitly control RTS/CTS)
};

void _sio_buffer_alloc(sio_buffer *buffer, uint16_t size) {
	buffer->vals = malloc(size);
	buffer->size = size;
}

bool _sio_buffer_put(sio_buffer *buffer, uint8_t ch) {
	if (buffer->count == buffer->size) { 
		return false; 
	}
	buffer->vals[buffer->put_ptr++] = ch;
	buffer->count++;
	if (buffer->put_ptr == buffer->size) {
		buffer->put_ptr = 0;
	}
	return true;
}

bool _sio_buffer_put_str(sio_buffer *buffer, uint8_t *str) { // TODO: remove this 
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		if (!_sio_buffer_put(buffer, *ptr)) {
			return false;
		}
	}
	return true;
}

uint8_t _sio_buffer_get(sio_buffer *buffer) {
	if (buffer->count == 0) {
		return 0;
	}
	uint8_t ch = buffer->vals[buffer->get_ptr++];
	buffer->count--;
	if (buffer->get_ptr == buffer->size) {
		buffer->get_ptr = 0;
	}
	return ch;
}

uint16_t _sio_buffer_read(sio_buffer *buffer, uint8_t *dest) {
	uint8_t *p = dest;
	uint16_t count = buffer->count;
	while (buffer->count > 0) {
		*(p++) = _sio_buffer_get(buffer);
	}
	return count;
}

void _out(uint8_t port, uint8_t val) __naked { 
__asm
	pop de // de=return address
	pop bc // c=port, b=val
	// restore stack
  	push bc
  	push de
	out (c),b
	ret
__endasm;
}

uint8_t _in(uint8_t port) __naked { 
__asm
	pop de // de=return address
	pop bc // c=port
	// restore stack
  	push bc
  	push de
	in l,(c);
	ret
__endasm;
}

// TODO: bauds
sio_port *sio_sio_init_ex(sio_port_addr port_addr, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	uint16_t out_buffer_sz, uint16_t in_buffer_sz, uint16_t in_buffer_ext, uint16_t no_activity_thr) { 
	for (uint8_t i = 1; i < _sio_sio_init_str[0] + 1; i++) {
		uint8_t val = _sio_sio_init_str[i];
		if (i == 4) { // WR4: clock, stop bits, parity
			val = clock | stop_bits | parity;
		} else if (i == 8) { // WR1: enable or disable interrupts
			val = mode == SIO_MODE_POLLING ? 0x04 : 0x1C;
		} else if (i == 10) { // WR3: data bits & RX enable
			val = data_bits | 1;
		}
		_out(port_addr, val);
	}
	// create port object
	sio_port *port = calloc(1, sizeof(sio_port));
	_sio_buffer_alloc(&port->buffer_in, in_buffer_sz + in_buffer_ext); 
	_sio_buffer_alloc(&port->buffer_out, out_buffer_sz);
	port->in_buffer_ext = in_buffer_ext;
	port->no_activity_thr = no_activity_thr;
	port->addr = port_addr;
	port->wr5 = _sio_sio_init_str[6];
	return port;
}

sio_port *sio_sio_init(sio_port_addr port_addr, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity) { 
	return sio_sio_init_ex(port_addr, mode, clock, data_bits, stop_bits, parity, 
		/*out_buffer_sz*/1024,
		/*in_buffer_sz*/3,
		/*in_buffer_ext*/64,
		/*no_activity_thr*/100);
}

void _sio_set_rts(sio_port *port, bool flag) {
	if (flag && (port->wr5 & 2) == 0) {
		_out(port->addr, 5);
		_out(port->addr, port->wr5 = (port->wr5 | 2));
	} else if (!flag && (port->wr5 & 2) != 0) {
		_out(port->addr, 5);
		_out(port->addr, port->wr5 = (port->wr5 - 2));
	}
}

bool _sio_check_cts(sio_port *port) {
	return _in(port->addr) & 64; // D6 of RR0
}

bool _sio_check_tx_buffer_empty(sio_port *port) {
	return _in(port->addr) & 4;
}

bool _sio_check_ch_available(sio_port *port) {
	return _in(port->addr) & 1; // D0 of RR0	
}

sio_exit_code _sio_flush(sio_port *port) { 
	uint16_t loop_count = 0;
	while (true) {
		if (_sio_check_ch_available(port)) {
			loop_count = 0;
			// read char
			uint8_t ch = _in(port->addr - 1);
			// put char into buffer
			_sio_buffer_put(&port->buffer_in, ch);
			// check if buffer overflow
			if (port->buffer_in.count >= port->buffer_in.size) {
				return SIO_EXIT_CODE_BUFFER_OVERFLOW;
			}
		} else {
			if (++loop_count >= port->no_activity_thr) {
				return SIO_EXIT_CODE_NO_ACTIVITY;
			}
		}
	}
}

sio_exit_code sio_poll(sio_port *port) {
	if (port->buffer_in.count >= port->buffer_in.size - port->in_buffer_ext) { 
		return port->buffer_in.count >= port->buffer_in.size
			? SIO_EXIT_CODE_BUFFER_OVERFLOW
			: SIO_EXIT_CODE_BUFFER_FULL;
	}
	uint8_t ch;
	uint16_t loop_count = 0;
	_sio_set_rts(port, true);
	while (true) {
		// try send char 
		if (_sio_check_cts(port) && port->buffer_out.count > 0) {
			loop_count = 0; // reset activity counter
			if (_sio_check_tx_buffer_empty(port)) {
				_out(port->addr - 1, _sio_buffer_get(&port->buffer_out));
			}
		}
		// check if ch available
		if (_sio_check_ch_available(port)) {
			loop_count = 0; // reset activity counter
			// read char
			ch = _in(port->addr - 1);
			// put char into buffer
			_sio_buffer_put(&port->buffer_in, ch);
			//printf("%u %u ", port->buffer_in.count, port->buffer_in.size);
			// check if buffer full
			if (port->buffer_in.count >= port->buffer_in.size - port->in_buffer_ext) {
				_sio_set_rts(port, false);
				return _sio_flush(port) == SIO_EXIT_CODE_BUFFER_OVERFLOW
					? SIO_EXIT_CODE_BUFFER_OVERFLOW
					: SIO_EXIT_CODE_BUFFER_FULL;
			}
		} else {
			if (++loop_count >= port->no_activity_thr) {
				return SIO_EXIT_CODE_NO_ACTIVITY;
			}
		}
	}
}

void sio_send_ch(sio_port *port, uint8_t ch) { 
	while (!_sio_check_cts(port) || !_sio_check_tx_buffer_empty(port));
	_out(port->addr - 1, ch);
}

void sio_send(sio_port *port, uint16_t len, uint8_t *buffer) {
	for (uint8_t i = 0; i < len; i++) {
		sio_send_ch(port, buffer[i]);
	}
}

void sio_send_str(sio_port *port, uint8_t *str) { 
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		sio_send_ch(port, *ptr);
	}
}

void sio_done(sio_port *port) {
	free(port->buffer_in.vals);
	free(port->buffer_out.vals);
	free(port);
}

// -----------------------------------------------------------

uint8_t tmp[1024];

int main(int argc, char **argv) {
	// init LPT
	sio_port *port = sio_sio_init(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_CLOCK_X16, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE);
	sio_send_str(port, "This is a test ");
	_sio_buffer_put_str(&port->buffer_out, "HELLO DARKNESS MY OLD FRIEND");
	while (true) {
		sio_exit_code exit_code = sio_poll(port);
		uint16_t count = _sio_buffer_read(&port->buffer_in, tmp);
		if (count > 0) { 
			tmp[count] = 0;
			printf("(%u)%s\n\r", exit_code, tmp); 
		}
	}
}