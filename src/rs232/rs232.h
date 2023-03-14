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

typedef enum {
	SIO_PORT_CRT = 0xD9,
	SIO_PORT_LPT = 0xDB,
	SIO_PORT_VAX = 0xE1,
	SIO_PORT_MOD = 0xE3
} sio_port_addr;

typedef struct {
	uint8_t *vals;
	uint16_t put_ptr;
	uint16_t get_ptr;
	uint16_t count;
	uint16_t size;
} sio_buffer;

typedef struct {
	sio_buffer buffer_in;
	sio_buffer buffer_out;
	uint16_t no_activity_thr;
	uint16_t in_buffer_ext;
	uint8_t wr5;
	sio_port_addr addr;
} sio_port;

// initializers
sio_port *sio_init(sio_port_addr port_addr, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity);
sio_port *sio_init_ex(sio_port_addr port_addr, sio_mode mode, sio_clock_mode clock, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	uint16_t out_buffer_sz, uint16_t in_buffer_sz, uint16_t in_buffer_ext, uint16_t no_activity_thr);

// receive buffer
//...

// send buffer
//...

// non-blocking send & receive
sio_exit_code sio_poll(sio_port *port);

// blocking send
void sio_send_ch(sio_port *port, uint8_t ch);
void sio_send(sio_port *port, uint16_t len, uint8_t *buffer);
void sio_send_str(sio_port *port, uint8_t *str);

// finalizer
void sio_done(sio_port *port);

#endif