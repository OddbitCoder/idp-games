#ifndef __FILES_H__
#define __FILES_H__

#include <bdos.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	fcb_t fcb;
	uint8_t dma[DMA_SIZE];
} file;

void fl_init(const char *filename);
void fl_done();

uint8_t fl_current_area();
void fl_set_area(uint8_t area);
uint8_t fl_area_for(const char *filename);

bool fl_open(file *f, const char *filename, bool create);
bool fl_create(file *f, const char *filename);
void *fl_read(file *f, uint8_t *buf, uint16_t pos, uint16_t len);
bool fl_write(file *f, uint8_t *buf, uint16_t len);
void fl_close(file *f);

#endif