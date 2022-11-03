#include <stdlib.h>
#include <bdos.h>
#include <string.h>
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

char *strchr(const char *str, UINT16 c) { // TODO: test
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
    int sign = 1;
    int i = 0;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; i++) {
        val = val * 10 + str[i] - '0';
    }
    return sign * val;
}

char *itoa(int num, char *str, int base) { // TODO: test
	int i = 0;
	bool isneg = false;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
	if (num < 0 && base == 10) {
		isneg = true;
		num = -num;
	}
	while (num != 0) {
		int rem = num % base;
		str[i++] = (rem > 9) ? (rem-10) + 'a' : rem + '0';
		num = num / base;
	}
	if (isneg) {
		str[i++] = '-';
	}
	str[i] = '\0';
	//strrev(str, i);
	i--;
	UINT8 j = 0, t;
	while (j < i) {
		// swap
		t = str[j];
		str[j] = str[i];
		str[i] = t;
		j++;
		i--;
	}
	return str;
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
	return (arg == '\t' || arg == '\n' ||
	    arg == '\v' || arg == '\f' || arg == '\r' || arg == ' ' ? 1 : 0);
}

bool ispunct(int arg) { // TODO: test
	return ((arg >= 33 && arg <= 47) || (arg >= 58 && arg <= 64) 
		|| (arg >= 91 && arg <= 96) || (arg >= 123 && arg <= 126) ? 1 : 0);
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
	strcpy(dest, "Score: ");
	char *ptr = dest + 7;
	itoa(score, ptr, 10);
	for (; *ptr; ptr++);
	strcat(ptr, "/3"); // NOTE: this means "'out of' 3"
}

void setWindowSize() {
}

void clearScreen() {
}

void setPos(int x, int y) {
}

void getPos(int *x, int *y) {
}

void setColor(int color) {
}

void writeHeader(int line, const char *leftText, const char *rightText) {
}

void delay(int ms) {
}

char getKey() {
	return 0;
}

char getKeySilent() {
	return 0;
}

char getOneOf(char key1, char key2, char key3) {
	return 0;
}

char *getString(char *buffer, int max) {
	return 0;
}