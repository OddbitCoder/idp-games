#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>

typedef signed char INT8;
typedef unsigned char UINT8;
typedef unsigned int UINT16;

// standard utils

UINT16 timer();

UINT16 strlen(const char *str);
char *strchr(const char *str, char c);
char *strcpy(char *dest, const char *src);
//char tolower(char arg);
//char toupper(char arg);
int atoi(const char *str);
char *strcat(char *dest, const char *src);
bool isspace(int arg);
bool ispunct(int arg);
int strcmp(const char *str1, const char *str2);

// file utils

UINT8 *fread(char *path, UINT8 *buf, UINT16 pos, UINT16 len);

// console and other utils

void setScoreText(char *dest, int score);

void setWindowSize();
void clearScreen();
void setPos(int x, int y);
void savePos();
void restorePos();
void setColor(int color);
void writeHeader(int line, const char *leftText, const char *rightText);
void delay(int ms);
char getKey();
char getKeySilent();
char getOneOf(char key1, char key2, char key3);
char *getString(char *buffer, int max);

#endif 