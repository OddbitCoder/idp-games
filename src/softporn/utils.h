#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>

unsigned int strlen(const char *str);
char *strchr(const char *str, unsigned int c);
char *strcpy(char *dest, const char *src);
char tolower(char arg);
char toupper(char arg);
int atoi(const char *str);
int sprintf(char *str, const char *format, ...);
char *strcat(char *dest, const char *src);
bool isspace(int arg);
bool ispunct(int arg);
int strcmp(const char *str1, const char *str2);


#endif 