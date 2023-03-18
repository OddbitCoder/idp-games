#include <stdio.h>
#include "../common/sio.h"

#define MAX_IN 128

uint8_t io_buffer[128 + 1]; // one byte extra for '\0'

void console_input(char *io_buffer) {
    printf("? ");
    char ch;
    uint8_t len = 0;
    do {
        while (!(ch = kbhit()));
        if (ch >= 32 && ch <= 126) {
            if (len < MAX_IN) {
                io_buffer[len] = ch;
                printf("%c", ch);
                io_buffer[++len] = '\0'; 
            } 
        } else if (ch == '\b') { 
            if (len > 0) {
                printf("\b \b");
                io_buffer[--len] = '\0';
            }
        } 
    } while (ch != '\r' || len == 0);
    printf("\n\r");
}

void main() {
	printf("\n\r*** Iskra Delta Partner AI ***\n\r\n\r");

	sio_port *port = sio_init_ex(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_BAUDS_9600, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE, SIO_FLOW_CONTROL_RTSCTS,
		/*out_buffer_sz*/128 + 1, // one byte extra for '\0'
		/*in_buffer_sz*/128,
		/*in_buffer_ext*/64,
		/*no_activity_thr*/100);

	do {	
		console_input(io_buffer);
		printf("\n\r");
		sio_buffer_put_str(&port->buffer_out, io_buffer);
		sio_buffer_put_ch(&port->buffer_out, '\0');
		
		uint16_t len;
		bool done = false;
		do {
			sio_poll(port);
			len = sio_buffer_get(&port->buffer_in, io_buffer);
			if (len > 0) {
				done = io_buffer[len - 1] == '\0';
				io_buffer[len] = '\0';
				printf("%s", io_buffer);
			}
		} while (!done);
		printf("\n\r\n\r");
	} while (true);
	
	sio_done(port);
}