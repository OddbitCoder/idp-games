#include <stdint.h>

typedef enum {
	SIO_CRT = 0xD9,
	SIO_LPT = 0xDB,
	SIO_VAX = 0xE1,
	SIO_MOD = 0xE3
};

void sio_send_byte(uint8_t port, uint8_t value) __naked {
__asm
	pop de // de=return address
	pop bc // c=port, b=value
  	// restore stack
  	push bc
  	push de
  	push af
sio_wait:
   	in a,(c)
	and #4
	jr z,sio_wait
	dec c
	out (c),b
	pop af
  	ret
__endasm;
}

void sio_send_string(uint8_t port, uint8_t *str) {
	uint8_t *ptr = str;
	while (ptr++ != 0) {
		sio_send_byte(port, *ptr);
	}
}

int main(int argc, char **argv) {
	sio_send_string(SIO_LPT, "HELLO FROM THE PAST!");
	return 0;
}