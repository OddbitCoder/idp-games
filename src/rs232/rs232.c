#include <stdio.h>
#include <stdint.h>

const uint8_t SIO_PORT_CRT = 0xD9;
const uint8_t SIO_PORT_LPT = 0xDB;
const uint8_t SIO_PORT_VAX = 0xE1;
const uint8_t SIO_PORT_MOD = 0xE3;

__sfr __at 0xD9 SIO_PORT_CRT_C; // CRT control
__sfr __at 0xD8 SIO_PORT_CRT_D; // CRT data
__sfr __at 0xDB SIO_PORT_LPT_C; // LPT control
__sfr __at 0xDA SIO_PORT_LPT_D; // LPT data
__sfr __at 0xE1 SIO_PORT_VAX_C; // VAX control
__sfr __at 0xE0 SIO_PORT_VAX_D; // VAX data
__sfr __at 0xE3 SIO_PORT_MOD_C; // MOD control
__sfr __at 0xE2 SIO_PORT_MOD_D; // MOD data

uint8_t _sio_wr5;

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

const uint8_t sio_init_str[] = {
	10,   // init string size
	0x30, // write into WR0: error reset, select WR0
	0x18, // write into WR0: channel reset
	0x04, // write into WR0: select WR4
	0x44, // write into WR4: clk*16, 1 stop bit, no parity (9600 bauds)
	0x05, // write into WR0: select WR5
	0xE8, // write into WR5: DTR active, TX 8 bit, BREAK off, TX on, RTS inactive
	0x01, // write into WR0: select WR1
	//0x1C, // write into WR1: status affects vector, int on all RX chars (D2 needs to be 1)
	0x04,
	0x03, // write into WR0: select WR3
	0xC1  // write into WR3: RX 8 bit, auto enable off, RX on
};

void sio_init(uint8_t port, uint8_t *init_str) {
	__asm__ ("di");
	for (uint8_t i = 1; i < init_str[0] + 1; i++) {
		_out(port, init_str[i]);
	}
	_sio_wr5 = init_str[6];
	__asm__ ("ei");
}

void sio_set_rts(uint8_t port, bool flag) {
	if (flag && (_sio_wr5 & 2) == 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 | 2));
	} else if (!flag && (_sio_wr5 & 2) != 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 - 2));
	}
}

void sio_set_dtr(uint8_t port, bool flag) {
	if (flag && (_sio_wr5 & 128) == 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 | 128));
	} else if (!flag && (_sio_wr5 & 128) != 0) {
		_out(port, 5);
		_out(port, _sio_wr5 = (_sio_wr5 - 128));
	}
}

bool sio_check_cts(uint8_t port) {
	return _in(port) & 64; // D6 of RR0
}

// WARNME: this actually checks DCD; DCD should be "shorted" to DSR for DSR/DTR handshaking to work
bool sio_check_dsr(uint8_t port) {
	return _in(port) & 8; // D3 of RR0
}

bool sio_check_all_sent(uint8_t port) {
	_out(port, 1);
	return _in(port) & 1; // D0 of RR1
}

bool sio_check_ch_available(uint8_t port) {
	return _in(port) & 1; // D0 of RR0	
}

void sio_tx_byte_rtscts(uint8_t port, uint8_t byte) {
	// request to send
	sio_set_rts(port, true);
	// wait for "clear to send"
	while (!sio_check_cts(port));
	// send byte
	_out(port - 1, byte);
	// wait for "all sent"
	while (!sio_check_all_sent(port));
	// clear RTS
	sio_set_rts(port, false);
}

void sio_tx_str_rtscts(uint8_t port, uint8_t *str) {
	// request to send
	sio_set_rts(port, true);
	for (uint8_t *p = str; *p != 0; p++) {
		// wait for "clear to send"
		while (!sio_check_cts(port));
		// send byte (char)
		_out(port - 1, *p);
		// wait for "all sent"
		while (!sio_check_all_sent(port));
	}
	// clear RTS
	sio_set_rts(port, false);
}

#define SIO_OUT_SZ 1024
#define SIO_IN_SZ 3
#define SIO_IN_SZ_EXT (3 + 64)

const uint16_t _no_activity_thr = 100;

uint8_t _sio_out[SIO_OUT_SZ];
uint8_t _sio_in[SIO_IN_SZ_EXT];
uint8_t _sio_in_ptr = 0;

typedef enum {
	SIO_RCV_NO_ACTIVITY,
	SIO_RCV_BUFFER_FULL,
	SIO_RCV_BUFFER_OVERFLOW
} sio_rcv_exit_code;

sio_rcv_exit_code _sio_flush(uint8_t port) { 
	uint16_t loop_count = 0;
	while (true) {
		if (sio_check_ch_available(port)) {
			loop_count = 0;
			// read char
			uint8_t ch = _in(port - 1);
			// put char into buffer
			_sio_in[_sio_in_ptr++] = ch;
			// check if buffer overflow
			if (_sio_in_ptr >= SIO_IN_SZ_EXT) {
				return SIO_RCV_BUFFER_OVERFLOW;
			}
		} else {
			if ((++loop_count) >= _no_activity_thr) {
				return SIO_RCV_NO_ACTIVITY;
			}
		}
	}
}

sio_rcv_exit_code sio_rcv(uint8_t port) {
	uint16_t loop_count = 0;
	sio_set_rts(port, true);
	//sio_set_dtr(port, true);
	while (true) {
		// try send char 
		// TODO
		// check if ch available
		if (sio_check_ch_available(port)) {
			loop_count = 0;
			// read char
			uint8_t ch = _in(port - 1);
			// put char into buffer
			_sio_in[_sio_in_ptr++] = ch;
			// check if buffer full
			if (_sio_in_ptr >= SIO_IN_SZ) {
				//printf("%d ", _sio_wr5);
				sio_set_rts(port, false);
				//printf("%d ", _sio_wr5);
				//sio_set_dtr(port, false);
				return _sio_flush(port) == SIO_RCV_BUFFER_OVERFLOW
					? SIO_RCV_BUFFER_OVERFLOW
					: SIO_RCV_BUFFER_FULL;
			}
		} else {
			if ((++loop_count) >= _no_activity_thr) {
				return SIO_RCV_NO_ACTIVITY;
			}
		}
	}
}

void lptint() {
	printf("1 ");
}

void lpterr() {
	printf("2 ");
}

void sio_set_int() {
	uint16_t *bios_addr_ptr = 1;
	uint16_t bios_addr = *bios_addr_ptr - 3;
	*(uint16_t *)(bios_addr + 0x94) = (uint16_t)&lptint;
	*(uint16_t *)(bios_addr + 0x96) = (uint16_t)&lpterr;
}

int main(int argc, char **argv) {
	// init LPT
	sio_init(SIO_PORT_LPT, sio_init_str);
	//sio_tx_byte_rtscts(SIO_PORT_LPT, 'A');
	//sio_tx_str_rtscts(SIO_PORT_LPT, "HELLO FROM THE OTHER SIDE");
	sio_set_rts(SIO_PORT_LPT, false);
	//sio_set_int();
	// while (true) {
	// 	sio_rcv_exit_code exit_code = sio_rcv(SIO_PORT_LPT);
	// 	_sio_in[_sio_in_ptr] = 0;
	// 	if (_sio_in_ptr > 0) { 
	// 		printf("(%u)%s\n\r", exit_code, _sio_in); 
	// 	}
	// 	_sio_in_ptr = 0;
	// }
	bool flag = true;
	while(true) {
		for (int i = 0; i < 300; i++) {
			printf("DELAY\n\r");
		}
		sio_set_dtr(SIO_PORT_LPT, flag);
		flag = !flag;
	}
	return 0;
}