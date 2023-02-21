/*
 * io.h
 *
 * Miha Grčar 2022
 *
 * User input and console output
 */

#ifndef __IO_H
#define __IO_H

#include "hdr.h"

#define DATA_BIN "ADVENT.BIN"
#define TRAV_OFS 43336
#define VOC_OFS 46049

void getin(char **wrd1,char **wrd2);
BOOL yes(int x,int y,int z);
void rspeak(int msg);
void mspeak(int msg);
void speak(struct text *msg);
void pspeak(int m,int skip);

#endif