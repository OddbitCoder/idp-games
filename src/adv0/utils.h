/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Various utils
 */

#ifndef __UTILS_H
#define __UTILS_H

#include "hdr.h"

char *strcpy(char *destination, const char *source);
void exit(int status);
int atoi(char *str);

UINT8 *fread(char *path, UINT8 *out, UINT16 pos, UINT16 len);

void conin();
void parsein(char *wd1buf, char *wd2buf, int maxwd1, int maxwd2);

#endif