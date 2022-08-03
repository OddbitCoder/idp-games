#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "frogger.h"
#include "../common/avdc.h"
#include "../common/utils.h"
#include "../common/gdp.h"

#define ADDR_BASE (17 * 2 + 60)
#define VISIBLE_ROW_LEN 132
//(61 + 20)

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
	uint8_t *chars;
	uint8_t len;
	uint8_t gap; // gap to the left of this sprite
} sprite;

typedef struct {
	sprite **sprites;
	uint8_t sprite_count;
} frame;

typedef struct {
	sprite **sprites;
	uint8_t sprite_count;
	frame **frames;
	// set by the engine
	uint16_t addr; 
} row;

typedef struct {
	row **rows; // WARNME: each row in a lane needs to be of the same size
	uint8_t row_count;
	uint8_t shift_row_delay_multiplier;
	uint8_t switch_row_delay_multiplier;
	uint8_t render_frame_delay_multiplier;
	dir dir;
	uint8_t frame_count;
	// set by the engine
	uint16_t shift_row_counter;
	uint16_t switch_row_counter;
	uint16_t render_frame_counter;
	uint8_t row_idx;
	uint8_t frame_idx;
	uint8_t row_offset;
	uint8_t row_len;
} lane;

typedef struct {
	lane **lanes;
	uint8_t lane_count;
} level;

// *** sprite

sprite sprite_log_9_18 = {
	"ABBBBBBBBBBBBBBCDE",
	/*len*/ 18,
	/*gap*/ 9
};

sprite sprite_log_6_18 = {
	"ABBBBBBBBBBBBBBCDE",
	/*len*/ 18,
	/*gap*/ 6
};

sprite sprite_log_11_27 = {
	"ABBBBBBBBBBBBBBBBBBBBBBBCDE",
	/*len*/ 27,
	/*gap*/ 11
};

sprite sprite_log_11_13 = {
	"ABBBBBBBBBCDE",
	/*len*/ 13,
	/*gap*/ 11
};

sprite sprite_log_16_13 = {
	"ABBBBBBBBBCDE",
	/*len*/ 13,
	/*gap*/ 16
};

sprite sprite_turtle_0_18 = {
	"FGHI",
	/*len*/ 4,
	/*gap*/ 18
};

sprite sprite_turtle_1_18 = {
	"JKLM",
	/*len*/ 4,
	/*gap*/ 18
};

sprite sprite_turtle_0_7 = {
	"FGHI",
	/*len*/ 4,
	/*gap*/ 7
};

sprite sprite_turtle_0_1 = {
	"FGHI",
	/*len*/ 4,
	/*gap*/ 1
};

sprite sprite_turtle_1_7 = {
	"JKLM",
	/*len*/ 4,
	/*gap*/ 7
};

sprite sprite_turtle_1_1 = {
	"JKLM",
	/*len*/ 4,
	/*gap*/ 1
};

sprite sprite_turtle_dive_18 = {
	"fghi",
	/*len*/ 4,
	/*gap*/ 18
};

sprite sprite_turtle_dive_more_18 = {
	"jklm",
	/*len*/ 4,
	/*gap*/ 18
};

sprite sprite_turtle_submerged_18 = {
	"    ",
	/*len*/ 4,
	/*gap*/ 18
};

sprite sprite_dummy_53 = {
	"",
	/*len*/ 0,
	/*gap*/ 53
};

// *** frame_<level>_<lane>_<row>_<frame>

sprite *aux_frame_0_1_0_0_sprites[] = {
	&sprite_turtle_0_18,
	&sprite_dummy_53
};

frame frame_0_1_0_0 = { // "reset frame"
	aux_frame_0_1_0_0_sprites,
	/*sprite_count*/ 2
};

sprite *aux_frame_0_1_1_0_sprites[] = {
	&sprite_turtle_1_18,
	&sprite_dummy_53
};

frame frame_0_1_1_0 = { // "reset frame"
	aux_frame_0_1_1_0_sprites,
	/*sprite_count*/ 2
};

// these are row-independent

sprite *aux_frame_0_1_1_sprites[] = {
	&sprite_turtle_dive_18,
	&sprite_dummy_53
};

frame frame_0_1_1 = {
	aux_frame_0_1_1_sprites,
	/*sprite_count*/ 2
};

sprite *aux_frame_0_1_2_sprites[] = {
	&sprite_turtle_dive_more_18,
	&sprite_dummy_53
};

frame frame_0_1_2 = {
	aux_frame_0_1_2_sprites,
	/*sprite_count*/ 2
};

sprite *aux_frame_0_1_3_sprites[] = {
	&sprite_turtle_submerged_18,
	&sprite_dummy_53
};

frame frame_0_1_3 = {
	aux_frame_0_1_3_sprites,
	/*sprite_count*/ 2
};

// *** row_<level>_<lane>_<row>

sprite *aux_sprites_0_0_0[] = {
	&sprite_log_6_18,
	&sprite_log_9_18,
	&sprite_log_9_18
};

sprite *aux_sprites_0_1_0[] = {
	&sprite_turtle_0_18,
	&sprite_turtle_0_1,
	&sprite_turtle_0_7,
	&sprite_turtle_0_1,
	&sprite_turtle_0_7,
	&sprite_turtle_0_1,
	&sprite_turtle_0_7,
	&sprite_turtle_0_1
};

sprite *aux_sprites_0_1_1[] = {
	&sprite_turtle_1_18,
	&sprite_turtle_1_1,
	&sprite_turtle_1_7,
	&sprite_turtle_1_1,
	&sprite_turtle_1_7,
	&sprite_turtle_1_1,
	&sprite_turtle_1_7,
	&sprite_turtle_1_1
};

sprite *aux_sprites_0_2_0[] = {
	&sprite_log_11_27
};

sprite *aux_sprites_0_3_0[] = {
	&sprite_log_16_13,
	&sprite_log_11_13,
	&sprite_log_11_13
};

frame *aux_row_0_1_0_frames[] = {
	&frame_0_1_0_0,
	&frame_0_1_1,
	&frame_0_1_2,
	&frame_0_1_3,
	&frame_0_1_2,
	&frame_0_1_1
};

frame *aux_row_0_1_1_frames[] = {
	&frame_0_1_1_0,
	&frame_0_1_1,
	&frame_0_1_2,
	&frame_0_1_3,
	&frame_0_1_2,
	&frame_0_1_1
};

row row_0_0_0 = {
	aux_sprites_0_0_0,
	/*sprite_count*/ 3
};

row row_0_1_0 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	aux_row_0_1_0_frames
};

row row_0_1_1 = {
	aux_sprites_0_1_1,
	/*sprite_count*/ 8,
	aux_row_0_1_1_frames
};

row row_0_2_0 = {
	aux_sprites_0_2_0,
	/*sprite_count*/ 1
};

row row_0_3_0 = {
	aux_sprites_0_3_0,
	/*sprite_count*/ 3
};

// *** lane_<level>_<lane>

row *aux_lane_rows_0_0[] = {
	&row_0_0_0
};

row *aux_lane_rows_0_1[] = {
	&row_0_1_0,
	&row_0_1_1
};

row *aux_lane_rows_0_2[] = {
	&row_0_2_0
};

lane lane_0_0 = {
	aux_lane_rows_0_0,
	/*row_count*/ 1,
	/*shift_row_delay_multiplier*/ 1,
	/*switch_row_delay_multiplier*/ 1,
	/*render_frame_delay_multiplier*/ 1,
	/*dir*/ DIR_RIGHT,
	/*frame_count*/ 0
};

lane lane_0_1 = {
	aux_lane_rows_0_1,
	/*row_count*/ 2,
	/*shift_row_delay_multiplier*/ 1,
	/*switch_row_delay_multiplier*/ 5,
	/*render_frame_delay_multiplier*/ 10,
	/*dir*/ DIR_LEFT,
	/*frame_count*/ 6
};

lane lane_0_2 = {
	aux_lane_rows_0_2,
	/*row_count*/ 1,
	/*shift_row_delay_multiplier*/ 1,
	/*switch_row_delay_multiplier*/ 1,
	/*render_frame_delay_multiplier*/ 1,
	/*dir*/ DIR_RIGHT,
	/*frame_count*/ 0
};

// *** level_<level>

lane *aux_level_lanes_0[] = {
	&lane_0_0,
	&lane_0_1,
	&lane_0_2
};

level level_0 = {
	aux_level_lanes_0,
	/*lane_count*/ 3
};

uint8_t buffer[256];

uint8_t row_get_len(row *row) {
	uint8_t sz = 0;
	for (uint8_t i = 0; i < row->sprite_count; i++) {
		sprite *sprite = row->sprites[i];
		sz += sprite->gap;
		sz += sprite->len;
	}
	return sz;
}

void sprite_render(sprite *sprite, uint16_t addr) {
	avdc_set_cursor_addr(addr);
	avdc_write_str_at_cursor(sprite->chars, NULL);
}

void sprite_render_chars(sprite *sprite, uint16_t addr, uint8_t char_count) {
	avdc_set_cursor_addr(addr);
	// TODO: speed this up (customize avdc_write_str_at_cursor)
	memcpy(buffer, sprite->chars, char_count);
	buffer[char_count] = 0;
	avdc_write_str_at_cursor(buffer, NULL);
}

void _sprites_render(sprite **sprites, uint8_t count, uint16_t addr) {
	sprite *sprite;
	for (uint8_t i = 0; i < count; i++) {
		sprite = sprites[i];
		addr += sprite->gap;
		sprite_render(sprite, addr);
		addr += sprite->len;
	}
	// repeat the first VISIBLE_ROW_LEN chars
	uint8_t len = 0;
	while (true) {
		for (uint8_t i = 0; i < count; i++) {
			sprite = sprites[i];
			addr += sprite->gap;
			len += sprite->gap;
			if (len >= VISIBLE_ROW_LEN) {
				return;
			}
			len += sprite->len;
			if (len >= VISIBLE_ROW_LEN) {
				sprite_render_chars(sprite, addr, sprite->len - (len - VISIBLE_ROW_LEN));
				return;
			}
			sprite_render(sprite, addr);
			addr += sprite->len;
		}
	}
}

void row_render(row *row) {
	_sprites_render(row->sprites, row->sprite_count, row->addr);
}

void row_render_frame(row *row, uint8_t frame_idx) {
	_sprites_render(row->frames[frame_idx]->sprites, row->frames[frame_idx]->sprite_count, row->addr);
}

void lane_render_frame(lane *lane) {
	for (uint8_t i = 0; i < lane->row_count; i++) {
		row_render_frame(lane->rows[i], lane->frame_idx);
	}
}

void lane_init(lane *lane) {
	lane->shift_row_counter = 0;
	lane->switch_row_counter = 0;
	lane->render_frame_counter = 0;
	lane->row_idx = 0;
	lane->frame_idx = 0;
	lane->row_offset = 0;
	lane->row_len = row_get_len(lane->rows[0]);
}

void level_init(level *level) {
	uint16_t addr = ADDR_BASE;
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane *lane = level->lanes[i];
		lane_init(lane);
		for (uint8_t j = 0; j < lane->row_count; j++) {
			row *row = lane->rows[j];
			// assign memory address to row
			row->addr = addr;
			addr += row_get_len(row) + VISIBLE_ROW_LEN;
		}
	}
}

void lane_render(lane *lane) {
	for (uint8_t j = 0; j < lane->row_count; j++) {
		row *row = lane->rows[j];
		row_render(row);
	}
}

void level_render(level *level) {
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane *lane = level->lanes[i];
		lane_render(lane);
	}	
}

uint16_t level_get_byte_count(level *level) {
	uint16_t sz = 0;
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane *lane = level->lanes[i];
		for (uint8_t j = 0; j < lane->row_count; j++) {
			row *row = lane->rows[j];
			sz += row_get_len(row) + VISIBLE_ROW_LEN;
		}
	}
	return sz;
}

void row_table_init(level *level) {
	avdc_set_cursor_addr(0);
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane *lane = level->lanes[i];
		avdc_write_addr_at_cursor(lane->rows[lane->row_idx]->addr + lane->row_offset);
	}
}

void debug_print_u8(uint8_t y, uint8_t val) {
	itoa(val, buffer, 10);
	avdc_write_str_at_cursor_pos(y, 0, buffer, NULL);
}

void debug_print_u16(uint8_t y, uint16_t val) {
	itoa(val, buffer, 10);
	avdc_write_str_at_cursor_pos(y, 0, buffer, NULL);
}

void lane_shift_left(lane *lane, uint8_t step) {
	lane->row_offset += step;
	if (lane->row_offset >= lane->row_len) {
		uint8_t diff = lane->row_offset - lane->row_len;
		lane->row_offset = diff;
	}
}

void lane_shift_right(lane *lane, uint8_t step) {
	if (step <= lane->row_offset) {
		lane->row_offset -= step;
	} else { // step > lane->row_offset
		lane->row_offset = lane->row_len - (step - lane->row_offset);
	}
}

void lane_update(lane *lane, uint8_t delay_base) {
	// this does the following:
	// - shifts rows - if lane_update is called delay_base * shift_row_delay_multiplier times
	// - switches rows - if lane_update is called delay_base * switch_row_delay_multiplier times
	// - switches frames (for each row in the lane) - if lane_update is called delay_base * render_frame_delay_multiplier times
	if (++lane->shift_row_counter >= delay_base * lane->shift_row_delay_multiplier) {
		lane->shift_row_counter = 0;
		// shift rows
		if (lane->dir == DIR_LEFT) {
			lane_shift_left(lane, 1);
		} else {
			lane_shift_right(lane, 1);
		}
		// TODO: adjustable step?
	}
	if (lane->row_count > 1 && ++lane->switch_row_counter >= delay_base * lane->switch_row_delay_multiplier) {
		lane->switch_row_counter = 0;
		// switch rows
		if (++lane->row_idx == lane->row_count) {
			lane->row_idx = 0;
		}
	}
	if (lane->frame_count > 0 && ++lane->render_frame_counter >= delay_base * lane->render_frame_delay_multiplier) {
		lane->render_frame_counter = 0;
		if (++lane->frame_idx == lane->frame_count) {
			lane->frame_idx = 0;
		}
		// render frame
		for (uint8_t i = 0; i < lane->row_count; i++) {
			row_render_frame(lane->rows[i], lane->frame_idx);
		}
	}
}

int main() {
	//avdc_init();
	//avdc_init_ex(AVDC_MODE_CUSTOM, 0xC4, avdc_init_str);
	avdc_init_ex(AVDC_MODE_132, 0, NULL);

	srand(timer());

	level_init(&level_0);
	row_table_init(&level_0);
	level_render(&level_0);

	debug_print_u16(20, level_get_byte_count(&level_0));

	do {
		lane_update(&lane_0_0, 1);
		lane_update(&lane_0_1, 1);
		lane_update(&lane_0_2, 2);
		row_table_init(&level_0);
		msleep(50);
	} while (!kbhit());

	avdc_done();
	return 0;
}