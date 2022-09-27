#ifndef __IO_H
#define __IO_H

#include "hdr.h"

void rdata();
void twrite(int loq);
void rvoc();
void rspeak(int msg);
void mspeak(int msg);
void speak(struct text *msg);
int yes(int x,int y,int z);
void pspeak(int m,int skip);
void getin(char **wrd1,char **wrd2);

#endif