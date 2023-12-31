#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "frogger.h"
#include "../common/avdc.h"
#include "../common/utils.h"
#include "../common/gdp.h"

#define ADDR_BASE (26 * 2)
#define VISIBLE_ROW_LEN 132

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
	LANE_UPDATE_STATIC = 0,
	LANE_UPDATE_SCROLL = 1,
	LANE_UPDATE_AUTO = 2,
	LANE_UPDATE_DEFAULT = LANE_UPDATE_SCROLL | LANE_UPDATE_AUTO
} lane_update_config;

typedef enum {
	DIR_LEFT,
	DIR_RIGHT
} dir;

typedef struct {
	uint8_t *chars;
	uint8_t len;
} sprite;

typedef struct {
	uint8_t *frame_def;
	uint8_t frame_def_len;
} frame;

typedef struct {
	uint8_t *row_def;
	uint8_t row_def_len;
	frame **frames;
	// set by the engine
	uint16_t addr; 
} row;

typedef struct {
	row **rows; // WARNME: each row in a lane needs to be of the same size
	uint8_t row_count;
	uint8_t frame_count;
	lane_update_config update_config;
	uint8_t bkgr_char;
	dir dir;
	uint8_t shift_row_delay_multiplier;
	uint8_t switch_row_delay_multiplier;
	uint8_t render_frame_delay_multiplier;
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

sprite sprite_log_18 = {
	"ABBBBBBBBBBBBBBCDE",
	/*len*/ 18
};

sprite sprite_log_27 = {
	"ABBBBBBBBBBBBBBBBBBBBBBBCDE",
	/*len*/ 27
};

sprite sprite_log_13 = {
	"ABBBBBBBBBCDE",
	/*len*/ 13
};

sprite sprite_turtle_0 = {
	"FGHI",
	/*len*/ 4
};

sprite sprite_turtle_1 = {
	"JKLM",
	/*len*/ 4
};

sprite sprite_turtle_dive = {
	"fghi",
	/*len*/ 4
};

sprite sprite_turtle_dive_more = {
	"jklm",
	/*len*/ 4
};

sprite sprite_turtle_submerged = {
	"    ",
	/*len*/ 4
};

sprite sprite_truck = {
	"NOPQQQRS",
	/*len*/ 8	
};

sprite sprite_f1_left = {
	"TUVW",
	/*len*/ 4
};

sprite sprite_f1_right = {
	"tuvw",
	/*len*/ 4
};

sprite sprite_car = {
	"XYZ0",
	/*len*/ 4
};

sprite sprite_dozer = {
	"1234",
	/*len*/ 4
};

sprite sprite_home = {
	"     ",
	/*len*/ 5
};

sprite sprite_dummy = {
	"",
	/*len*/ 0
};

sprite *sprite_list[] = {
	&sprite_log_18,
	&sprite_log_27,
	&sprite_log_13,
	&sprite_turtle_0,
	&sprite_turtle_1,
	&sprite_turtle_dive,
	&sprite_turtle_dive_more,
	&sprite_turtle_submerged,
	&sprite_truck,
	&sprite_f1_left,
	&sprite_f1_right,
	&sprite_car,
	&sprite_dozer,
	&sprite_home,
	&sprite_dummy
};

#define SPRITE_LOG_18 "\x00"
#define SPRITE_LOG_27 "\x01"
#define SPRITE_LOG_13 "\x02"
#define SPRITE_TURTLE_0 "\x03"
#define SPRITE_TURTLE_1 "\x04"
#define SPRITE_TURTLE_DIVE "\x05"
#define SPRITE_TURTLE_DIVE_MORE "\x06"
#define SPRITE_TURTLE_SUBMERGED "\x07"
#define SPRITE_TRUCK "\x08"
#define SPRITE_F1_LEFT "\x09"
#define SPRITE_F1_RIGHT "\x0A"
#define SPRITE_CAR "\x0B"
#define SPRITE_DOZER "\x0C"
#define SPRITE_HOME "\x0D"
#define SPRITE_DUMMY "\x0E"

// *** frame_<level>_<lane>_<row>_<frame>

frame frame_0_1_0_0 = { // "reset frame"
	"\x12"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x30"SPRITE_DUMMY,
	/*frame_def_len*/ 3 * 2
};

frame frame_0_1_1_0 = { // "reset frame"
	"\x12"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x30"SPRITE_DUMMY,
	/*frame_def_len*/ 3 * 2
};

// these are row-independent

frame frame_0_1_1 = {
	"\x12"SPRITE_TURTLE_DIVE"\x01"SPRITE_TURTLE_DIVE"\x30"SPRITE_DUMMY,
	/*frame_def_len*/ 3 * 2
};

frame frame_0_1_2 = {
	"\x12"SPRITE_TURTLE_DIVE_MORE"\x01"SPRITE_TURTLE_DIVE_MORE"\x30"SPRITE_DUMMY,
	/*frame_def_len*/ 3 * 2
};

frame frame_0_1_3 = {
	"\x12"SPRITE_TURTLE_SUBMERGED"\x01"SPRITE_TURTLE_SUBMERGED"\x30"SPRITE_DUMMY,
	/*frame_def_len*/ 3 * 2
};

// lane 4 frames

frame frame_0_4_0_0 = { // "reset frame"
	"\x05"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x39"SPRITE_DUMMY,
	/*frame_def_len*/ 4 * 2
};

frame frame_0_4_1_0 = { // "reset frame"
	"\x05"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x39"SPRITE_DUMMY,
	/*frame_def_len*/ 4 * 2
};

// these are row-independent

frame frame_0_4_1 = {
	"\x05"SPRITE_TURTLE_DIVE"\x01"SPRITE_TURTLE_DIVE"\x01"SPRITE_TURTLE_DIVE"\x39"SPRITE_DUMMY,
	/*frame_def_len*/ 4 * 2
};

frame frame_0_4_2 = {
	"\x05"SPRITE_TURTLE_DIVE_MORE"\x01"SPRITE_TURTLE_DIVE_MORE"\x01"SPRITE_TURTLE_DIVE_MORE"\x39"SPRITE_DUMMY,
	/*frame_def_len*/ 4 * 2
};

frame frame_0_4_3 = {
	"\x05"SPRITE_TURTLE_SUBMERGED"\x01"SPRITE_TURTLE_SUBMERGED"\x01"SPRITE_TURTLE_SUBMERGED"\x39"SPRITE_DUMMY,
	/*frame_def_len*/ 4 * 2
};

// *** row_<level>_<lane>_<row>

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

frame *aux_row_0_4_0_frames[] = {
	&frame_0_4_0_0,
	&frame_0_4_1,
	&frame_0_4_2,
	&frame_0_4_3,
	&frame_0_4_2,
	&frame_0_4_1
};

frame *aux_row_0_4_1_frames[] = {
	&frame_0_4_1_0,
	&frame_0_4_1,
	&frame_0_4_2,
	&frame_0_4_3,
	&frame_0_4_2,
	&frame_0_4_1
};

row row_0_0_0 = {
	"\x06"SPRITE_LOG_18"\x09"SPRITE_LOG_18"\x09"SPRITE_LOG_18, 
	/*row_def_len*/ 3 * 2
};

row row_0_1_0 = {
	"\x12"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x07"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x07"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x07"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0,
	/*row_def_len*/ 8 * 2,
	aux_row_0_1_0_frames
};

row row_0_1_1 = {
	"\x12"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x07"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x07"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x07"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1,
	/*row_def_len*/ 8 * 2,
	aux_row_0_1_1_frames
};

row row_0_2_0 = {
	"\x0B"SPRITE_LOG_27,
	/*sprite_count*/ 1 * 2
};

row row_0_3_0 = {
	"\x10"SPRITE_LOG_13"\x0B"SPRITE_LOG_13"\x0B"SPRITE_LOG_13, 
	/*row_def_len*/ 3 * 2
};

row row_0_4_0 = {
	"\x05"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x05"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x05"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x05"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0"\x01"SPRITE_TURTLE_0,
	/*row_def_len*/ 12 * 2,
	aux_row_0_4_0_frames
};

row row_0_4_1 = {
	"\x05"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x05"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x05"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x05"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1"\x01"SPRITE_TURTLE_1,
	/*row_def_len*/ 12 * 2,
	aux_row_0_4_1_frames
};

row row_0_5_0 = {
	"\x2A"SPRITE_TRUCK"\x12"SPRITE_TRUCK,
	/*row_def_len*/ 2 * 2
};

row row_0_6_0 = {
	"\x46"SPRITE_F1_RIGHT,
	/*row_def_len*/ 1 * 2
};

row row_0_7_0 = {
	"\x22"SPRITE_CAR"\x0F"SPRITE_CAR"\x0F"SPRITE_CAR,
	/*row_def_len*/ 3 * 2
};

row row_0_8_0 = {
	"\x1D"SPRITE_F1_LEFT"\x11"SPRITE_F1_LEFT"\x11"SPRITE_F1_LEFT,
	/*row_def_len*/ 3 * 2
};

row row_0_9_0 = {
	"\x22"SPRITE_DOZER"\x0F"SPRITE_DOZER"\x0F"SPRITE_DOZER,
	/*row_def_len*/ 3 * 2
};

row row_grass = {
	"\x84"SPRITE_DUMMY, // WARNME: needs to be adjusted wrt VISIBLE_ROW_LEN
	/*row_def_len*/ 1 * 2	
};

row row_home = {
	"\x25"SPRITE_HOME"\x08"SPRITE_HOME"\x08"SPRITE_HOME"\x08"SPRITE_HOME"\x08"SPRITE_HOME, // WARNME: needs to be adjusted wrt VISIBLE_ROW_LEN
	/*row_def_len*/ 5 * 2	
};

// *** lane_<level>_<lane>

row *aux_lane_0_0_rows[] = {
	&row_0_0_0
};

row *aux_lane_0_1_rows[] = {
	&row_0_1_0,
	&row_0_1_1
};

row *aux_lane_0_2_rows[] = {
	&row_0_2_0
};

row *aux_lane_0_3_rows[] = {
	&row_0_3_0
};

row *aux_lane_0_4_rows[] = {
	&row_0_4_0,
	&row_0_4_1
};

row *aux_lane_0_5_rows[] = {
	&row_0_5_0
};

row *aux_lane_0_6_rows[] = {
	&row_0_6_0
};

row *aux_lane_0_7_rows[] = {
	&row_0_7_0
};

row *aux_lane_0_8_rows[] = {
	&row_0_8_0
};

row *aux_lane_0_9_rows[] = {
	&row_0_9_0
};

row *aux_lane_grass_rows[] = {
	&row_grass
};

row *aux_lane_home_rows[] = {
	&row_home
};

lane lane_0_0 = {
	aux_lane_0_0_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_RIGHT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_1 = {
	aux_lane_0_1_rows,
	/*row_count*/ 2,
	/*frame_count*/ 6,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_LEFT,
	/*shift_row_delay_multiplier*/ 1,
	/*switch_row_delay_multiplier*/ 5,
	/*render_frame_delay_multiplier*/ 10
};

lane lane_0_2 = {
	aux_lane_0_2_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_RIGHT,
	/*shift_row_delay_multiplier*/ 1
	
};

lane lane_0_3 = {
	aux_lane_0_3_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_RIGHT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_4 = {
	aux_lane_0_4_rows,
	/*row_count*/ 2,
	/*frame_count*/ 6,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_LEFT,
	/*shift_row_delay_multiplier*/ 1,
	/*switch_row_delay_multiplier*/ 5,
	/*render_frame_delay_multiplier*/ 10
};

lane lane_0_5 = {
	aux_lane_0_5_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_LEFT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_6 = {
	aux_lane_0_6_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_RIGHT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_7 = {
	aux_lane_0_7_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_LEFT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_8 = {
	aux_lane_0_8_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_RIGHT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_0_9 = {
	aux_lane_0_9_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_DEFAULT,
	/*bkgr_char*/ ' ',
	/*dir*/ DIR_LEFT,
	/*shift_row_delay_multiplier*/ 1
};

lane lane_grass = {
	aux_lane_grass_rows,
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_STATIC,
	/*bkgr_char*/ '.'
};

lane lane_home = {
	aux_lane_home_rows, 
	/*row_count*/ 1,
	/*frame_count*/ 0,
	/*update_config*/ LANE_UPDATE_STATIC,
	/*bkgr_char*/ '.'
};

// *** level_<level>

lane *aux_level_0_lanes[] = {
	&lane_grass,
	&lane_home,
	&lane_0_0,
	&lane_0_1,
	&lane_0_2,
	&lane_0_3,
	&lane_0_4,
	&lane_grass,
	&lane_0_5,
	&lane_0_6,
	&lane_0_7,
	&lane_0_8,
	&lane_0_9,
	&lane_grass,
	&lane_grass
};

level level_0 = {
	aux_level_0_lanes,
	/*lane_count*/ 15
};

uint8_t buffer[256];

void debug_print_u8(uint8_t y, uint8_t val) {
	itoa(val, buffer, 10);
	avdc_write_str_at_cursor_pos(y, 0, buffer, NULL);
}

void debug_print_u16(uint8_t y, uint16_t val) {
	itoa(val, buffer, 10);
	avdc_write_str_at_cursor_pos(y, 0, buffer, NULL);
}

uint8_t row_get_len(row *row) {
	uint8_t sz = 0;
	for (uint8_t i = 0; i < row->row_def_len; i += 2) {
		sprite *sprite = sprite_list[row->row_def[i + 1]];
		sz += row->row_def[i];
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

void _row_render(uint8_t *row_def, uint8_t row_def_len, uint16_t addr, bool scrollable) {
	sprite *sprite;
	uint8_t i;	
	for (i = 0; i < row_def_len; i += 2) {
		sprite = sprite_list[row_def[i + 1]];
		addr += row_def[i];
		sprite_render(sprite, addr);
		addr += sprite->len;
	}
	if (scrollable) {
		// repeat the first VISIBLE_ROW_LEN chars
		uint8_t len = 0;
		while (true) {
			for (i = 0; i < row_def_len; i += 2) {
				sprite = sprite_list[row_def[i + 1]];
				addr += row_def[i];
				len += row_def[i];
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
}

void row_render(row *row, bool scrollable) {
	_row_render(row->row_def, row->row_def_len, row->addr, scrollable);
}

void row_fill(row *row, uint8_t ch, uint8_t row_len) {
	avdc_set_cursor_addr(row->addr);
	for (uint8_t i = 0; i < row_len; i++) {
		avdc_write_at_cursor(ch, /*attr*/0);
	}
}

void row_render_frame(row *row, uint8_t frame_idx, bool scrollable) {
	_row_render(row->frames[frame_idx]->frame_def, row->frames[frame_idx]->frame_def_len, row->addr, scrollable);
}

void lane_render_frame(lane *lane) {
	for (uint8_t i = 0; i < lane->row_count; i++) {
		row_render_frame(lane->rows[i], lane->frame_idx, /*scrollable*/(lane->update_config & LANE_UPDATE_SCROLL) != 0);
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
	bool scrollable = (lane->update_config & LANE_UPDATE_SCROLL) != 0;
	row *row;
	if (lane->bkgr_char != ' ') {
		for (uint8_t j = 0; j < lane->row_count; j++) {
			row = lane->rows[j];
			uint8_t row_len = lane->row_len + (scrollable ? VISIBLE_ROW_LEN : 0);
			if (row_len < VISIBLE_ROW_LEN) {
				row_len = VISIBLE_ROW_LEN;
			}
			row_fill(row, lane->bkgr_char, row_len);
		}
	}
	for (uint8_t j = 0; j < lane->row_count; j++) {
		row = lane->rows[j];
		row_render(row, scrollable);
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
	if ((lane->update_config & LANE_UPDATE_AUTO) != 0) { 
		if ((lane->update_config & LANE_UPDATE_SCROLL) != 0 && ++lane->shift_row_counter >= delay_base * lane->shift_row_delay_multiplier) {
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
				row_render_frame(lane->rows[i], lane->frame_idx, /*scrollable*/(lane->update_config & LANE_UPDATE_SCROLL) != 0);
			}
		}
	}
}

void level_update(level *level, uint8_t delay_base) {
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane_update(level->lanes[i], delay_base);
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
		level_update(&level_0, 1);
		row_table_init(&level_0);
		msleep(50);
	} while (!kbhit());

	avdc_done();
	return 0;
}