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
#define VISIBLE_ROW_LEN 61

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
	uint8_t *left;
	uint8_t *body;
	uint8_t *right;
} sprite_chars;

typedef struct {
	sprite_chars *chars;
	uint8_t body_size;
	uint8_t gap; // gap to the left of this sprite
	// set by engine
	uint8_t cache[64]; // for faster rendering (TODO)
} sprite;

typedef struct {
	sprite **sprites;
	uint8_t sprite_count;
	uint8_t delay_multiplier;
} anim_frame;

typedef struct {
	sprite **sprites;
	uint8_t sprite_count;
	anim_frame **anim_frames;
	uint8_t anim_frame_count;
	// set by engine
	uint16_t addr_start; 
	uint16_t addr_end;
	uint16_t addr;
} row;

typedef struct {
	row **rows;
	uint8_t row_count;
	// set by engine
	uint8_t anim_frame_idx;
} lane;

typedef struct {
	lane **lanes;
	uint8_t lane_count;
} level;

sprite_chars sprite_chars_log = {
	"\x01""A",
	"\x01""B",
	"\x03""CDE"
};

sprite_chars sprite_chars_turtle_0 = {
	"\x04""FGHI",
	"\x00""",
	"\x00"""
};

sprite_chars sprite_chars_turtle_1 = {
	"\x04""JKLM",
	"\x00""",
	"\x00"""
};

sprite_chars sprite_chars_turtle_dive = {
	"\x04""NOPQ",
	"\x00""",
	"\x00"""
};

sprite sprite_log_9_18 = {
	&sprite_chars_log,
	/*body_size*/ 18 - 4,
	/*gap*/ 9
};

sprite sprite_log_6_18 = {
	&sprite_chars_log,
	/*body_size*/ 18 - 4,
	/*gap*/ 6
};

sprite sprite_log_11_27 = {
	&sprite_chars_log,
	/*body_size*/ 27 - 4,
	/*gap*/ 11
};

sprite sprite_log_11_13 = {
	&sprite_chars_log,
	/*body_size*/ 13 - 4,
	/*gap*/ 11
};

sprite sprite_log_16_13 = {
	&sprite_chars_log,
	/*body_size*/ 13 - 4,
	/*gap*/ 16
};

sprite sprite_turtle_0_18 = {
	&sprite_chars_turtle_0,
	/*body_size*/ 0,
	/*gap*/ 18
};

sprite sprite_turtle_0_7 = {
	&sprite_chars_turtle_0,
	/*body_size*/ 0,
	/*gap*/ 7
};

sprite sprite_turtle_0_1 = {
	&sprite_chars_turtle_0,
	/*body_size*/ 0,
	/*gap*/ 1
};

sprite sprite_turtle_1_7 = {
	&sprite_chars_turtle_1,
	/*body_size*/ 0,
	/*gap*/ 7
};

sprite sprite_turtle_1_1 = {
	&sprite_chars_turtle_1,
	/*body_size*/ 0,
	/*gap*/ 1
};

// aux_sprites_<level>_<lane>_<row> 

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

sprite *aux_sprites_0_2_0[] = {
	&sprite_log_11_27
};

sprite *aux_sprites_0_3_0[] = {
	&sprite_log_16_13,
	&sprite_log_11_13,
	&sprite_log_11_13
};

// row_<level>_<lane>_<row>

row row_0_0_0 = {
	aux_sprites_0_0_0,
	/*sprite_count*/ 3,
	NULL, 0 // no animation
};

row row_0_1_0 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_1 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_2 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_3 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_4 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_5 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_1_6 = {
	aux_sprites_0_1_0,
	/*sprite_count*/ 8,
	NULL, 0 // no animation
};

row row_0_2_0 = {
	aux_sprites_0_2_0,
	/*sprite_count*/ 1,
	NULL, 0 // no animation
};

row row_0_3_0 = {
	aux_sprites_0_3_0,
	/*sprite_count*/ 3,
	NULL, 0 // no animation
};

row *aux_lane_rows_0_0[] = {
	&row_0_0_0
};

row *aux_lane_rows_0_1[] = {
	&row_0_1_0,
	&row_0_1_1,
	&row_0_1_2,
	&row_0_1_3,
	&row_0_1_4,
	&row_0_1_5,
	&row_0_1_6
};

row *aux_lane_rows_0_2[] = {
	&row_0_2_0
};

lane lane_0_0 = {
	aux_lane_rows_0_0,
	/*row_count*/ 1
};

lane lane_0_1 = {
	aux_lane_rows_0_1,
	/*row_count*/ 7
};

lane lane_0_2 = {
	aux_lane_rows_0_2,
	/*row_count*/ 1
};

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

uint8_t sprite_get_len(sprite *sprite) {
	uint8_t sz = *sprite->chars->left;
	sz += *sprite->chars->body * sprite->body_size;
	sz += *sprite->chars->right;
	return sz;
}

uint8_t row_get_len(row *row) {
	uint8_t sz = 0;
	for (uint8_t i = 0; i < row->sprite_count; i++) {
		sprite *sprite = row->sprites[i];
		sz += sprite->gap;
		sz += sprite_get_len(sprite);
	}
	return sz;
}

void sprite_render(sprite *sprite, uint16_t addr) {
	avdc_set_cursor_addr(addr);
	avdc_write_str_at_cursor(sprite->chars->left + 1, NULL);
	for (uint8_t i = 0; i < sprite->body_size; i++) {
		avdc_write_str_at_cursor(sprite->chars->body + 1, NULL);
	}
	avdc_write_str_at_cursor(sprite->chars->right + 1, NULL);
}

void sprite_render_buffer(sprite *sprite, uint8_t *buffer) {
	memcpy(buffer, sprite->chars->left + 1, *sprite->chars->left);
	buffer += *sprite->chars->left;
	for (uint8_t i = 0; i < sprite->body_size; i++) {
	memcpy(buffer, sprite->chars->body + 1, *sprite->chars->body);
		buffer += *sprite->chars->body;
	}
	memcpy(buffer, sprite->chars->right + 1, *sprite->chars->right);
}

void row_render_buffer(row *row, uint8_t len_min, uint8_t *buffer) {
	uint8_t len = 0;
	memset(buffer, '.', len_min);
	while (true) {
		for (uint8_t i = 0; i < row->sprite_count; i++) {
			sprite *sprite = row->sprites[i];
			buffer += sprite->gap;
			len += sprite->gap;
			sprite_render_buffer(sprite, buffer);
			uint8_t sprite_len = sprite_get_len(sprite);
			buffer += sprite_len;
			len += sprite_len;
			if (len >= len_min) { return; }
		}
	}	
}

void row_render(row *row) {
	uint16_t addr = row->addr_start;
	for (uint8_t i = 0; i < row->sprite_count; i++) {
		sprite *sprite = row->sprites[i];
		addr += sprite->gap;
		sprite_render(sprite, addr);
		addr += sprite_get_len(sprite);
	}
	// end with the first VISIBLE_ROW_LEN chars
	row_render_buffer(row, VISIBLE_ROW_LEN, buffer);
	buffer[VISIBLE_ROW_LEN] = 0;
	avdc_set_cursor_addr(addr);
	avdc_write_str_at_cursor(buffer, NULL);
}

void level_init(level *level) {
	uint16_t addr = ADDR_BASE;
	for (uint8_t i = 0; i < level->lane_count; i++) {
		lane *lane = level->lanes[i];
		for (uint8_t j = 0; j < lane->row_count; j++) {
			row *row = lane->rows[j];
			// assign memory address to row
			row->addr_start = row->addr = addr;
			row->addr_end = addr + row_get_len(row);
			addr = row->addr_end + VISIBLE_ROW_LEN;
			// render row
			row_render(row);
		}
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
		avdc_write_addr_at_cursor(lane->rows[0]->addr);
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

void row_shift_left(row *row, uint8_t step) {
	row->addr += step;
	if (row->addr >= row->addr_end) {
		uint8_t diff = row->addr_end - row->addr;
		row->addr = row->addr_start + diff;
	}
}

void row_shift_right(row *row, uint8_t step) {
	row->addr -= step;
	if (row->addr < row->addr_start) {
		uint8_t diff = row->addr_start - row->addr; 
		row->addr = row->addr_end - diff; 
	}
}

void lane_shift_left(lane *lane, uint8_t step) {
	for (uint8_t j = 0; j < lane->row_count; j++) {
		row *row = lane->rows[j];
		row_shift_left(row, step);
	}
}

void lane_shift_right(lane *lane, uint8_t step) {
	for (uint8_t j = 0; j < lane->row_count; j++) {
		row *row = lane->rows[j];
		row_shift_right(row, step);
	}
}

int main() {
	//avdc_init();
	//avdc_init_ex(AVDC_MODE_CUSTOM, 0xC4, avdc_init_str);
	avdc_init_ex(AVDC_MODE_132, 0, NULL);

	srand(timer());

	level_init(&level_0);
	row_table_init(&level_0);

	//debug_print_u16(20, level_get_byte_count(&level_0));

	//debug_print_u8(0, row_get_len(&row_0_0_0) + VISIBLE_ROW_LEN);
	//debug_print_u8(1, row_get_len(&row_0_1_0) + VISIBLE_ROW_LEN);
	//debug_print_u8(2, row_get_len(&row_0_2_0) + VISIBLE_ROW_LEN);
	//debug_print_u8(3, row_get_len(&row_0_3_0) + VISIBLE_ROW_LEN);

	do {
		lane_shift_left(&lane_0_0, 1);
		lane_shift_right(&lane_0_1, 1);
		lane_shift_left(&lane_0_2, 1);
		row_table_init(&level_0);
		msleep(100);
	} while (!kbhit());

	avdc_done();
	return 0;
}