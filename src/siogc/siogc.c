#include <stdio.h>
#include <string.h>
#include "../common/sio.h"
#include "../common/gdp.h"
#include "../common/avdc.h"

#define RENDER_BUTTON(x, id) if (gc##id.x.state != gc##id##_new.x.state) { draw_button(&gc_btn_##x, gc##id##_new.x.state, id); }

typedef uint8_t bool8_t;

typedef struct {
	uint16_t x;
	uint8_t y;
	uint8_t h;
	uint8_t **gfx_down;
	uint8_t **gfx_up;
} gc_button;

typedef struct {
	bool8_t count; 
	bool8_t state; 
} gc_btn_state;

typedef struct {
	gc_btn_state up;
	gc_btn_state down;
	gc_btn_state left;
	gc_btn_state right;
	gc_btn_state l_trigger;
	gc_btn_state r_trigger;
	gc_btn_state l_shoulder;
	gc_btn_state r_shoulder;
	gc_btn_state x;
	gc_btn_state y;
	gc_btn_state a;
	gc_btn_state b;
	gc_btn_state start;
	gc_btn_state back;
	gc_btn_state l_stick;
	gc_btn_state r_stick;
} gc_state;

typedef enum {
	PP_CMD_GC_STATE = 1
} pp_cmd; // PlayPal command

typedef enum {
	PP_RESP_GC_STATE = 1
} pp_response;

uint8_t *gfx_gc[] = {
	"\x06\x00\x18\x17\x33\x17\x18",
	"\x0A\x00\x15\x03\x16\x01\x33\x01\x16\x03\x15",
	"\x08\x00\x12\x9C\x3F\x00\x20\xA3\x12",
	"\x0A\x00\x10\x93\x04\x2D\x05\x2D\xA3\x02\x10",
	"\x0A\x00\x0E\x92\x03\x3F\x00\x28\x9A\x02\x0E",
	"\x0A\x00\x0E\xD0\x02\x3F\x00\x2E\x91\x01\x0E",
	"\x0A\x00\x0E\x02\x3F\x00\x1E\x0B\x09\x02\x0E",
	"\x0B\x00\x0D\x02\x3F\x00\x1C\x03\x0B\x9F\x02\x0D",
	"\x0B\x00\x0C\x01\x3F\x00\x1B\x03\x11\x9E\x01\x0C",
	"\x0B\x00\x0A\x02\x3F\x00\x1A\x02\x17\x95\x02\x0A",
	"\x0B\x00\x09\x01\x3F\x00\x1B\x01\x1B\x8E\x01\x09",
	"\x0B\x00\x08\x01\x3F\x00\x1A\x02\x1D\x95\x01\x08",
	"\x09\x87\x01\x3F\x00\x1A\x01\x21\x8D\x8F",
	"\x09\x87\x01\x3F\x00\x19\x01\x23\x8C\x8F",
	"\x09\x86\x01\x3F\x00\x19\x01\x25\x8C\x8E",
	"\x0B\x85\x01\x14\x09\x3B\x01\x1A\x02\x0B\x8C\x8D",
	"\x0E\x85\x01\x11\x03\x09\x03\x37\x01\x12\xB3\x43\x0B\x8B\x8D",
	"\x0F\x84\x01\x10\x02\x0F\x02\x35\x01\x10\x96\x92\x02\x0B\x8C\x8C",
	"\x0F\x83\x01\x10\x01\x13\x01\x33\x01\x10\x8B\xA3\x01\x0F\x8C\x8B",
	"\x10\x83\x01\x0E\x02\x15\x02\x30\x01\x0F\x92\x94\x91\x01\x10\x8B\x8B",
	"\x10\x82\x01\x0E\x02\x17\x02\x2F\x01\x0E\x8C\x8E\x8A\x01\x0F\x8C\x8A",
	"\x12\x82\x01\x0E\x01\x0A\x05\x0A\x01\x2E\x01\x0D\x94\x01\x08\x43\x10\x8B\x8A",
	"\x12\x82\x01\x0D\x01\x0A\x8D\x01\x0A\x01\x2D\x01\x0C\x8E\x8E\x45\x10\x8B\x8A",
	"\x13\x81\x01\x0D\x01\x0B\x8A\x8A\x01\x0B\x01\x2C\x01\x0A\x97\x8E\x45\x10\x8C\x42",
	"\x13\x81\x01\x0D\x01\x0B\x47\x0B\x01\x2B\x01\x0A\x01\x09\x8D\x91\x43\x11\x8B\x42",
	"\x16\x81\x01\x0C\x01\x0C\x8D\x01\x0C\x01\x2A\x01\x08\x02\x0B\x8A\x99\x8A\x01\x0D\x92\x8B\x42",
	"\x13\x81\x01\x0C\x01\x0C\x8D\x01\x0C\x01\x2A\x8F\x01\x0D\x94\x91\x8D\xB3\xEB\x42",
	"\x13\x01\x0D\x01\x0C\x8D\x01\x0C\x01\x29\x8E\x02\x10\xA2\x93\x96\x92\x92\x8B\x01",
	"\x13\x01\x0C\x01\x0D\x8D\x01\x0D\x01\x28\x8D\x92\x04\x10\x93\x8B\xA3\x8D\x8B\x01",
	"\x14\x01\x0C\x8E\x08\x05\x08\xB1\x28\x8D\xD0\x94\x02\x0D\x8B\x92\x94\x91\x8D\x8B\x01",
	"\x16\x01\x0C\x8D\x01\x09\x03\x09\x8D\x01\x28\x8C\x8A\x8E\x01\x0B\x93\x8C\x8E\x8A\x8C\x8B\x01",
	"\x17\x01\x0C\x8D\x8A\x8D\x8B\x8D\x8A\x8D\x01\x28\x8C\x43\x08\x01\x09\x8B\x94\x01\x08\xDC\x8B\x01",
	"\x12\x01\x0C\x8D\xDE\x8B\x8E\xDD\x01\x28\x8C\xDE\xDF\x93\x8E\x8E\xEC\x8B\x01",
	"\x18\x01\x0C\x8D\x8A\x8D\x8B\x8D\x8A\x8D\x01\x0E\x03\x0C\x03\x08\x8C\xDE\xDE\x8B\x97\x8E\xEC\x8B\x01",
	"\x1B\x01\x0C\x8D\x01\x09\x03\x09\x8D\x01\x0C\x93\x01\x09\x93\x8F\x8C\xDD\x91\x8C\x93\x01\x09\x8D\x91\xDC\x8B\x01",
	"\x1A\x01\x0C\x8E\x08\x05\x08\xB1\x0A\x95\x8F\x95\x8F\x8C\x8A\x8A\x99\x8C\x8B\x02\x0B\x8A\x99\x8A\x8C\x8B\x01",
	"\x1A\x01\x0C\x01\x0D\x8D\x01\x0D\x01\x08\x96\x8E\x96\x01\x08\x8D\xD0\x94\x92\x93\x01\x0D\x94\x91\x8D\x8B\x01",
	"\x18\x01\x0D\x01\x0C\x8D\x01\x0C\x01\x08\x8E\x96\x8E\x02\x09\x8D\x8B\xA3\x8B\x02\x10\xA2\x95\x8B\x01",
	"\x18\x81\x01\x0C\x01\x0C\x8D\x01\x0C\x8F\x8D\x97\x8D\x02\x0C\xD0\x92\x96\x93\x92\x04\x10\x95\x8B\x42",
	"\x18\x81\x01\x0C\x01\x0C\x8D\x01\x0C\x8F\x8B\x02\x09\x8B\x02\x0E\xEB\xB5\xD0\x94\x02\x0D\x8F\x8B\x42",
	"\x18\x81\x01\x0D\x01\x0B\x47\x0B\x01\x09\x03\x0C\x03\x10\x8A\x02\x0D\x8A\x8E\x01\x0B\x02\x08\x8B\x42",
	"\x15\x81\x01\x0D\x01\x0B\x8A\x8A\x01\x0B\x01\x2C\x01\x10\x43\x08\x01\x09\x01\x09\x8C\x42",
	"\x12\x82\x01\x0D\x01\x0A\x8D\x01\x0A\x01\x2D\x01\x10\xDE\xDF\x02\x0A\x8B\x8A",
	"\x11\x82\x01\x0E\x01\x0A\x05\x0A\x01\x2E\x01\x10\xDE\xDE\x01\x0C\x8B\x8A",
	"\x10\x82\x01\x0E\x02\x17\x02\x2F\x01\x0F\xDD\x91\x8C\x02\x0C\x8C\x8A",
	"\x11\x83\x01\x0E\x02\x15\x02\x30\x01\x0F\x8A\x8A\x99\x8C\x01\x0E\x8B\x8B",
	"\x14\x83\x01\x10\x01\x13\x01\x0D\x4B\x04\x4B\x0C\x01\x0F\xD0\x94\x92\x02\x0E\x8C\x8B",
	"\x13\x84\x01\x10\x02\x0F\x02\x0F\x4B\x04\x4B\x0C\x01\x0E\x8B\xA3\x01\x0F\x8C\x8C",
	"\x0F\x85\x01\x11\x03\x09\x03\x37\x01\x0B\x92\x96\x02\x10\x8B\x8D",
	"\x0C\x85\x01\x14\x09\x3B\x01\x0A\xDB\x06\x11\x8C\x8D",
	"\x0B\x86\x01\x3F\x00\x19\x01\x0A\x02\x19\x8C\x8E",
	"\x09\x87\x01\x3F\x00\x19\x01\x23\x8C\x8F",
	"\x09\x87\x01\x3F\x00\x1A\x01\x21\x8D\x8F",
	"\x0B\x00\x08\x01\x3F\x00\x1A\x02\x1D\x95\x01\x08",
	"\x0B\x00\x09\x01\x3F\x00\x1B\x01\x1B\x8E\x01\x09",
	"\x0B\x00\x0A\x02\x3F\x00\x1A\x02\x17\x95\x02\x0A",
	"\x0B\x00\x0C\x01\x25\x2D\x08\x03\x11\x9E\x01\x0C",
	"\x0D\x00\x0D\x02\x21\x02\x2D\x02\x09\x03\x0B\x9F\x02\x0D",
	"\x0C\x00\x0F\x01\x1D\x03\x31\x03\x09\x0B\x09\x01\x0F",
	"\x0A\x00\x10\x02\x19\x02\x37\x02\x19\x02\x10",
	"\x0A\x00\x12\x03\x13\x03\x3B\x03\x13\x03\x12",
	"\x0B\x00\x15\x04\x0B\x04\x3F\x82\x04\x0B\x04\x15",
	"\x08\x00\x19\x0B\x3F\x00\x0A\x0B\x19"
};

uint8_t *gfx_btn_round_up[] = {
	"\x02\x01\x84",
	"\x01\x86",
	"\x02\x85\xC1",
	"\x02\x85\xC1",
	"\x02\x84\xC2",
	"\x03\x01\x81\xC3"
};

uint8_t *gfx_btn_round_down[] = {
	"\x02\x01\xC4",
	"\x01\xC6",
	"\x01\xC6",
	"\x01\xC6",
	"\x01\xC6",
	"\x02\x01\xC4"
};

uint8_t *gfx_btn_oval_up[] = {
	"\x03\x05\x02\x83",
	"\x03\x03\x02\x85",
	"\x03\x01\x02\x86",
	"\x03\x01\x86\x02",
	"\x02\x85\x02",
	"\x02\x83\x02"
};

uint8_t *gfx_btn_oval_down[] = {
	"\x02\x07\xC3",
	"\x02\x05\xC5",
	"\x02\x03\xC6",
	"\x02\x01\xC6",
	"\x01\xC5",
	"\x01\xC3"
};

uint8_t *gfx_btn_lr_down[] = {
	"\x01\xC6",
	"\x01\xC6",
	"\x01\xC6"
};

uint8_t *gfx_btn_ud_down[] = {
	"\x01\xC3",
	"\x01\xC3",
	"\x01\xC3",
	"\x01\xC3",
	"\x01\xC3",
	"\x01\xC3"
};

uint8_t *gfx_btn_left_up[] = {
	"\x03\x81\xC1\x84",
	"\x02\xC1\x85",
	"\x03\x81\xC1\x84"
};

uint8_t *gfx_btn_right_up[] = {
	"\x03\x84\xC1\x81",
	"\x02\x85\xC1",
	"\x03\x84\xC1\x81"
};

uint8_t *gfx_btn_up_up[] = {
	"\x03\x81\xC1\x81",
	"\x03\xC1\x81\xC1",
	"\x01\x83",
	"\x01\x83",
	"\x01\x83",
	"\x01\x83"
};

uint8_t *gfx_btn_down_up[] = {
	"\x01\x83",
	"\x01\x83",
	"\x01\x83",
	"\x01\x83",
	"\x03\xC1\x81\xC1",
	"\x03\x81\xC1\x81"
};

uint8_t *gfx_btn_lt_up[] = {
	"\x02\x0A\xD7",
	"\x04\x07\xC3\x96\xC1",
	"\x04\x04\xC3\x84\xD6",
	"\x04\x02\xC2\x83\xC4",
	"\x04\xC2\x82\xC3\x05",
	"\x04\xC1\x81\xC2\x04"
};

uint8_t *gfx_btn_lt_down[] = {
	"\x02\x0A\x97",
	"\x03\x07\x83\xD6",
	"\x03\x04\x83\xC4",
	"\x03\x02\x82\xC3",
	"\x03\x82\xC2\x08",
	"\x03\x01\xC1\x06"
};

uint8_t *gfx_btn_rt_up[] = {
	"\x01\xD7",
	"\x03\xC1\x96\xC3",
	"\x03\xD6\x84\xC3",
	"\x04\x16\xC4\x83\xC2",
	"\x05\x15\x05\xC3\x82\xC2",
	"\x05\x19\x04\xC2\x81\xC1"
};

uint8_t *gfx_btn_rt_down[] = {
	"\x01\x97",
	"\x03\x01\xD6\x83",
	"\x03\x16\xC4\x83",
	"\x03\x1A\xC3\x82",
	"\x04\x15\x08\xC2\x82",
	"\x03\x19\x06\xC1"
};

gc_button gc_btn_y = {
	381 << 1, 
	117,
	6,
	gfx_btn_round_down,
	gfx_btn_round_up
};

gc_button gc_btn_x = {
	366 << 1, 
	127, 
	6,
	gfx_btn_round_down,
	gfx_btn_round_up
};

gc_button gc_btn_a = {
	380 << 1, 
	137, 
	6,
	gfx_btn_round_down,
	gfx_btn_round_up
};

gc_button gc_btn_b = {
	395 << 1, 
	127, 
	6,
	gfx_btn_round_down,
	gfx_btn_round_up
};

gc_button gc_btn_start = {
	339 << 1, 
	131, 
	6,
	gfx_btn_oval_down,
	gfx_btn_oval_up
};

gc_button gc_btn_back = {
	324 << 1, 
	131, 
	6,
	gfx_btn_oval_down,
	gfx_btn_oval_up
};

gc_button gc_btn_up = {
	298 << 1, 
	120, 
	6,
	gfx_btn_ud_down,
	gfx_btn_up_up
};

gc_button gc_btn_down = {
	298 << 1, 
	133, 
	6,
	gfx_btn_ud_down,
	gfx_btn_down_up
};

gc_button gc_btn_right = {
	303 << 1, 
	128, 
	3,
	gfx_btn_lr_down,
	gfx_btn_right_up
};

gc_button gc_btn_left = {
	290 << 1, 
	128, 
	3,
	gfx_btn_lr_down,
	gfx_btn_left_up
};

gc_button gc_btn_l_trigger = { 
	283 << 1, 
	97, 
	6,
	gfx_btn_lt_down,
	gfx_btn_lt_up
};

gc_button gc_btn_r_trigger = { 
	367 << 1, 
	97, 
	6,
	gfx_btn_rt_down,
	gfx_btn_rt_up
};

void draw_button(gc_button *btn, bool8_t state, uint8_t gc_id) {
	gdp_draw_sprite(state ? btn->gfx_down : btn->gfx_up, btn->h - 1, btn->x - (gc_id == 0 ? 171 << 1 : 0), btn->y);
}

void put_pp_cmd_gc_state(sio_port *port, uint8_t id) {
	if (port->buffer_out.size - port->buffer_out.count >= 4) {
		// command code
		sio_buffer_put_ch(&port->buffer_out, PP_CMD_GC_STATE);
		sio_buffer_put_ch(&port->buffer_out, (uint8_t)~PP_CMD_GC_STATE);
		// data
		sio_buffer_put_ch(&port->buffer_out, id);
		// checksum
		sio_buffer_put_ch(&port->buffer_out, id);
	}
}

uint8_t resp_id = 1;

uint8_t last_id = 0;
uint8_t last_delta_id = 255;

uint8_t fake_data_id = 0;

void put_fake_pp_response_gc_state(sio_port *port) {
	uint8_t checksum = 0;
	// response code
	sio_buffer_put_ch(&port->buffer_in, PP_RESP_GC_STATE);
	sio_buffer_put_ch(&port->buffer_in, (uint8_t)~PP_RESP_GC_STATE);
	// data
	sio_buffer_put_ch(&port->buffer_in, resp_id); // response id
	sio_buffer_put_ch(&port->buffer_in, last_id); // delta id
	sio_buffer_put_ch(&port->buffer_in, 2); // number of controllers
	// gc 1
	sio_buffer_put_ch(&port->buffer_in, 0); // GC id
	if (fake_data_id == 0) {
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
	} else {
		sio_buffer_put_ch(&port->buffer_in, 255); 
		sio_buffer_put_ch(&port->buffer_in, 255);
		sio_buffer_put_ch(&port->buffer_in, 255);
		sio_buffer_put_ch(&port->buffer_in, 255);
		checksum += 255+255+255+255;
	}
	// gc 2
	sio_buffer_put_ch(&port->buffer_in, 1); // GC id
	if (fake_data_id == 0) {
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
		sio_buffer_put_ch(&port->buffer_in, 0);
	} else {
		sio_buffer_put_ch(&port->buffer_in, 255); 
		sio_buffer_put_ch(&port->buffer_in, 255);
		sio_buffer_put_ch(&port->buffer_in, 255);
		sio_buffer_put_ch(&port->buffer_in, 255);
		checksum += 255+255+255+255;
	}
	// checksum
	sio_buffer_put_ch(&port->buffer_in, checksum + resp_id + last_id + 2 + 1);
	resp_id++;
	fake_data_id = (fake_data_id + 1) & 1;
}

void sio_read_gc_state(sio_port *port, gc_state *state, uint8_t *checksum) {
	uint8_t b;
	// up, down, left, right
	*checksum += (b = sio_buffer_get_ch(&port->buffer_in));
	state->right.state = b & 1;
	state->right.count = (b >>= 1) & 1;
	state->left.state = (b >>= 1) & 1;
	state->left.count = (b >>= 1) & 1;
	state->down.state = (b >>= 1) & 1;
	state->down.count = (b >>= 1) & 1;
	state->up.state = (b >>= 1) & 1;
	state->up.count = (b >>= 1) & 1;
	// l_trigger, r_trigger, l_shoulder, r_shoulder
	*checksum += (b = sio_buffer_get_ch(&port->buffer_in));
	state->r_shoulder.state = b & 1;
	state->r_shoulder.count = (b >>= 1) & 1;
	state->l_shoulder.state = (b >>= 1) & 1;
	state->l_shoulder.count = (b >>= 1) & 1;
	state->r_trigger.state = (b >>= 1) & 1;
	state->r_trigger.count = (b >>= 1) & 1;
	state->l_trigger.state = (b >>= 1) & 1;
	state->l_trigger.count = (b >>= 1) & 1;
	// x, y, a, b
	*checksum += (b = sio_buffer_get_ch(&port->buffer_in));
	state->b.state = b & 1;
	state->b.count = (b >>= 1) & 1;
	state->a.state = (b >>= 1) & 1;
	state->a.count = (b >>= 1) & 1;
	state->y.state = (b >>= 1) & 1;
	state->y.count = (b >>= 1) & 1;
	state->x.state = (b >>= 1) & 1;
	state->x.count = (b >>= 1) & 1;
	// start, back, l_stick, r_stick
	*checksum += (b = sio_buffer_get_ch(&port->buffer_in));
	state->r_stick.state = b & 1;
	state->r_stick.count = (b >>= 1) & 1;
	state->l_stick.state = (b >>= 1) & 1;
	state->l_stick.count = (b >>= 1) & 1;
	state->back.state = (b >>= 1) & 1;
	state->back.count = (b >>= 1) & 1;
	state->start.state = (b >>= 1) & 1;
	state->start.count = (b >>= 1) & 1;	
}

// NOTE: global vars are initialized to 0
// current GC state
gc_state gc0;
gc_state gc1;
// updated GC state
gc_state gc0_new;
gc_state gc1_new;

gc_state *gc_new[] = {
	&gc0_new,
	&gc1_new
};

bool8_t gc_updated[] = {
	false,
	false
};

bool8_t gc_data_valid = false;

void main() {
	//avdc_init();
	gdp_init();
	sio_port *port = sio_init_ex(SIO_PORT_LPT, SIO_MODE_POLLING, SIO_BAUDS_9600, SIO_DATA_BITS_8, SIO_STOP_BITS_1, SIO_PARITY_NONE, SIO_FLOW_CONTROL_RTSCTS,
		/*out_buffer_sz*/128, 
		/*in_buffer_sz*/128,
		/*in_buffer_ext*/64,
		/*no_activity_thr*/100);

	gdp_set_write_page(1);
	gdp_draw(gfx_gc, 62, 98 << 1, 97);
	gdp_draw(gfx_gc, 62, 269 << 1, 97);
	gdp_set_display_page(1);

	while (!kbhit()) { // main "game" loop

		put_pp_cmd_gc_state(port, last_id);
		sio_send(port);	
		
		// < GAME LOGIC AND RENDERING GOES HERE >

		if (gc_data_valid) {
			//printf("valid ");
			// update GC 1
			if (!gc_updated[0]) {
				memset(&gc0_new, 0, sizeof(gc_state));
			}
			RENDER_BUTTON(up, 0);
			RENDER_BUTTON(down, 0);
			RENDER_BUTTON(left, 0);
			RENDER_BUTTON(right, 0);
			RENDER_BUTTON(l_trigger, 0);
			RENDER_BUTTON(r_trigger, 0);
			// WARNME: l_shoulder, r_shoulder ignored
			RENDER_BUTTON(x, 0);
			RENDER_BUTTON(y, 0);
			RENDER_BUTTON(a, 0);
			RENDER_BUTTON(b, 0);
			RENDER_BUTTON(start, 0);
			RENDER_BUTTON(back, 0);
			// WARNME: l_stick, r_stick ignored
			memcpy(&gc0, &gc0_new, sizeof(gc_state));
			// update GC 2
			if (!gc_updated[1]) {
				memset(&gc1_new, 0, sizeof(gc_state));
			}
			RENDER_BUTTON(up, 1);
			RENDER_BUTTON(down, 1);
			RENDER_BUTTON(left, 1);
			RENDER_BUTTON(right, 1);
			RENDER_BUTTON(l_trigger, 1);
			RENDER_BUTTON(r_trigger, 1);
			// WARNME: l_shoulder, r_shoulder ignored
			RENDER_BUTTON(x, 1);
			RENDER_BUTTON(y, 1);
			RENDER_BUTTON(a, 1);
			RENDER_BUTTON(b, 1);
			RENDER_BUTTON(start, 1);
			RENDER_BUTTON(back, 1);
			// WARNME: l_stick, r_stick ignored
			memcpy(&gc1, &gc1_new, sizeof(gc_state));
		}

		gc_data_valid = false;
		gc_updated[0] = false;
		gc_updated[1] = false;

		// < ANY SYNC DELAY GOES HERE >
		
		sio_exchange(port); // receive controller state
		put_fake_pp_response_gc_state(port); // WARNME

		while (port->buffer_in.count >= 6) { // state is at least 6 bytes
			uint8_t cmd = sio_buffer_peek(&port->buffer_in, 0);
			uint8_t chck = sio_buffer_peek(&port->buffer_in, 1);
			if (chck == (uint8_t)~cmd && cmd == PP_RESP_GC_STATE) { 
				// |code|~code|id|delta id|n|gc id|gc 1 (4 bytes)|gc id|gc 2 (4 bytes)|...|checksum|
				uint8_t n = sio_buffer_peek(&port->buffer_in, 4);
				if (port->buffer_in.count >= 6 + n * 5) {
					// parse controller state 
					//printf("gc_state ");
					sio_buffer_get_ch(&port->buffer_in); // PP_RESP_GC_STATE
					sio_buffer_get_ch(&port->buffer_in); // ~PP_RESP_GC_STATE
					uint8_t id = sio_buffer_get_ch(&port->buffer_in);
					uint8_t checksum = id;
					uint8_t delta_id = sio_buffer_get_ch(&port->buffer_in);
					checksum += delta_id;
					sio_buffer_get_ch(&port->buffer_in); // n
					checksum += n;
					for (uint8_t i = 0; i < n; i++) {
						uint8_t gc_id = sio_buffer_get_ch(&port->buffer_in);
						//printf("c %u ", gc_id);
						checksum += gc_id;
						if (gc_id <= 1) {
							gc_updated[gc_id] = true;
							sio_read_gc_state(port, gc_new[gc_id], &checksum);
						} else { 
							// "drop" 4 bytes
							for (uint8_t k = 0; k < 4; k++) {
								checksum += sio_buffer_get_ch(&port->buffer_in);
							}
						}
					}
					uint8_t cs = sio_buffer_get_ch(&port->buffer_in);
					if (cs == checksum && delta_id != last_delta_id) {
						//printf("ok. ");
						last_id = id;
						last_delta_id = delta_id;
						gc_data_valid = true;
					} else {
						//printf("%u %u. ", checksum, cs);
					}
				} // else we wait for more data
			} else {
				// drop 1 byte
				sio_buffer_get_ch(&port->buffer_in);
			}
		}
	}

	sio_done(port);
	gdp_done();
	//avdc_done();
}