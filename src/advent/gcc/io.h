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

#define TEXT_BIN "TEXT.BIN"
#define VOC_BIN "VOC.BIN"
#define TRAV_BIN "TRAV.BIN"

void getin(char **wrd1,char **wrd2);
BOOL yes(int x,int y,int z);
void rspeak(int msg);
void mspeak(int msg);
void speak(struct text *msg);
void pspeak(int m,int skip);

#endif