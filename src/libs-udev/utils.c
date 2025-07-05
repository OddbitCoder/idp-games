#include "utils.h"
#include "avdc.h" 
#include <bdos.h>

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

uint16_t timer() {
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

// WARNME: does this work with timer_* being unsigned?
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

// simplified standard C functions

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

void exit() {
    bdos(P_TERMCPM, 0);
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
	return rand() / 10; // WARNME: this should be removed when/if rand is fixed
}