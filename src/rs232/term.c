#include "rs232.h"
#include <stdio.h>

uint8_t tmp[1024];

int main(int argc, char **argv) {
	// init LPT
	sio_port *port = sio_init(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_BPS_9600, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE, SIO_FLOW_CONTROL_XONXOFF);
	while (true) {
		sio_buffer_put_str(&port->buffer_out, "HELLO ");
		sio_exit_code exit_code = sio_poll(port);
		uint16_t count = sio_buffer_get(&port->buffer_in, tmp);
		if (count > 0) { 
			tmp[count] = 0;
			printf("(%u)%s\n\r", exit_code, tmp); 
		}
	}
}