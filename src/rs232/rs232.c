#include <stdio.h>
#include <rs232.h>

#define SIO_OUT_SZ 1024
#define SIO_IN_SZ 3
#define SIO_IN_SZ_EXT (SIO_IN_SZ + 64)

const uint16_t _sio_no_activity_thr = 100;

uint8_t _sio_in[SIO_IN_SZ_EXT + 1]; // "+ 1" is useful if we simply want to put 0 at the end of the buffer for output to console
uint16_t _sio_in_ptr = 0;

uint8_t _sio_out[SIO_OUT_SZ];
uint16_t _sio_out_put_ptr = 0;
uint16_t _sio_out_get_ptr = 0;
uint16_t _sio_out_count = 0;

uint8_t _sio_wr5;

const uint8_t _sio_init_str[] = {
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
	// NOTE: I think it's ok if auto enable if always off, even in the RTS/CTS mode (because we explicitly control RTS/CTS)
};

void _out(uint8_t port, uint8_t val) __naked { // TODO: move to utils
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

uint8_t _in(uint8_t port) __naked { // TODO: move to utils
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
void sio_init(uint8_t port, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity) { 
	for (uint8_t i = 1; i < _sio_init_str[0] + 1; i++) {
		uint8_t val = _sio_init_str[i];
		if (i == 4) { // WR4: clock, stop bits, parity
			val = clock | stop_bits | parity;
		} else if (i == 8) { // WR1: enable or disable interrupts
			val = mode == SIO_MODE_POLLING ? 0x04 : 0x1C;
		} else if (i == 10) { // WR3: data bits & RX enable
			val = data_bits | 1;
		}
		_out(port, val);
	}
	_sio_wr5 = _sio_init_str[6];
}

void _sio_set_rts(uint8_t port, bool flag) {
	if (flag && (_sio_wr5 & 2) == 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 | 2));
	} else if (!flag && (_sio_wr5 & 2) != 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 - 2));
	}
}

bool _sio_check_cts(uint8_t port) {
	return _in(port) & 64; // D6 of RR0
}

bool _sio_check_all_sent(uint8_t port) {
	_out(port, 1);
	return _in(port) & 1; // D0 of RR1
}

bool _sio_check_tx_buffer_empty(uint8_t port) {
	return _in(port) & 4;
}

bool _sio_check_ch_available(uint8_t port) {
	return _in(port) & 1; // D0 of RR0	
}

sio_exit_code _sio_flush(uint8_t port) { 
	uint16_t loop_count = 0;
	while (true) {
		if (_sio_check_ch_available(port)) {
			loop_count = 0;
			// read char
			uint8_t ch = _in(port - 1);
			// put char into buffer
			_sio_in[_sio_in_ptr++] = ch;
			// check if buffer overflow
			if (_sio_in_ptr >= SIO_IN_SZ_EXT) {
				return SIO_EXIT_CODE_BUFFER_OVERFLOW;
			}
		} else {
			if (++loop_count >= _sio_no_activity_thr) {
				return SIO_EXIT_CODE_NO_ACTIVITY;
			}
		}
	}
}

bool sio_out_buffer_put(uint8_t ch) {
	if (_sio_out_count == SIO_OUT_SZ) { 
		return false; 
	}
	_sio_out[_sio_out_put_ptr++] = ch;
	_sio_out_count++;
	if (_sio_out_put_ptr == SIO_OUT_SZ) {
		_sio_out_put_ptr = 0;
	}
	return true;
}

uint8_t _sio_out_buffer_get() {
	if (_sio_out_count == 0) {
		return 0;
	}
	uint8_t ch = _sio_out[_sio_out_get_ptr++];
	_sio_out_count--;
	if (_sio_out_get_ptr == SIO_OUT_SZ) {
		_sio_out_get_ptr = 0;
	}
	return ch;
}

bool _sio_out_buffer_peek() {
	return _sio_out_count > 0;
}

bool sio_out_buffer_put_str(uint8_t *str) {
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		if (!sio_out_buffer_put(*ptr)) {
			return false;
		}
	}
	return true;
}

uint16_t sio_out_buffer_count() {
	return _sio_out_count;
}

sio_exit_code sio_poll(uint8_t port) {
	if (_sio_in_ptr >= SIO_IN_SZ) {
		return _sio_in_ptr >= SIO_IN_SZ_EXT
			? SIO_EXIT_CODE_BUFFER_OVERFLOW
			: SIO_EXIT_CODE_BUFFER_FULL;
	}
	uint8_t ch;
	uint16_t loop_count = 0;
	_sio_set_rts(port, true);
	while (true) {
		// try send char 
		if (_sio_check_cts(port) && _sio_out_buffer_peek()) {
			loop_count = 0; // reset activity counter
			if (/*_sio_check_all_sent(port)*/_sio_check_tx_buffer_empty(port)) {
				_out(port - 1, _sio_out_buffer_get());
			}
		}
		// check if ch available
		if (_sio_check_ch_available(port)) {
			loop_count = 0; // reset activity counter
			// read char
			ch = _in(port - 1);
			// put char into buffer
			_sio_in[_sio_in_ptr++] = ch;
			// check if buffer full
			if (_sio_in_ptr >= SIO_IN_SZ) {
				_sio_set_rts(port, false);
				return _sio_flush(port) == SIO_EXIT_CODE_BUFFER_OVERFLOW
					? SIO_EXIT_CODE_BUFFER_OVERFLOW
					: SIO_EXIT_CODE_BUFFER_FULL;
			}
		} else {
			if (++loop_count >= _sio_no_activity_thr) {
				return SIO_EXIT_CODE_NO_ACTIVITY;
			}
		}
	}
}

void sio_send_ch(uint8_t port, uint8_t ch) { // WARNME: blocking
	while (!_sio_check_cts(port) || !_sio_check_tx_buffer_empty(port));
	_out(port - 1, ch);
}

void sio_send(uint8_t port, uint16_t len, uint8_t *buffer) { // WARNME: blocking
	for (uint8_t i = 0; i < len; i++) {
		sio_send_ch(port, buffer[i]);
	}
}

void sio_send_str(uint8_t port, uint8_t *str) { // WARNME: blocking
	for (uint8_t *ptr = str; *ptr != 0; ptr++) {
		sio_send_ch(port, *ptr);
	}
}

void sio_done(uint8_t port) {
	// nothing to do here for now
}

int main(int argc, char **argv) {
	// init LPT
	sio_init(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_CLOCK_X16, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE);
	sio_send_str(SIO_PORT_LPT, "This is a test ");
	sio_out_buffer_put_str("HELLO DARKNESS MY OLD FRIEND");
	while (true) {
		sio_exit_code exit_code = sio_poll(SIO_PORT_LPT);
		_sio_in[_sio_in_ptr] = 0;
		if (_sio_in_ptr > 0) { 
			printf("(%u)%s\n\r", exit_code, _sio_in); 
		}
		_sio_in_ptr = 0;
	}
}