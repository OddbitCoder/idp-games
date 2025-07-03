#include "utils.h"
#include "avdc.h" 
#include <string.h>

int fparse(char *path, fcb_t *fcb, uint8_t *area); // from io.h

// timer

uint16_t timer_ms() {
	uint8_t s = CTC_SECONDS;
	uint8_t s100 = CTC_HUNDREDS;
	uint8_t cs10 = CTC_TENTHS_CS;
	uint8_t s100_check = CTC_HUNDREDS;
	uint8_t s_check = CTC_SECONDS;
	uint16_t ms;
	if (s100 == s100_check) {
		ms = BCD2BIN(s100) * 10 + BCD2BIN(cs10 >> 4);
	} else {
		ms = BCD2BIN(s100_check) * 10;
	}
	if (s == s_check) {
		return BCD2BIN(s) * 1000 + ms;
	} else {
		return BCD2BIN(s_check) * 1000;
	}
}

int16_t timer() {
	uint8_t seconds = CTC_SECONDS;
	uint8_t hundreds = CTC_HUNDREDS;
	uint8_t seconds_check = CTC_SECONDS;
	if (seconds == seconds_check) {
		return BCD2BIN(seconds) * 100 + BCD2BIN(hundreds);
	} else {
		return BCD2BIN(seconds_check) * 100;
	}
}

int16_t timer_diff(uint16_t timer_start) {
	int16_t now = timer();
	if (now >= timer_start) { return now - timer_start; }
	return (now + 6000) - timer_start; 
}

int16_t timer_diff_ex(uint16_t timer_start, uint16_t timer_end) {
	if (timer_end >= timer_start) { return timer_end - timer_start; }
	return (timer_end + 6000) - timer_start; 
}

// keyboard

void kbd_wait_ready() {
	uint8_t status = 0;
	while ((status & KBD_STATUS_READY) == 0) {
		status = KBD_STATUS;
	}
}

void kbd_beep(bool long_beep) {
	kbd_wait_ready();
	KBD_CMD = long_beep ? KBD_CMD_BEEP_LONG : KBD_CMD_BEEP;
}

char kbd_get_key() {
	char key = kbhit();
	if (key == 27) {
		if (!(key = kbhit())) {
			key = 27;
		} else if (key == 91) {
			key = kbhit() | 128; // ANSI special key
		} else {
			key |= 128; // VT52 special key
		}
	}
	return key;
}

// standard or simplified standard C functions

char *strcpy(char *dest, const char *src) {
    char *p = dest;
    while (*p++ = *src++);
    return dest;
}

int atoi(const char *str) {
    int result = 0;
    char ch;
    uint8_t negative = 0;
    if ((ch = *str) == '-') {
        negative = 1;
        str++;
    } else if (ch == '+') {
        str++;
    }
    while (ch = *str++) {
        result = (result << 3) + (result << 1); 
        result += ch - '0';
    }
    return negative ? -result : result;
}

// files

uint8_t _files_user_area;

void files_init(const char *filename) {
	_files_user_area = files_current_area();
	files_set_area(files_area_for(filename));
}

void files_done() {
	files_set_area(_files_user_area);
}

uint8_t files_current_area() {
	return bdos(F_USERNUM, 0xFF);	
}

void files_set_area(uint8_t area) {
	bdos(F_USERNUM, area);
}

uint8_t files_area_for(const char *filename) {
	uint8_t area;
	fcb_t fcb;
    fparse((char *)filename, &fcb, &area);
    return area;	
}

bool fopen(file *f, const char *filename) {
	// reset fcb
    memset(&f->fcb, 0, sizeof(fcb_t));
    // parse filename 
    uint8_t area;
    fparse((char *)filename, &f->fcb, &area);
    // open file 
    bdos_ret_t result;
    bdosret(F_OPEN, (uint16_t)&f->fcb, &result);
    if (result.reta != BDOS_SUCCESS) { 
        bdosret(F_MAKE, (uint16_t)&f->fcb, &result);
        if (result.reta != BDOS_SUCCESS) {
            return false;
        }
    }
    return true;
}

void *fread(file *f, uint8_t *buf, uint16_t pos, uint16_t len) {
	// set DMA to our block    
    bdos(F_DMAOFF, (uint16_t)f->dma);
	bdos_ret_t result;
    // seek
    uint16_t rec = pos / DMA_SIZE;
    uint16_t dma_offs = pos % DMA_SIZE;
    f->fcb.rrec = rec;
    bdosret(F_READRAND, (uint16_t)&f->fcb, &result);
    // read
    uint16_t r_len = 0;
    uint8_t *p_buf = buf;
    while (r_len < len) {
    	bdosret(F_READ, (uint16_t)&f->fcb, &result);
        uint16_t count = DMA_SIZE - dma_offs;
        if (r_len + count > len) {
            count = len - r_len;
        }
		memcpy(p_buf, f->dma + dma_offs, count);
		p_buf += count;
        r_len += count;
		dma_offs = 0;
    }
}

bool fwrite(file *f, uint8_t *buf, uint16_t len) {
    // set DMA to our block
    bdos(F_DMAOFF, (uint16_t)f->dma);
    bdos_ret_t result;
    // write
    uint16_t w_len = 0;
    uint8_t *p_buf = buf;
    while (w_len < len) {
        // set DMA
        uint16_t sz = (len - w_len) > DMA_SIZE ? DMA_SIZE : (len - w_len);
        memcpy(f->dma, p_buf, sz);
        // write
        bdosret(F_WRITE, (uint16_t)&f->fcb, &result);
        if (result.reta != BDOS_SUCCESS) {
            return false;
        }
        p_buf += sz;
        w_len += sz;
    }
    return true;
}

void fclose(file *f) {
    bdos_ret_t result;
    bdosret(F_CLOSE, (uint16_t)&f->fcb, &result);
}

// other

bool sys_is_emu() {
	static int _idp_is_emu = -1;
	if (_idp_is_emu == -1) {
		_idp_is_emu = avdc_get_pointer(0, 0) == 0xFFFF;
	}
	return _idp_is_emu;
}

int sys_rand() {
	return rand() / 10; // WARNME: this should be removed when rand is fixed
}