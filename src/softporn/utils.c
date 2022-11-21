#include <stdlib.h>
#include <bdos.h>
#include <string.h>
#include <ctype.h>
#include "metrics.h"
#include "utils.h"

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

__sfr __at 0xA0 CTC_TENTHS_CS;
__sfr __at 0xA1 CTC_HUNDREDS;
__sfr __at 0xA2 CTC_SECONDS;

fcb_t fcb;
UINT8 dma[DMA_SIZE];

int fparse(char *path, fcb_t *fcb, UINT8 *area);

UINT16 timer() {
    UINT8 s = CTC_SECONDS;
    UINT8 s100 = CTC_HUNDREDS;
    UINT8 cs10 = CTC_TENTHS_CS;
    UINT8 s100_check = CTC_HUNDREDS;
    UINT8 s_check = CTC_SECONDS;
    UINT16 ms;
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

UINT16 strlen(const char *str) { // TODO: test
	UINT16 len = 0;
	for (; *str; str++, len++);
	return len;
}

char *strchr(const char *str, char c) { // TODO: test
	for (; *str; str++) {
		if (*str == c) {
			return str;
		}
	}
	return NULL;
}

char *strcpy(char *dest, const char *src) { // TODO: test
	char *ptr = dest;
	for (; *src; src++, ptr++) {
		*ptr = *src;
	}
	*ptr = '\0';
	return dest;
}

// char tolower(char arg) {
// 	if (arg >= 'A' && arg <= 'Z') {
// 		return arg + ('a' - 'A');
// 	}
// 	return arg;
// }

// char toupper(char arg) {
// 	if (arg >= 'A' && arg <= 'Z') {
// 		return arg - ('a' - 'A');
// 	} 
// 	return arg;
// }

int atoi(const char *str) { // TODO: test
    int val = 0;
    INT8 sign = 1;
    UINT8 i = 0;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; i++) {
        val = val * 10 + str[i] - '0';
    }
    return sign * val;
}

char *strcat(char *dest, const char *src) { // TODO: test
	char *ptr = dest;
	for (; *ptr; ptr++);
	for (; *src; src++, ptr++) {
		*ptr = *src;
	}
	*ptr = '\0';
	return dest;
}

bool isspace(int arg) { // TODO: test
	return arg == '\t' || arg == '\n' ||
	    arg == '\v' || arg == '\f' || arg == '\r' || arg == ' ';
}

bool ispunct(int arg) { // TODO: test
	return (arg >= 33 && arg <= 47) || (arg >= 58 && arg <= 64) 
		|| (arg >= 91 && arg <= 96) || (arg >= 123 && arg <= 126);
}

int strcmp(const char *str1, const char *str2) { // TODO: test
	while (*str1) {
		if (*str1 != *str2) {
			break;
		}
		str1++;
		str2++;
	}
	return *str1 - *str2;
}

UINT8 *fread(char *path, UINT8 *buf, UINT16 pos, UINT16 len) {
	UINT8 *ret_val = NULL;
	// reset fcb
    memset(&fcb, 0, sizeof(fcb_t));
    // parse filename 
    UINT8 area;
    if (fparse(path, &fcb, &area)) {
        return ret_val;
    }
    // preserve user area, and change it
    UINT8 prev_area = bdos(F_USERNUM, 0xff);
    if (area != prev_area) { // only if different 
        bdos(F_USERNUM, area);
    } 
    // result of bdos operation 
    bdos_ret_t result;
    bool file_open = FALSE;
	// open file 
    bdosret(F_OPEN, (UINT16)&fcb, &result);
    if (result.reta == BDOS_FAILURE) { 
    	goto fread_done;
    }
    file_open = TRUE; // file is open
	// set DMA to our block
    bdos(F_DMAOFF, (UINT16)dma);
    // seek
    UINT16 rec = pos / DMA_SIZE;
    UINT16 dma_offs = pos % DMA_SIZE;
    fcb.rrec = rec;
    bdosret(F_READRAND, (UINT16)&fcb, &result);
    if (result.reta == BDOS_FAILURE) {
        goto fread_done;
    }
    // read
    UINT16 r_len = 0;
    UINT8 *p_buf = buf;
    while (r_len < len) {
    	bdosret(F_READ, (UINT16)&fcb, &result);
        if (result.reta != 0) {
        	goto fread_done;
        }
        UINT16 count = DMA_SIZE - dma_offs;
        if (r_len + count > len) {
            count = len - r_len;
        }
		memcpy(p_buf, dma + dma_offs, count);
		p_buf += count;
        r_len += count;
		dma_offs = 0;
    }
    ret_val = buf;
fread_done:
    if (file_open) {
        bdosret(F_CLOSE, (UINT16)&fcb, &result);
    }
	if (area != prev_area) {
		bdos(F_USERNUM, prev_area);
	}
	return ret_val;
}

void setScoreText(char *dest, int score) {
	// WARNME: we assume that 'score' is a single digit
	strcpy(dest, "Score: ");
	dest += 7;
	*dest = score + '0';
	dest++;
	strcpy(dest, "/3"); // "'out of' 3"
}

void setWindowSize() {
}

void clearScreen() {
	printf("\x1B[2J");
	setPos(0, 0);
}

void setPos(int x, int y) {
	printf("\x1B[%d;%dH", y, x);
}

void savePos() {
	printf("\x1B""7");
}

void restorePos() {
	printf("\x1B""8");
}

void setColor(int color) {
	switch (color) {
		case REGULARCOLOR:
			printf("\x1B[0m");
			break;
		case BOLDCOLOR:
			printf("\x1B[1m");
			break;
		case HEADERCOLOR:
			printf("\x1B[7m");
			break;
	}
}

void writeHeader(int line, const char *leftText, const char *rightText) {
	savePos();
	setPos(0, line);
	setColor(HEADERCOLOR);
	for (int i = 0; i < COLS; i++) {
		putchar(' ');
	}
	if (leftText != NULL) {
		setPos(1, line);
		puts(leftText);
	}
	if (rightText != NULL) {
		setPos(COLS - 1 - strlen(rightText), line);
		puts(rightText);
	}
	//setColor(REGULARCOLOR); // WARNME: do we need this?
	restorePos();
}

void delay(int ms) {
	// TODO
}

char getKey() {
	char ch;
	while (!(ch = toupper(kbhit())));
	putchar(ch);
	puts("");
	return ch;
}

char getKeySilent() {
	char ch;
	while (!(ch = toupper(kbhit())));
	return ch;
}

char getOneOf(char key1, char key2, char key3) {
	key1 = toupper(key1);
	key2 = toupper(key2);
	key3 = toupper(key3);
	char answer;
loop:
	while (!(answer = toupper(kbhit())));
	if (answer == key1 || answer == key2 || (key3 != 0 && answer == key3)) {
		putchar(answer);
		puts("");
		return answer;
	} else {
		//Beep(1000, 10); // TODO
		goto loop;
	}
}

char *getString(char *buffer, int max) {
    char ch;
    UINT8 len = 0;
    do {
        while (!(ch = toupper(kbhit())));
        if ((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (len < max) {
                buffer[len] = ch;
                printf("%c", ch);
                buffer[++len] = '\0'; 
            } 
        } else if (ch == '\b') { 
            if (len > 0) {
                printf("\b \b");
                buffer[--len] = '\0';
            }
        } 
    } while (ch != '\r' || len == 0);
    printf("\n\r");
}