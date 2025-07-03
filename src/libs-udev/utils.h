#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <bdos.h>

#define LO(x) ((uint8_t *)&x)[0]
#define HI(x) ((uint8_t *)&x)[1]

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

// timer

__sfr __at 0xA0 CTC_TENTHS_CS; // R: clock, tenths of a centisecond
__sfr __at 0xA1 CTC_HUNDREDS;  // R: clock, hundreds of a second
__sfr __at 0xA2 CTC_SECONDS;   // R: clock, seconds

int16_t timer();
uint16_t timer_ms();

int16_t timer_diff(uint16_t timer_start);
int16_t timer_diff_ex(uint16_t timer_start, uint16_t timer_end);

// keyboard

#define KBD_STATUS_READY  0x04
#define KBD_CMD_BEEP      0x02
#define KBD_CMD_BEEP_LONG 0x04

__sfr __at 0xD9 KBD_STATUS; // R: status (ready)
__sfr __at 0xD8 KBD_CMD;    // W: command

void kbd_wait_ready();
void kbd_beep(bool long_beep);
char kbd_get_key();

// standard or simplified standard C functions

char *strcpy(char *dest, const char *src);
int atoi(const char *str);

// files

typedef struct {
	fcb_t fcb;
	uint8_t dma[DMA_SIZE];
} file;

void files_init(const char *filename);
void files_done();

uint8_t files_current_area();
void files_set_area(uint8_t area);
uint8_t files_area_for(const char *filename);

bool fopen(file *f, const char *filename);
void *fread(file *f, uint8_t *buf, uint16_t pos, uint16_t len);
bool fwrite(file *f, uint8_t *buf, uint16_t len);
void fclose(file *f);

// other

bool sys_is_emu();
int sys_rand();

#endif