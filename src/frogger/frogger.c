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
	uint8_t body_size_min;
	uint8_t body_mul_max;
	uint8_t anim_delay_frames;
} sprite_def;

typedef struct {
	uint8_t sprite_count;
	sprite_def **sprite_defs;
	uint8_t gap_min;
	uint8_t gap_max;
	uint8_t move_delay_frames;
	uint8_t y;
	dir dir;
} lane_def;

typedef struct {
	sprite_def *def;
	int8_t x;
	uint8_t frame;
	uint8_t body_size;
	uint8_t char_count;
	uint8_t anim_frames;
} sprite;

typedef struct {
	sprite items[16];
	int8_t idx_s;
	int8_t idx_e;
} sprite_queue;

typedef struct {
	lane_def *def;
	int8_t gap;
	uint8_t target_gap;
	uint8_t buffer_idx;
	uint8_t buffer[2][132]; 
	sprite_queue sprites;
	uint8_t move_frames;
} lane;

uint8_t *aux_chars_log_left[] = {  "A" };
uint8_t *aux_chars_log_body[] = {  "B" };
uint8_t *aux_chars_log_right[] = { "CDE" };

uint8_t *aux_chars_turtle_left[] = { "FGHI", "JKLM", "NOPR" };
uint8_t *aux_chars_turtle_empty[] = { "", "", "" };

uint8_t *aux_chars_croc_left[] = {  "STUV", "STUV" };
uint8_t *aux_chars_croc_body[] = {  "X",    "X" };
uint8_t *aux_chars_croc_right[] = { "YZQ0", "1234" };

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

sprite_chars sprite_chars_croc = {
	/*frame_count*/ 2,
	aux_chars_croc_left,
	aux_chars_croc_body,
	aux_chars_croc_right
};

sprite_def sprite_def_log_0_0 = {
	&sprite_chars_log,
	/*body_size_min*/ 1,
	/*body_size_max*/ 10,
	/*anim_delay_frames*/ 0
};

sprite_def sprite_def_croc_0_0 = {
	&sprite_chars_croc,
	/*body_size_min*/ 1,
	/*body_size_max*/ 1,
	/*anim_delay_frames*/ 0
};

sprite_def *aux_sprite_def_0_0[] = { 
	&sprite_def_log_0_0, 
	&sprite_def_croc_0_0 
};

lane_def lane_def_0_0 = {
	/*sprite_count*/ 2,
	aux_sprite_def_0_0,
	/*gap_min*/ 1,
	/*gap_max*/ 1,
	/*move_delay_frames*/ 0,
	/*y*/ 0,
	/*dir*/ DIR_RIGHT
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

void sprite_init(sprite *sprite, sprite_def *sprite_def, uint8_t x) {
	// *** init def
	sprite->def = sprite_def;
	// *** init x
	sprite->x = x;
	// *** init frame
	sprite->frame = 0;
	// *** init body_size
	sprite->body_size = (sys_rand() % (sprite_def->body_mul_max - sprite_def->body_size_min + 1)) + sprite_def->body_size_min; 
	// *** init char_count
	sprite->char_count = strlen(sprite->def->chars->left[0]) + sprite->body_size * strlen(sprite->def->chars->body[0]) + strlen(sprite->def->chars->right[0]); // WARNME: first item is measured
	// *** init anim_frames
	sprite->anim_frames = sprite_def->anim_delay_frames;
}

void sprite_render(sprite *sprite, uint8_t *buffer) { // NOTE: the buffer has 132 chars, x = 0 starts at 35
	// WARNME: slow (?)
	uint8_t *p_buffer = buffer + 35 + sprite->x;
	for (uint8_t *p_str = sprite->def->chars->left[sprite->frame]; *p_str != 0; p_str++, p_buffer++) {
		*p_buffer = *p_str;
	}
	for (uint8_t i = 0; i < sprite->body_size; i++) {
		for (uint8_t *p_str = sprite->def->chars->body[sprite->frame]; *p_str != 0; p_str++, p_buffer++) {
			*p_buffer = *p_str;
		}	
	}
	for (uint8_t *p_str = sprite->def->chars->right[sprite->frame]; *p_str != 0; p_str++, p_buffer++) {
		*p_buffer = *p_str;
	}
}

void lane_init(lane *lane, lane_def *def) {
	// *** init def
	lane->def = def;
	// *** init buffer_idx, buffer
	lane->buffer_idx = 0;
	memset(lane->buffer[1], 0, 132);
	memset(lane->buffer[0], CHAR_BUFFER_EMPTY, 132);
	// *** init sprites
	sprite_queue_init(&lane->sprites);
	// think of a gap from [lane->def->gap_min, lane->def->gap_max]
	uint8_t gap = (sys_rand() % (lane->def->gap_max - lane->def->gap_min + 1)) + lane->def->gap_min;
	// create the first sprite
	sprite_def *sprite_def = lane->def->sprite_defs[0]; // TODO: we should select one of the sprites from this array
	sprite *sprite = sprite_queue_push_right(&lane->sprites);
	sprite_init(sprite, sprite_def, /*x*/gap);
	// offset the sprite for [0, sprite->char_count + gap - 1]
	sprite->x -= sys_rand() % (sprite->char_count + gap);
	// create other sprites
	while (true) {
		gap = (sys_rand() % (lane->def->gap_max - lane->def->gap_min + 1)) + lane->def->gap_min;
		uint8_t x = sprite->x + sprite->char_count + gap;
		if (x >= 61) {
			// *** init gap, target_gap
			if (lane->def->dir == DIR_LEFT) {
				lane->gap = 61 - sprite->x - sprite->char_count; 
			} else { // DIR_RIGHT
				lane->gap = lane->sprites.items[0].x;
			}
			lane->target_gap = (sys_rand() % (lane->def->gap_max - lane->def->gap_min + 1)) + lane->def->gap_min;
			break; 
		}
		sprite = sprite_queue_push_right(&lane->sprites);
		sprite_init(sprite, sprite_def, x);
	}
	// render sprites [into the buffer]
	for (int8_t i = 0; i < lane->sprites.idx_e; i++) {
		sprite_render(&lane->sprites.items[i], lane->buffer[0]);
	}
	// *** init move_frames
	lane->move_frames = lane->def->move_delay_frames;
}

void lane_update(lane *lane) { 
	if (lane->move_frames == 0) {
		sprite *sprite;
		// move sprites
		int8_t i = lane->sprites.idx_s;
		if (lane->def->dir == DIR_LEFT) {
			while (i != lane->sprites.idx_e) {
				sprite = &lane->sprites.items[i];
				sprite->x--;
				if (sprite->x <= -sprite->char_count) { 
					// remove sprite
					sprite_queue_pop_left(&lane->sprites); 
				}
				if (++i == 16) { i = 0; }
			}
		} else { // DIR_RIGHT
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
		}
		// add another sprite
		if (++lane->gap == lane->target_gap + 1) {
	 		sprite_def *sprite_def = lane->def->sprite_defs[0]; // TODO: we should select one of the sprites from this array
	 		if (lane->def->dir == DIR_LEFT) {
	 			sprite = sprite_queue_push_right(&lane->sprites);
	 			sprite_init(sprite, sprite_def, 60);
	 		} else { // DIR_RIGHT
	 			sprite = sprite_queue_push_left(&lane->sprites);
	 			sprite_init(sprite, sprite_def, 0); 
	 			sprite->x = -sprite->char_count + 1; // NOTE: sprite->char_count is set in sprite_init
	 		}
	 		// init gap
	 		lane->gap = -sprite->char_count + 1;
	 		lane->target_gap = (sys_rand() % (lane->def->gap_max - lane->def->gap_min + 1)) + lane->def->gap_min;
		}
		// clear buffer
		memset(lane->buffer[0], CHAR_BUFFER_EMPTY, 132);
		// render sprites
		i = lane->sprites.idx_s;
		uint8_t y = 0;
		while (i != lane->sprites.idx_e) {
			sprite = &lane->sprites.items[i];
			debug_print_8(y++, i);
			sprite_render(sprite, lane->buffer[0]);
			if (++i == 16) { i = 0; }
		}
		lane->move_frames = lane->def->move_delay_frames;
	} else {
		lane->move_frames--;
	}
}

void lane_render(lane *lane) { // TODO: optimize rendering
	// lane->buffer_idx points to the updated buffer
	// 1 - lane->buffer_idx points to the old buffer
	uint8_t buffer[62];
	buffer[61] = 0;
	memcpy(buffer, lane->buffer[lane->buffer_idx] + 35, 61);
	avdc_write_str_at_pointer_pos(lane->def->y, 35, buffer, NULL);
}	

int main() {
	//avdc_init_ex(AVDC_MODE_CUSTOM, 0xC4, avdc_init_str);
	avdc_init_ex(AVDC_MODE_132, 0, NULL);
	// for (uint8_t i = 0; i < 62; i++) {
	// 	avdc_define_glyph(i, glyphs[i]);
	// }

	srand(timer());

	lane lane_0;
	lane_init(&lane_0, &lane_def_0_0);
	lane_render(&lane_0);
	
	do {
		lane_update(&lane_0);
		lane_render(&lane_0);
		//msleep(1000);
		//avdc_clear_screen();
	} while (!kbhit());

	avdc_done();
	return 0;
}