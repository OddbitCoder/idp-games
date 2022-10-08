/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Various utils for Adventure
 */

#include <bdos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "vocab.h"

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

__sfr __at 0xA0 CTC_TENTHS_CS;
__sfr __at 0xA1 CTC_HUNDREDS;
__sfr __at 0xA2 CTC_SECONDS;

fcb_t fcb;
UINT8 dma[DMA_SIZE];

int fparse(char *path, fcb_t *fcb, UINT8 *area);

char *strcpy(char *destination, const char *source)
{
	while (*source != '\0') {
		*destination = *source;
		destination++;
		source++;
	}
	*destination = '\0';
	return destination;
}

void exit(int status)
{
    status;
    // unfortunately, the status is lost in CP/M
    bdos(P_TERMCPM, 0);
}

UINT16 atoi(char *str)
{
    UINT16 res = 0;
    for (UINT8 i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

void set_fname(char *in_str) {
    int len = length(in_str);
    memcpy(fname, in_str, len > 8 ? 8 : len);
    fname[8] = '\0';
    to_upper(fname);
    char *p = fname;
    for (; *p; p++);
    memcpy(p, ".SAV", 5);
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
    BOOL file_open = FALSE;
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

BOOL fwrite(char *path, UINT8 *buf, UINT16 len) {
    BOOL ret_val = FALSE;
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
    BOOL file_open = FALSE;
    // open or create file 
    bdosret(F_OPEN, (UINT16)&fcb, &result);
    if (result.reta == BDOS_FAILURE) { 
        bdosret(F_MAKE, (UINT16)&fcb, &result);
        if (result.reta == BDOS_FAILURE) {
            goto fwrite_done;
        }
    }
    file_open = TRUE; // file is open
    // set DMA to our block
    bdos(F_DMAOFF, (UINT16)dma);
    // write
    UINT16 w_len = 0;
    UINT8 *p_buf = buf;
    while (w_len < len) {
        // set DMA
        UINT16 sz = (len - w_len) > DMA_SIZE ? DMA_SIZE : (len - w_len);
        memcpy(dma, p_buf, sz);
        // write
        bdosret(F_WRITE, (UINT16)&fcb, &result);
        if (result.reta != 0) {
            goto fwrite_done;
        }
        p_buf += sz;
        w_len += sz;
    }
    ret_val = TRUE;
fwrite_done:
    if (file_open) {
        bdosret(F_CLOSE, (UINT16)&fcb, &result);
    }
    if (area != prev_area) {
        bdos(F_USERNUM, prev_area);
    }
    return ret_val;
}

void to_lower(char *str) {
    for (; *str; str++) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 'a' - 'A';
        }
    }
}

void to_upper(char *str) {
    for (; *str; str++) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 'a' - 'A';
        }
    }
}

void con_in() {
    printf("? ");
    char ch;
    UINT8 len = 0;
    do {
        // TODO: history, back/fwd arrow, insert, delete...
        while (!(ch = kbhit()));
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (len < 2 * MAXSTR + 1) {
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

void parse_in(char *w1_buf, char *w2_buf, int w1_max_len, int w2_max_len) {
    // word 1
    char *p_buf = buffer, *w_start;
    for (; *p_buf == ' '; p_buf++);
    w_start = p_buf;
    UINT8 len = 0;
    for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
    if (len > w1_max_len) {
        memcpy(w1_buf, w_start, w1_max_len);
        w1_buf[w1_max_len] = '\0';
    } else {
        memcpy(w1_buf, w_start, len);
        w1_buf[len] = '\0';
    }
    to_lower(w1_buf);
    // word 2
    w2_buf[0] = '\0';
    if (w2_max_len > 0) {
        for (; *p_buf == ' '; p_buf++); 
        if (*p_buf != '\0') { // is there a word?
            w_start = p_buf;
            len = 0;
            for (; *p_buf != ' ' && *p_buf != '\0'; p_buf++, len++);
            if (len > w2_max_len) {
                memcpy(w2_buf, w_start, w2_max_len);
                w2_buf[w2_max_len] = '\0';
            } else {
                memcpy(w2_buf, w_start, len);
                w2_buf[len] = '\0';
            }
            to_lower(w2_buf);
        }
    }
}

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