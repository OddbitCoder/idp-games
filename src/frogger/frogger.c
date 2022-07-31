#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "frogger.h"
#include "../common/avdc.h"
#include "../common/utils.h"
#include "../common/gdp.h"

#define CHAR_BUFFER_EMPTY '.'

#define LANE_GET_BUFFER(lane) lane->buffer[lane->buffer_idx]
#define LANE_GET_DIFF_BUFFER(lane) lane->buffer[1 - lane->buffer_idx]
#define SPRITE_GET_CHARS(sprite, part) sprite->def->chars->part[sprite->frame]
#define LANE_GET_SPRITE_DEF(lane) lane->def->sprite_defs[lane->sprite_def_idx]

uint8_t avdc_init_str[] = { 
	0xF8, // 16 SCAN LINES PER CHAR ROW
	0x3E, 
	0xBF, 
	0x05,
	0x90, // ACTIVE CHAR ROWS PER SCREEN = 16 + 1
	0x83, 
	0x0B, 
	0xEA, 
	0x00, 
	0x30 
};

typedef enum {
	DIR_LEFT,
	DIR_RIGHT
} dir;

typedef struct {
	uint8_t frame_count;
	uint8_t **left;
	uint8_t **body;
	uint8_t **right;
} sprite_chars;

typedef struct {
	sprite_chars *chars;
	uint8_t body_size;
	uint8_t char_count;
	uint8_t gap; // gap to the left of this sprite
	uint8_t anim_delay;
} sprite_def;

typedef struct {
	uint8_t sprite_def_count;
	sprite_def **sprite_defs;
	uint8_t move_delay;
	uint8_t y;
	dir dir;
} lane_def;

typedef struct {
	sprite_def *def;
	int8_t x;
	uint8_t frame;
	uint8_t anim_delay_counter;
} sprite;

typedef struct {
	sprite items[16];
	int8_t idx_s;
	int8_t idx_e;
} sprite_queue;

typedef struct {
	lane_def *def;
	int8_t gap;
	uint8_t buffer_idx;
	uint8_t buffer[2][132]; 
	sprite_queue sprites;
	uint8_t move_delay_counter;
	int8_t sprite_def_idx;
} lane;

uint8_t *aux_chars_log_left[] =  { "\x01""A" };
uint8_t *aux_chars_log_body[] =  { "\x01""B" };
uint8_t *aux_chars_log_right[] = { "\x03""CDE" };

uint8_t *aux_chars_turtle_left[] = { "\x04""FGHI", "\x04""JKLM", "\x04""NOPR" };
uint8_t *aux_chars_turtle_dive_left[] = { "\x04""fghi", "\x04""jklm", "\x04""nopr" };
uint8_t *aux_chars_turtle_empty[] = { "\x00""", "\x00""", "\x00""" };

uint8_t *aux_chars_croc_left[] =  { "\x04""STUV", "\x04""STUV" };
uint8_t *aux_chars_croc_body[] =  { "\x01""X",    "\x01""X" };
uint8_t *aux_chars_croc_right[] = { "\x04""YZQ0", "\x04""1234" };

sprite_chars sprite_chars_log = {
	/*frame_count*/ 1,
	aux_chars_log_left,
	aux_chars_log_body,
	aux_chars_log_right
};

sprite_chars sprite_chars_turtle = {
	/*frame_count*/ 3,
	aux_chars_turtle_left,
	aux_chars_turtle_empty,
	aux_chars_turtle_empty
};

sprite_chars sprite_chars_turtle_dive = {
	/*frame_count*/ 3,
	aux_chars_turtle_dive_left,
	aux_chars_turtle_empty,
	aux_chars_turtle_empty
};

sprite_chars sprite_chars_croc = {
	/*frame_count*/ 2,
	aux_chars_croc_left,
	aux_chars_croc_body,
	aux_chars_croc_right
};

sprite_def sprite_def_log_6_18 = {
	&sprite_chars_log,
	/*body_size*/ 14,
	/*char_count*/ 18,
	/*gap*/ 6,
	/*anim_delay*/ 0
};

sprite_def sprite_def_log_9_18 = {
	&sprite_chars_log,
	/*body_size*/ 14,
	/*char_count*/ 18,
	/*gap*/ 9,
	/*anim_delay*/ 0
};

sprite_def sprite_def_log_11_27 = {
	&sprite_chars_log,
	/*body_size*/ 23,
	/*char_count*/ 27,
	/*gap*/ 11,
	/*anim_delay*/ 0
};

sprite_def sprite_def_log_16_13 = {
	&sprite_chars_log,
	/*body_size*/ 9,
	/*char_count*/ 13,
	/*gap*/ 16,
	/*anim_delay*/ 0
};

sprite_def sprite_def_log_11_13 = {
	&sprite_chars_log,
	/*body_size*/ 9,
	/*char_count*/ 13,
	/*gap*/ 11,
	/*anim_delay*/ 0
};

sprite_def sprite_def_turtle_dive_18 = {
	&sprite_chars_turtle_dive,
	/*body_size*/ 0,
	/*char_count*/ 4,
	/*gap*/ 18,
	/*anim_delay*/ 0
};

sprite_def sprite_def_turtle_dive_1 = {
	&sprite_chars_turtle_dive,
	/*body_size*/ 0,
	/*char_count*/ 4,
	/*gap*/ 1,
	/*anim_delay*/ 0	
};

sprite_def sprite_def_turtle_dive_5 = {
	&sprite_chars_turtle_dive,
	/*body_size*/ 0,
	/*char_count*/ 4,
	/*gap*/ 5,
	/*anim_delay*/ 0	
};

sprite_def sprite_def_turtle_7 = {
	&sprite_chars_turtle,
	/*body_size*/ 0,
	/*char_count*/ 4,
	/*gap*/ 7,
	/*anim_delay*/ 0
};

sprite_def sprite_def_turtle_1 = {
	&sprite_chars_turtle,
	/*body_size*/ 0,
	/*char_count*/ 4,
	/*gap*/ 1,
	/*anim_delay*/ 0
};

sprite_def *aux_sprite_def_0_0[] = {
	&sprite_def_log_6_18,
	&sprite_def_log_9_18,
	&sprite_def_log_9_18
};

lane_def lane_def_0_0 = {
	/*sprite_def_count*/ 3,
	aux_sprite_def_0_0,
	/*move_delay*/ 0,
	/*y*/ 0,
	/*dir*/ DIR_RIGHT
};

sprite_def *aux_sprite_def_0_1[] = {
	&sprite_def_turtle_dive_18,
	&sprite_def_turtle_dive_1,
	&sprite_def_turtle_7,
	&sprite_def_turtle_1,
	&sprite_def_turtle_7,
	&sprite_def_turtle_1,
	&sprite_def_turtle_7,
	&sprite_def_turtle_1
};

lane_def lane_def_0_1 = {
	/*sprite_def_count*/ 8,
	aux_sprite_def_0_1,
	/*move_delay*/ 0,
	/*y*/ 2,
	/*dir*/ DIR_LEFT
};

sprite_def *aux_sprite_def_0_2[] = {
	&sprite_def_log_11_27
};

lane_def lane_def_0_2 = {
	/*sprite_def_count*/ 1,
	aux_sprite_def_0_2,
	/*move_delay*/ 0,
	/*y*/ 4,
	/*dir*/ DIR_RIGHT
};

sprite_def *aux_sprite_def_0_3[] = {
	&sprite_def_log_16_13,
	&sprite_def_log_11_13
};

lane_def lane_def_0_3 = {
	/*sprite_def_count*/ 2,
	aux_sprite_def_0_3,
	/*move_delay*/ 0,
	/*y*/ 6,
	/*dir*/ DIR_RIGHT
};

sprite_def *aux_sprite_def_0_4[] = {
	&sprite_def_turtle_dive_5,
	&sprite_def_turtle_1,
	&sprite_def_turtle_1,
	&sprite_def_turtle_1
};

lane_def lane_def_0_4 = {
	/*sprite_def_count*/ 4,
	aux_sprite_def_0_4,
	/*move_delay*/ 0,
	/*y*/ 8,
	/*dir*/ DIR_LEFT
};

lane lane_0_0 = { &lane_def_0_0 };
lane lane_0_1 = { &lane_def_0_1 };
lane lane_0_2 = { &lane_def_0_2 };
lane lane_0_3 = { &lane_def_0_3 };
lane lane_0_4 = { &lane_def_0_4 };

lane *lanes_0[] = {
	&lane_0_0,
	&lane_0_1,
	&lane_0_2,
	&lane_0_3,
	&lane_0_4,
	&lane_0_0,
	&lane_0_1,
	&lane_0_2,
	&lane_0_3,
	&lane_0_4
};

uint8_t buffer[256];

void debug_print_8(uint8_t y, int8_t val) {
	itoa(val, buffer, 10);
	avdc_write_str_at_cursor_pos(y, 0, buffer, NULL);
}

sprite *sprite_queue_push_left(sprite_queue *queue) {
	queue->idx_s--;
	if (queue->idx_s < 0) { 
		queue->idx_s = 15; 
	}
	return &queue->items[queue->idx_s];
}

sprite *sprite_queue_push_right(sprite_queue *queue) {
	sprite *sprite = &queue->items[queue->idx_e];
	queue->idx_e++;
	if (queue->idx_e == 16) { 
		queue->idx_e = 0; 
	}
	return sprite;
}

sprite *sprite_queue_pop_left(sprite_queue *queue) {
	sprite *sprite = &queue->items[queue->idx_s];
	queue->idx_s++;
	if (queue->idx_s == 16) { 
		queue->idx_s = 0; 
	}
	return sprite;
}

sprite *sprite_queue_pop_right(sprite_queue *queue) {
	queue->idx_e--;
	if (queue->idx_e < 0) { 
		queue->idx_e = 15; 
	}
	return &queue->items[queue->idx_e];
}

void sprite_queue_init(sprite_queue *queue) {
	queue->idx_s = 0;
	queue->idx_e = 0;
}

/*
typedef struct {
	sprite_chars *chars;
	uint8_t body_size;
	uint8_t char_count;
	uint8_t gap; // gap before this sprite
	uint8_t anim_delay;
} sprite_def;

typedef struct {
	sprite_def *def;
	int8_t x;
	uint8_t frame;
	uint8_t anim_delay_counter;
} sprite;
*/

void sprite_init(sprite *sprite, sprite_def *sprite_def, uint8_t x) {
	// *** init def
	sprite->def = sprite_def;
	// *** init x
	sprite->x = x;
	// *** init frame
	sprite->frame = 0;
	// *** init anim_delay_counter
	sprite->anim_delay_counter = sprite_def->anim_delay;
}

void sprite_render(sprite *sprite, uint8_t *buffer) { 
	uint8_t sz;
	buffer += 35 + sprite->x; // NOTE: x = 0 starts at 35
	memcpy(buffer, SPRITE_GET_CHARS(sprite, left) + 1, sz = *SPRITE_GET_CHARS(sprite, left));
	buffer += sz;
	sz = *SPRITE_GET_CHARS(sprite, body);
	for (uint8_t i = 0; i < sprite->def->body_size; i++) {
		memcpy(buffer, SPRITE_GET_CHARS(sprite, body) + 1, sz);
		buffer += sz;
	}
	memcpy(buffer, SPRITE_GET_CHARS(sprite, right) + 1, *SPRITE_GET_CHARS(sprite, right));
}

/*
typedef struct {
	uint8_t sprite_def_count;
	sprite_def **sprite_defs;
	uint8_t move_delay;
	uint8_t y;
	dir dir;
} lane_def;

typedef struct {
	x lane_def *def;
	x int8_t gap;
	x uint8_t buffer_idx;
	x uint8_t buffer[2][132]; 
	x sprite_queue sprites;
   	x uint8_t move_delay_counter;
	x int8_t sprite_def_idx;
} lane;
*/

void lane_sprite_def_idx_inc(lane *lane) {
	if (++lane->sprite_def_idx == (signed)lane->def->sprite_def_count) { 
		lane->sprite_def_idx = 0; 
	}
}

void lane_sprite_def_idx_dec(lane *lane) {
	if (--lane->sprite_def_idx < 0) { 
		lane->sprite_def_idx = lane->def->sprite_def_count - 1;
	}
}

void lane_init(lane *lane) {
	// *** init buffer_idx, buffer
	lane->buffer_idx = 0;
	memset(lane->buffer[1], 0, 132);
	memset(lane->buffer[0], CHAR_BUFFER_EMPTY, 132);
	// *** init sprite_def_idx
	lane->sprite_def_idx = 0;
	// *** init sprites
	sprite_queue_init(&lane->sprites);
	// create the first sprite
	sprite_def *sprite_def = LANE_GET_SPRITE_DEF(lane);
	sprite *sprite = sprite_queue_push_right(&lane->sprites);
	sprite_init(sprite, sprite_def, sprite_def->gap);
	// offset the sprite for [0, sprite->def->char_count + sprite_def->gap - 1]
	sprite->x -= sys_rand() % (sprite->def->char_count + sprite_def->gap);
	// create other sprites
	while (true) {
		lane_sprite_def_idx_inc(lane);
		sprite_def = LANE_GET_SPRITE_DEF(lane);
		uint8_t x = sprite->x + sprite->def->char_count + sprite_def->gap;
		if (x >= 61) {
			// *** init gap
			if (lane->def->dir == DIR_LEFT) {
				lane->gap = 61 - sprite->x - sprite->def->char_count; 
			} else { // DIR_RIGHT
				lane->gap = lane->sprites.items[0].x;
				lane->sprite_def_idx = 0;
			}
			break;
		}
		sprite = sprite_queue_push_right(&lane->sprites);
		sprite_init(sprite, sprite_def, x);
	}
	// render sprites
	for (int8_t i = 0; i < lane->sprites.idx_e; i++) {
		sprite_render(&lane->sprites.items[i], LANE_GET_BUFFER(lane));
	}
	// *** init move_delay_counter
	lane->move_delay_counter = lane->def->move_delay;
}

void lane_update(lane *lane) { 
	if (lane->move_delay_counter == 0) {
		sprite *sprite;
		sprite_def *sprite_def;
		int8_t i = lane->sprites.idx_s;
		if (lane->def->dir == DIR_LEFT) {
			// move sprites
			while (i != lane->sprites.idx_e) {
				sprite = &lane->sprites.items[i];
				sprite->x--;
				if (sprite->x <= -sprite->def->char_count) { 
					// remove sprite
					sprite_queue_pop_left(&lane->sprites); 
				}
				if (++i == 16) { i = 0; }
			}
			// create sprite
			sprite_def = LANE_GET_SPRITE_DEF(lane);
			if (++lane->gap == sprite_def->gap + 1) {
				lane_sprite_def_idx_inc(lane);
	 			sprite = sprite_queue_push_right(&lane->sprites);
	 			sprite_init(sprite, sprite_def, 60);
		 		lane->gap = -sprite_def->char_count + 1;
			}
		} else { // DIR_RIGHT
			// move sprites
			while (i != lane->sprites.idx_e) {
				sprite = &lane->sprites.items[i];
				sprite->x++;
				if (sprite->x > 60) { 
					// remove sprite
					sprite_queue_pop_right(&lane->sprites);
					break; // this is the last (right-most) sprite
				}
				if (++i == 16) { i = 0; }
			}
			// create sprite
			uint8_t target_gap = LANE_GET_SPRITE_DEF(lane)->gap;
			if (++lane->gap == target_gap + 1) {
				lane_sprite_def_idx_dec(lane);
				sprite_def = LANE_GET_SPRITE_DEF(lane);
			 	sprite = sprite_queue_push_left(&lane->sprites);
			 	sprite_init(sprite, sprite_def, -sprite_def->char_count + 1); 
			 	lane->gap = sprite->x; 
			}
		}
		// clear buffer
		lane->buffer_idx = 1 - lane->buffer_idx;
		memset(LANE_GET_BUFFER(lane), CHAR_BUFFER_EMPTY, 132);
		// render sprites
		i = lane->sprites.idx_s;
		uint8_t y = 0;
		while (i != lane->sprites.idx_e) {
			sprite = &lane->sprites.items[i];
			//debug_print_8(y++, i);
			sprite_render(sprite, LANE_GET_BUFFER(lane));
			if (++i == 16) { i = 0; }
		}
		lane->move_delay_counter = lane->def->move_delay;
	} else {
		lane->move_delay_counter--;
	}
}

void lane_render(lane *lane) { // TODO: optimize rendering (diff the two buffers)
	// NOTE: lane->buffer_idx points to the updated buffer, 1 - lane->buffer_idx points to the old buffer
	//buffer[61] = 0;
	//memcpy(buffer, LANE_GET_BUFFER(lane) + 35, 61);
	//avdc_write_str_at_pointer_pos(lane->def->y, 35, buffer, NULL);

	uint8_t *buffer = LANE_GET_BUFFER(lane) + 35;
	uint8_t *diff_buffer = LANE_GET_DIFF_BUFFER(lane) + 35;
	for (uint8_t i = 0; i < 61; i++, buffer++, diff_buffer++) {
		if (*buffer != *diff_buffer) {
			avdc_set_cursor(lane->def->y, i);
			avdc_write_at_cursor(*buffer, 0);
		}
	}

}	

int main() {
	//avdc_init();
	//avdc_init_ex(AVDC_MODE_CUSTOM, 0xC4, avdc_init_str);
	avdc_init_ex(AVDC_MODE_132, 0, NULL);
	// for (uint8_t i = 0; i < 62; i++) {
	// 	avdc_define_glyph(i, glyphs[i]);
	// }

	srand(timer());

	for (uint8_t i = 0; i < 5; i++) {
		lane_init(lanes_0[i]);
		lane_render(lanes_0[i]);
	}
	
	do {
		for (uint8_t i = 0; i < 5; i++) {
			lane_update(lanes_0[i]);
			lane_render(lanes_0[i]);
		}
		//avdc_clear_screen();
	} while (!kbhit());

	avdc_done();
	return 0;
}