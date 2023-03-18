#include "sio.h"
#include <stdlib.h>

#define SIO_CHAR_XON  17
#define SIO_CHAR_XOFF 19

const uint8_t _sio_init_str[] = {
	10,   // init string size
	0x30, // write into WR0: error reset, select WR0
	0x18, // write into WR0: channel reset
	0x04, // write into WR0: select WR4
	0x44, // write into WR4: clk x16, 1 stop bit, no parity (x16 = 9600 bauds)
	0x05, // write into WR0: select WR5
	0x68, // write into WR5: DTR inactive, TX 8 bit, BREAK off, TX on, RTS inactive
	0x01, // write into WR0: select WR1
	0x04, // write into WR1: status affects vector, no interrupts 
	0x03, // write into WR0: select WR3
	0xC1  // write into WR3: RX 8 bit, auto enable off, RX on 
	// NOTE: I think it's ok if auto enable is always off, even in the RTS/CTS mode (because we explicitly control RTS/CTS)
};

void _sio_buffer_alloc(sio_buffer *buffer, uint16_t size) {
	buffer->values = malloc(size);
	buffer->size = size;
}

bool sio_buffer_put_ch(sio_buffer *buffer, uint8_t ch) {
	if (buffer->count == buffer->size) { 
		return false; 
	}
	buffer->values[buffer->put_ptr++] = ch;
	buffer->count++;
	if (buffer->put_ptr == buffer->size) {
		buffer->put_ptr = 0;
	}
	return true;
}

bool sio_buffer_put(sio_buffer *buffer, uint16_t len, uint8_t *values) {
	for (uint16_t i = 0; i < len; i++) {
		if (!sio_buffer_put_ch(buffer, values[i])) {
			return false;
		}
	}
	return true;
}

bool sio_buffer_put_str(sio_buffer *buffer, uint8_t *str) {
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		if (!sio_buffer_put_ch(buffer, *ptr)) {
			return false;
		}
	}
	return true;
}

uint8_t sio_buffer_get_ch(sio_buffer *buffer) {
	if (buffer->count == 0) {
		return 0;
	}
	uint8_t ch = buffer->values[buffer->get_ptr++];
	buffer->count--;
	if (buffer->get_ptr == buffer->size) {
		buffer->get_ptr = 0;
	}
	return ch;
}

bool sio_buffer_peek(sio_buffer *buffer) {
	return buffer->count > 0;
}

uint16_t sio_buffer_get(sio_buffer *buffer, uint8_t *dest) {
	uint8_t *p = dest;
	uint16_t count = buffer->count;
	while (sio_buffer_peek(buffer)) {
		*(p++) = sio_buffer_get_ch(buffer);
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

sio_port *sio_init_ex(sio_port_addr port_addr, sio_mode mode, sio_bauds bauds, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	sio_flow_control flow_control, uint16_t out_buffer_sz, uint16_t in_buffer_sz, uint16_t in_buffer_ext, uint16_t no_activity_thr) { 
	for (uint8_t i = 1; i < _sio_init_str[0] + 1; i++) {
		uint8_t val = _sio_init_str[i];
		if (i == 4) { // WR4: clock, stop bits, parity
			val = bauds | stop_bits | parity;
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
	port->wr5 = _sio_init_str[6];
	port->flow_control = flow_control;
	port->xon_send = true; // WARNME: not sure what the default is here (I assume it's "on")
	port->xon_rcv = true;  // WARNME: same as above
	return port;
}

sio_port *sio_init(sio_port_addr port_addr, sio_mode mode, sio_bauds bauds, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	sio_flow_control flow_control) { 
	return sio_init_ex(port_addr, mode, bauds, data_bits, stop_bits, parity, flow_control, 
		/*out_buffer_sz*/512,
		/*in_buffer_sz*/128,
		/*in_buffer_ext*/64,
		/*no_activity_thr*/100);
}

bool sio_check_cts(sio_port *port) {
	return _in(port->addr) & 64; // D6 of RR0
}

bool _sio_check_tx_buffer_empty(sio_port *port) {
	return _in(port->addr) & 4; // D2 of RR0
}

bool _sio_check_ch_available(sio_port *port) {
	return _in(port->addr) & 1; // D0 of RR0	
}

void _sio_send_ch_force(sio_port *port, uint8_t ch) { 
	while (!_sio_check_tx_buffer_empty(port));
	_out(port->addr - 1, ch);
}

void _sio_set_able_to_rcv(sio_port *port, bool state) {
	if (port->flow_control == SIO_FLOW_CONTROL_RTSCTS) {
		sio_set_rts(port, state);		
	} else if (port->flow_control == SIO_FLOW_CONTROL_XONXOFF) {
		// send XON or XOFF
		if (state && !port->xon_rcv) {
			_sio_send_ch_force(port, SIO_CHAR_XON);
			port->xon_rcv = true;
		} else if (!state && port->xon_rcv) {
			_sio_send_ch_force(port, SIO_CHAR_XOFF);
			port->xon_rcv = false;
		}
	}
	// else do nothing
}

void sio_set_rts(sio_port *port, bool state) {
	if (state && (port->wr5 & 2) == 0) {
		_out(port->addr, 5);
		_out(port->addr, port->wr5 = (port->wr5 | 2));
	} else if (!state && (port->wr5 & 2) != 0) {
		_out(port->addr, 5);
		_out(port->addr, port->wr5 = (port->wr5 - 2));
	}
}

bool _sio_check_able_to_send(sio_port *port) {
	if (port->flow_control == SIO_FLOW_CONTROL_RTSCTS) {
		return sio_check_cts(port);		
	} else if (port->flow_control == SIO_FLOW_CONTROL_XONXOFF) {
		return port->xon_send;
	} 
	return true; // no flow control
}

sio_exit_code _sio_flush(sio_port *port) { 
	uint16_t loop_count = 0;
	while (true) {
		if (_sio_check_ch_available(port)) {
			loop_count = 0;
			// read char
			uint8_t ch = _in(port->addr - 1);
			if (port->flow_control == SIO_FLOW_CONTROL_XONXOFF && (ch == SIO_CHAR_XON || ch == SIO_CHAR_XOFF)) {
				port->xon_send = ch == SIO_CHAR_XON;
			} else {
				// put char into buffer
				sio_buffer_put_ch(&port->buffer_in, ch);
				// check if buffer overflow
				if (port->buffer_in.count >= port->buffer_in.size) {
					return SIO_EXIT_CODE_BUFFER_OVERFLOW;
				}
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
	_sio_set_able_to_rcv(port, true);
	while (true) {
		// try send char 
		if (_sio_check_able_to_send(port) && sio_buffer_peek(&port->buffer_out)) {
			loop_count = 0; // reset activity counter
			if (_sio_check_tx_buffer_empty(port)) {
				_out(port->addr - 1, sio_buffer_get_ch(&port->buffer_out));
			}
		}
		// check if ch available
		if (_sio_check_ch_available(port)) {
			loop_count = 0; // reset activity counter
			// read char
			ch = _in(port->addr - 1);
			if (port->flow_control == SIO_FLOW_CONTROL_XONXOFF && (ch == SIO_CHAR_XON || ch == SIO_CHAR_XOFF)) {
				port->xon_send = ch == SIO_CHAR_XON;
			} else {
				// put char into buffer
				sio_buffer_put_ch(&port->buffer_in, ch);
				// check if buffer full
				if (port->buffer_in.count >= port->buffer_in.size - port->in_buffer_ext) {
					_sio_set_able_to_rcv(port, false);
					return _sio_flush(port) == SIO_EXIT_CODE_BUFFER_OVERFLOW
						? SIO_EXIT_CODE_BUFFER_OVERFLOW
						: SIO_EXIT_CODE_BUFFER_FULL;
				}
			}
		} else {
			if (++loop_count >= port->no_activity_thr) {
				return SIO_EXIT_CODE_NO_ACTIVITY;
			}
		}
	}
}

void sio_send_ch(sio_port *port, uint8_t ch) { 
	while (!_sio_check_able_to_send(port) || !_sio_check_tx_buffer_empty(port));
	_out(port->addr - 1, ch);
}

void sio_send(sio_port *port, uint16_t len, uint8_t *values) {
	for (uint8_t i = 0; i < len; i++) {
		sio_send_ch(port, values[i]);
	}
}

void sio_send_str(sio_port *port, uint8_t *str) { 
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		sio_send_ch(port, *ptr);
	}
}

void sio_done(sio_port *port) {
	free(port->buffer_in.values);
	free(port->buffer_out.values);
	free(port);
}