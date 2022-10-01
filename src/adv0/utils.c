/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Various utils
 */

#include <bdos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

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

int atoi(char *str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }
    return res;
}

int fparse(char *path, fcb_t *fcb, UINT8 *area);

UINT8 *fread(char *path, UINT8 *out, UINT16 pos, UINT16 len) {
	// allocate fcb 
	fcb_t *fcb = calloc(1, sizeof(fcb_t));
	UINT8 *ret_val = NULL;
	// parse filename 
    UINT8 area;
    if (fparse(path, fcb, &area)) {
        free(fcb); 
        return ret_val;
    }
    // preserve user area, and change it
    UINT8 prev_area = bdos(F_USERNUM, 0xff);
    if (area != prev_area) { // only if different 
        bdos(F_USERNUM, area);
    } 
    // allocate DMA 
    UINT8 *dma = malloc(DMA_SIZE);
    // result of bdos operation 
    bdos_ret_t result;
    UINT8 file_open = 0;
	// open file 
    bdosret(F_OPEN, (UINT16)fcb, &result);
    if (result.reta == BDOS_FAILURE) { 
    	goto fread_done;
    }
    file_open = 1; // file is open
	// set DMA to our block
    bdos(F_DMAOFF, (UINT16)dma);
    // seek
    UINT16 rec = pos / DMA_SIZE;
    UINT16 dma_offs = pos % DMA_SIZE;
    fcb->rrec = rec;
    bdosret(F_READRAND, (UINT16)fcb, &result);
    if (result.reta == BDOS_FAILURE) {
        goto fread_done;
    }
    // read
    UINT16 rlen = 0;
    UINT8 *pout = out;
    while (rlen < len) {
    	bdosret(F_READ, (UINT16)fcb, &result);
        if (result.reta != 0) {
        	goto fread_done;
        }
        UINT16 count = DMA_SIZE - dma_offs;
        if (rlen + count > len) {
            count = len - rlen;
        }
		memcpy(pout, dma + dma_offs, count);
		pout += count;
        rlen += count;
		dma_offs = 0;
    }
    ret_val = out;
fread_done:
    if (file_open) {
        bdosret(F_CLOSE, (UINT16)fcb, &result);
    }
	if (area != prev_area) {
		bdos(F_USERNUM, prev_area);
	}
    free(fcb);
    free(dma);
	return ret_val;
}

void conin() {
    printf("? ");
    char ch;
    UINT8 l = 0;
    do {
        // TODO: history, back/fwd arrow, insert, delete...
        while (!(ch = kbhit()));
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == ' ') {
            if (l < 2 * MAXSTR + 1) {
                buffer[l] = ch;
                printf("%c", ch);
                buffer[++l] = '\0'; 
            } 
        } else if (ch == '\b') { 
            if (l > 0) {
                printf("\b \b");
                buffer[--l] = '\0';
            }
        } 
    } while (ch != '\r' || l == 0);
    printf("\n\r");
}

void parsein(char *wd1buf, char *wd2buf, int maxwd1, int maxwd2) {
    // word 1
    char *p = buffer, *r;
    for (; *p == ' '; p++);
    r = p;
    UINT8 l = 0;
    for (; *p != ' ' && *p != '\0'; p++, l++);
    if (l >= maxwd1) {
        memcpy(wd1buf, r, maxwd1 - 1);
        wd1buf[maxwd1 - 1] = '\0';
    } else {
        memcpy(wd1buf, r, l);
        wd1buf[l] = '\0';
    }
    // word 2
    wd2buf[0] = '\0';
    if (maxwd2 > 0) {
        for (; *p == ' '; p++); 
        if (*p != '\0') { // is there a word?
            r = p;
            l = 0;
            for (; *p != ' ' && *p != '\0'; p++, l++);
            if (l >= maxwd2) {
                memcpy(wd2buf, r, maxwd2 - 1);
                wd2buf[maxwd2 - 1] = '\0';
            } else {
                memcpy(wd2buf, r, l);
                wd2buf[l] = '\0';
            }
        }
    }
}