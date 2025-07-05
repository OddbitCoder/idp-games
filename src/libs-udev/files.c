#include "files.h"
#include <string.h>

int fparse(char *path, fcb_t *fcb, uint8_t *area); // from io.h

uint8_t _fl_user_area;

void fl_init(const char *filename) {
	_fl_user_area = fl_current_area();
	if (filename) {
		fl_set_area(fl_area_for(filename));
	}
}

void fl_done() {
	fl_set_area(_fl_user_area);
}

uint8_t fl_current_area() {
	return bdos(F_USERNUM, 0xFF);	
}

void fl_set_area(uint8_t area) {
	bdos(F_USERNUM, area);
}

uint8_t fl_area_for(const char *filename) {
	uint8_t area;
	fcb_t fcb;
    fparse((char *)filename, &fcb, &area);
    return area;	
}

bool fl_open(file *f, const char *filename, bool create) {
	// reset fcb
    memset(&f->fcb, 0, sizeof(fcb_t));
    // parse filename 
    uint8_t area;
    fparse((char *)filename, &f->fcb, &area);
    // open file 
    bdos_ret_t result;
    bdosret(F_OPEN, (uint16_t)&f->fcb, &result);
    if ((result.reta != BDOS_SUCCESS) && create) { 
        bdosret(F_MAKE, (uint16_t)&f->fcb, &result);
    }
    return result.reta == BDOS_SUCCESS; 
}

bool fl_create(file *f, const char *filename) {
    // reset fcb
    memset(&f->fcb, 0, sizeof(fcb_t));
    // parse filename 
    uint8_t area;
    fparse((char *)filename, &f->fcb, &area);
    // create file 
    bdos_ret_t result;
    bdosret(F_MAKE, (uint16_t)&f->fcb, &result);
    return result.reta == BDOS_SUCCESS;
}

void *fl_read(file *f, uint8_t *buf, uint16_t pos, uint16_t len) {
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

bool fl_write(file *f, uint8_t *buf, uint16_t len) {
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

void fl_close(file *f) {
    bdos_ret_t result;
    bdosret(F_CLOSE, (uint16_t)&f->fcb, &result);
}