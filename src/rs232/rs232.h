#ifndef __RS232_H__
#define __RS232_H__

#include <stdint.h>

typedef enum { 
	SIO_CLOCK_X1  = 0,
	SIO_CLOCK_X16 = 64,
	SIO_CLOCK_X32 = 128,
	SIO_CLOCK_X64 = 128 | 64
} sio_clock_mode;

typedef enum { // TODO
	SIO_BAUDS_9600 = 64
} sio_bauds;

typedef enum {
	SIO_STOP_BITS_1 = 4,
	SIO_STOP_BITS_2 = 8 | 4
} sio_stop_bits;

typedef enum {
	SIO_PARITY_ODD  = 1,
	SIO_PARITY_EVEN = 2 | 1,
	SIO_PARITY_NONE = 0
} sio_parity;

typedef enum {
	SIO_DATA_BITS_5 = 0,
	SIO_DATA_BITS_6 = 128,
	SIO_DATA_BITS_7 = 64,
	SIO_DATA_BITS_8 = 128 | 64
} sio_data_bits;

typedef enum {
	SIO_MODE_POLLING,
	//SIO_MODE_INTERRUPTS
} sio_mode;

typedef enum {
	SIO_EXIT_CODE_NO_ACTIVITY,
	SIO_EXIT_CODE_BUFFER_FULL,
	SIO_EXIT_CODE_BUFFER_OVERFLOW
} sio_exit_code;

const uint8_t SIO_PORT_CRT = 0xD9;
const uint8_t SIO_PORT_LPT = 0xDB;
const uint8_t SIO_PORT_VAX = 0xE1;
const uint8_t SIO_PORT_MOD = 0xE3;

// initializer
void sio_init(uint8_t port, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity);

bool sio_out_buffer_put(uint8_t ch);
bool sio_out_buffer_put_str(uint8_t *str);
uint16_t sio_out_buffer_count();

// non-blocking send & receive
sio_exit_code sio_poll(uint8_t port);

// blocking send
void sio_send_ch(uint8_t port, uint8_t ch);
void sio_send(uint8_t port, uint16_t len, uint8_t *buffer);
void sio_send_str(uint8_t port, uint8_t *str);

// finalizer
void sio_done(uint8_t port);

#endif