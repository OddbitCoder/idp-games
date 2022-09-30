/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Adds functionality that is missing in idp_udev
 */

#include <bdos.h>
#include <stdlib.h>
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