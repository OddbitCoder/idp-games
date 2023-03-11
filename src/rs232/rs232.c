#include <stdio.h>
#include <stdint.h>

const uint8_t SIO_PORT_CRT = 0xD9;
const uint8_t SIO_PORT_LPT = 0xDB;
const uint8_t SIO_PORT_VAX = 0xE1;
const uint8_t SIO_PORT_MOD = 0xE3;

// uint8_t sio_get_status(uint8_t port) __naked {
// __asm
// 	pop de // de=return address
// 	pop bc // c=port
// 	// restore stack
//   	push bc
//   	push de
// 	in a,(c)
// 	ld l,a
// 	ret;
// __endasm;
// }

// void sio_send_byte(uint8_t port, uint8_t val) __naked {
// __asm
// 	pop de // de=return address
// 	pop bc // c=port, b=value
//   	// restore stack
//   	push bc
//   	push de
// //sio_wait:
// //   	in a,(c)
// //	and #4
// //	jr z,sio_wait
// 	dec c
// 	out (c),b
//   	ret
// __endasm;
// }

// uint8_t _read_mem(uint16_t addr) __naked {
// __asm
// 	pop de // de=return address
// 	pop bc // bc=mem address
//   	// restore stack
//   	push bc
//   	push de
//   	ld a,(bc)
//   	ld l,a
//   	ret
// __endasm;
// }

uint16_t _bios_addr() __naked {
__asm
	pop de // de=return address
  	// restore stack
  	push de
  	ld hl,(#1)
  	dec hl
  	dec hl
  	dec hl
  	ret
__endasm;
}

void call(uint16_t addr) {
__asm
	pop de // de=return address
	pop bc // bc=jump addr
  	// restore stack
  	push bc
  	push de
  	ld h, b
  	ld l, c
  	jp (hl)
__endasm;
}

void test() {
	printf("test\n\r");
}

void int1() {
	printf("int1 ");
}

void int2() {
	printf("int2 ");
}

void int3() {
	printf("int3 ");
}

void int4() {
	printf("int4 ");
}

// void sio_send_string(uint8_t port, uint8_t *str) {
// 	uint8_t *ptr = str;
// 	while (*ptr != 0) {
// 		sio_send_byte(port, *(ptr++));
// 	}
// }

__sfr __at 0xDB SIO_PORT_LPT_C; // LPT control
__sfr __at 0xDA SIO_PORT_LPT_D; // LPT data

const uint8_t sio_init_str[] = {
	14,
	// TX & RX
	0x30, // write into WR0: error reset, select WR0
	0x18, // write into WR0: channel reset
	0x04, // write into WR0: select WR4
	0x44, // 44h write into WR4: clkx16,1 stop bit, no parity
	0x05, // write into WR0: select WR5
	0xE8, // DTR active, TX 8bit, BREAK off, TX on, RTS inactive
	0x01, // write into WR0: select WR1
	0x04, // no interrupt in CH B, special RX condition affects vect
	0x02, // write into WR0: select WR2
	0x00, // write into WR2: cmd line int vect (see int vec table); bits D3,D2,D1 are changed according to RX condition
	0x01, // write into WR0: select WR1
	0x18, // interrupt on all RX characters, parity is not a spec RX condition; buffer overrun is a spec RX condition
	// EI
	0x03, // write into WR0: select WR3
	0xC1  // RX 8bit, auto enable off, RX on
};

const uint8_t sio_init_str_grmt[] = {
// *** OUT DB (SIO1 kanal LPT status) = 18
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = 5
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = 68
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = 1
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = 1C
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = 3
// >>> EDC9	OTIR
// *** OUT DB (SIO1 kanal LPT status) = C1
	8, 0x18, 0x05, 0x68, 0x01, 0x01, 0x1C, 0x03, 0xC1
};

void sio_lpt_init(uint8_t *init_str) {
	__asm__ ("di");
	for (uint8_t i = 1; i < init_str[0] + 1; i++) {
		SIO_PORT_LPT_C = init_str[i];
	}
	__asm__ ("ei");
}

int main(int argc, char **argv) {
	printf("%u\n\r", _bios_addr());
	uint16_t *bios_addr = 1;
	printf("%u\n\r", *bios_addr - 3); // is this the same as above?
	// do we know how to call "test"?
	call((uint16_t)&test);
	// SIO interrupt vec table 
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x90));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x90 + 2));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x90 + 4));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x90 + 6));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x98));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x98 + 2));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x98 + 4));
	printf("%u\r\n", *(uint16_t *)(_bios_addr() + 0x98 + 6));
	// try init LPT
	sio_lpt_init(sio_init_str);
	SIO_PORT_LPT_D = 'A';	// try to send a character
	return 0;
}