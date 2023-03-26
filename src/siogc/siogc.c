#include <stdio.h>
#include "../common/sio.h"

void main() {
	sio_port *port = sio_init_ex(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_BAUDS_9600, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE, SIO_FLOW_CONTROL_RTSCTS,
		/*out_buffer_sz*/79 + 1, // one byte extra for '\0'
		/*in_buffer_sz*/128,
		/*in_buffer_ext*/64,
		/*no_activity_thr*/100);

	sio_done(port);
}