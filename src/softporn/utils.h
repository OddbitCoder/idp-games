#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <bdos.h>

#define COLS 80
#define ROWS 25

#define REGULARCOLOR 0x07
#define BOLDCOLOR 0x0F
#define HEADERCOLOR 0x70

void printLine(const char *);

// timer

uint16_t timer();

// file

typedef struct {
	fcb_t fcb;
	uint8_t dma[DMA_SIZE];
} FILE;

FILE *fopen(char *path);
uint8_t *fread(FILE *f, uint8_t *buf, uint16_t pos, uint16_t len);
uint16_t fwrite(FILE *f, uint8_t *buf, uint16_t len);
void fclose(FILE *f);

// string

unsigned int strlen(const char *str);
char *strchr(const char *str, unsigned int c);
char *strcpy(char *dest, const char *src);
int atoi(const char *str);
char *strcat(char *dest, const char *src);
bool isspace(int arg);
bool ispunct(int arg);
int strcmp(const char *str1, const char *str2);
void vprintf(const char *format, va_list args);

// conio

void clearScreen();
void setPos(int x, int y);
void savePos();
void restorePos();
void setColor(int color);
void writeHeaderLine(int line, const char *leftText, const char *rightText);
void delay(int ms);
char getKey();
char getKeySilent();
char getOneOf(char key1, char key2, char key3);
char *getUserInput(char *buffer, int max);

void writeLine(const char *str);
void write(const char *str, ...);

// agiparse

void loadDict();
extern char *dict;

// misc

int getRandom(int max);

#endif 
