#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>

#include "mytypes.h"

// timer

UINT16 timer();

// string

unsigned int strlen(const char *str);
char *strchr(const char *str, unsigned int c);
char *strcpy(char *dest, const char *src);
int atoi(const char *str);
int sprintf(char *str, const char *format, ...);
char *strcat(char *dest, const char *src);
bool isspace(int arg);
bool ispunct(int arg);
int strcmp(const char *str1, const char *str2);

// conio

void setWindowSize();
void clearScreen();
void setPos(int x, int y);
void getPos(int *x, int *y);
void setColor(int color);
void writeHeader(int line, const char *leftText, const char *rightText);
void delay(int ms);
char getKey();
char getKeySilent();
char getOneOf(char key1, char key2, char key3);
char *getString(char *buffer, int max);

// agiparse

extern void loadDict();
extern char *dict;

#endif 
