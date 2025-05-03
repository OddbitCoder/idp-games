#include <stdlib.h>
#include "utils.h"

// timer

#define BCD2BIN(val) (((val) & 15) + ((val) >> 4) * 10)

__sfr __at 0xA0 CTC_TENTHS_CS;
__sfr __at 0xA1 CTC_HUNDREDS;
__sfr __at 0xA2 CTC_SECONDS;

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

// string

unsigned int strlen(const char *str) {
	return 0;
}

char *strchr(const char *str, unsigned int c) {
	return 0;
}

char *strcpy(char *dest, const char *src) {
	return 0;
}

int atoi(const char *str) {
	return 0;
}

int sprintf(char *str, const char *format, ...) {
	return 0;
}

char *strcat(char *dest, const char *src) {
	return 0;
}

bool isspace(int arg) {
	return 0;
}

bool ispunct(int arg) {
	return 0;
}

int strcmp(const char *str1, const char *str2) {
    return 0;
}

// conio

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

// agiparse

int results[8];
char *dict = NULL;
int dictSize = 0;

void loadDict()
{
}

void parse(char* input)
{
}
