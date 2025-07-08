#include "txtadv.h"
#include "utils.h"
#include "avdc.h"
#include <string.h>

void (*_ta_write)(const char *) 
	= _ta_write_term;
void (*_ta_put_char)(char) 
	= _ta_put_char_term;
void (*_ta_save_cursor_pos)() 
 	= _ta_save_cursor_pos_term;
void (*_ta_restore_cursor_pos)() 
 	= _ta_restore_cursor_pos_term; 	
void (*_ta_move_left)(uint8_t) 
 	= _ta_move_left_term; 	
void (*_ta_move_right)(uint8_t) 
 	= _ta_move_right_term;

uint16_t _ta_avdc_cursor_addr;
char *_ta_term_buffer = "\033[    ";

bool _ta_weq(const char *w1, const char *w2)
{   
	const char *s, *t;
	uint8_t i;
	s = w1;
	t = w2;
	for (i = 0; i < 5; i++)
	{       
		if (*t == 0 && *s == 0)
			return true;
		if (*s++ != *t++) 
			return false;
	}
	return true;
}

uint16_t _ta_voc_lookup(const char *word, file *f, uint16_t ht_size, uint16_t voc_offs, ta_voc_word *voc)
{
	uint16_t hc = 0;
	// compute hash code
	uint8_t i = 0;
	for (const char *p_ch = word; *p_ch && i < 5; p_ch++, i++) {
		hc <<= 2;
		hc += *p_ch;
	}
	hc &= ht_size - 1;
	if (voc[hc].len == 0) { return 0; }
	uint8_t *buffer = malloc(voc[hc].len);
	fl_read(f, buffer, (uint16_t)voc[hc].addr + voc_offs, voc[hc].len);
	uint8_t *eod = buffer + voc[hc].len;
	// parse HT entry
	for (uint8_t *ptr = buffer; ptr < eod; ptr++) {
		char *w_start = ptr;
		for (; *ptr != '\0'; ptr++);
		//printf("%s* ", w_start);
		if (_ta_weq(word, w_start)) { // check if we have a match
			uint16_t val = *(uint16_t *)(ptr + 1);
			free(buffer);
			return val;
		}
		ptr += 2; // skip value
	}
	free(buffer);
	return 0;
}

void _ta_write_term(const char *text) {
	puts(text);
}

void _ta_put_char_term(char ch) {
	putchar(ch);
}

void _ta_save_cursor_pos_term() {
	_ta_write("\0337");
}

void _ta_restore_cursor_pos_term() {
	_ta_write("\0338");
}

void _ta_move_left_term(uint8_t n) {
	itoa(n, _ta_term_buffer + 2, 10);
	strcat(_ta_term_buffer, "D");
	_ta_write(_ta_term_buffer);
}

void _ta_move_right_term(uint8_t n) {
	itoa(n, _ta_term_buffer + 2, 10);
	strcat(_ta_term_buffer, "C");
	_ta_write(_ta_term_buffer);
}

void _ta_write_avdc(const char *text) {
	avdc_write_str_at_cursor(text, NULL);
}

void _ta_put_char_avdc(char ch) {
	avdc_write_at_cursor(ch, AVDC_DEFAULT_ATTR);
}

void _ta_save_cursor_pos_avdc() {
	_ta_avdc_cursor_addr = avdc_get_cursor_addr();
}

void _ta_restore_cursor_pos_avdc() {
	avdc_set_cursor_addr(_ta_avdc_cursor_addr);
}

void _ta_move_left_avdc(uint8_t n) {
	avdc_set_cursor_addr(avdc_get_cursor_addr() - n);
}

void _ta_move_right_avdc(uint8_t n) {
	avdc_set_cursor_addr(avdc_get_cursor_addr() + n);
}

void ta_user_input(char *buffer, const char *prev, uint8_t buffer_len, bool prompt) {
    if (prompt) {
        _ta_write("? ");
    }
    uint8_t len = 0;
    uint8_t cursor = 0;
    buffer[0] = '\0';
    char ch;
    do {
        while (!(ch = kbd_get_key()));
        if (ch == (68 | 128)) { // left arrow
            if (cursor > 0) {
                _ta_move_left(1);
                cursor--;
            }
        } else if (ch == (67 | 128)) { // right arrow
            if (cursor < len) {
                _ta_move_right(1);
                cursor++;
            }
        }
        // F3 or # (take previous input)
        else if ((ch == '#' /*|| ch == (?? | 128)*/) && prev != NULL) { // TODO: F3
        	strcpy(buffer, prev);
        	uint8_t prev_len = strlen(prev);
        	if (len > prev_len) {
        		memset(&buffer[prev_len], ' ', len - prev_len);
        	}
        	_ta_move_left(cursor);
        	_ta_write(buffer);
        	if (len > prev_len) {
        		_ta_move_left(len - prev_len);
        	}
        	cursor = prev_len;
        	len = prev_len;
        	buffer[len] = '\0';
        }
        // backspace
        else if (ch == '\b') {
            if (cursor > 0) {
                memmove(&buffer[cursor - 1], &buffer[cursor], len - cursor + 1);
                cursor--;
                len--;
                _ta_move_left(1); // move back
                _ta_save_cursor_pos();
                _ta_write(&buffer[cursor]); // redraw tail and space to erase old last char
                _ta_put_char(' ');
                _ta_restore_cursor_pos();
            }
        }
		// delete
        else if (ch == 127) {
			if (cursor < len) {
				memmove(&buffer[cursor], &buffer[cursor + 1], len - cursor);
				len--;
				_ta_save_cursor_pos();
				_ta_write(&buffer[cursor]); // redraw tail from current cursor position
				_ta_put_char(' '); // erase the leftover character at the end
				_ta_restore_cursor_pos();
			}
		}
        // enter
        else if (ch == '\r' && len > 0) {
            break;
        }
        // character (insert)
        else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (len < buffer_len - 1) {
                memmove(&buffer[cursor + 1], &buffer[cursor], len - cursor + 1);
                buffer[cursor] = ch;
                _ta_save_cursor_pos();
                _ta_write(&buffer[cursor]); // print rest of buffer from insert point
                _ta_restore_cursor_pos();
                _ta_put_char(ch); // move cursor after inserted char
                cursor++;
                len++;
            }
        }
    } while (true);
    buffer[len] = '\0';
}

void ta_set_mode(ta_mode mode) {
	if (mode == TA_MODE_AVDC) {
		_ta_write = _ta_write_avdc;
		_ta_put_char = _ta_put_char_avdc;
		_ta_save_cursor_pos = _ta_save_cursor_pos_avdc;
		_ta_restore_cursor_pos = _ta_restore_cursor_pos_avdc;
		_ta_move_left = _ta_move_left_avdc;
		_ta_move_right = _ta_move_right_avdc;
	} else { // TA_MODE_TERM
		_ta_write = _ta_write_term;
		_ta_put_char = _ta_put_char_term;
		_ta_save_cursor_pos = _ta_save_cursor_pos_term;
		_ta_restore_cursor_pos = _ta_restore_cursor_pos_term;
		_ta_move_left = _ta_move_left_term;
		_ta_move_right = _ta_move_right_term;
	}
}