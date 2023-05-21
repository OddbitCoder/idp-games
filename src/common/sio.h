#ifndef __RS232_H__
#define __RS232_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	SIO_FLOW_CONTROL_RTSCTS,  // hardware flow control
	SIO_FLOW_CONTROL_XONXOFF, // software flow control
	SIO_FLOW_CONTROL_NONE
} sio_flow_control;

typedef enum { 
	SIO_BAUDS_153600 = 0, // not tested (not working?)
	SIO_BAUDS_9600   = 64,
	SIO_BAUDS_4800   = 128,
	SIO_BAUDS_2400   = 128 | 64
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
	//SIO_MODE_INTERRUPTS // not implemented
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
	uint8_t *values;
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
	sio_flow_control flow_control;
	bool xon_send; // their XON state
	bool xon_rcv;  // my XON state
} sio_port;

// initializer
sio_port *sio_init(sio_port_addr port_addr, sio_mode mode, sio_bauds bauds, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	sio_flow_control flow_control);
sio_port *sio_init_ex(sio_port_addr port_addr, sio_mode mode, sio_bauds bauds, sio_data_bits data_bits, sio_stop_bits stop_bits, sio_parity parity,
	sio_flow_control flow_control, uint16_t out_buffer_sz, uint16_t in_buffer_sz, uint16_t in_buffer_ext, uint16_t no_activity_thr);

// buffer
bool sio_buffer_put(sio_buffer *buffer, uint16_t len, uint8_t *values);
bool sio_buffer_put_ch(sio_buffer *buffer, uint8_t ch);
bool sio_buffer_put_str(sio_buffer *buffer, uint8_t *str);
bool sio_buffer_empty(sio_buffer *buffer);
uint8_t sio_buffer_get_ch(sio_buffer *buffer);
uint8_t sio_buffer_peek(sio_buffer *buffer, uint16_t idx);
uint16_t sio_buffer_get(sio_buffer *buffer, uint8_t *dest);

// send & receive (non-blocking)
sio_exit_code sio_exchange(sio_port *port);

// send only (non-blocking)
bool sio_send(sio_port *port);

// control signals
void sio_set_rts(sio_port *port, bool state);
bool sio_check_cts(sio_port *port);

// finalizer
void sio_done(sio_port *port);

#endif