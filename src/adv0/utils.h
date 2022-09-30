/*
 * utils.c
 *
 * Miha Grčar 2022
 *
 * Adds functionality that is missing in idp_udev
 */

#ifndef __UTILS_H
#define __UTILS_H

#include "hdr.h"

char *strcpy(char *destination, const char *source);
void exit(int status);

UINT8 *fread(char *path, UINT8 *out, UINT16 pos, UINT16 len);

#endif