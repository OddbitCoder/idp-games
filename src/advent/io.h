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
void conin();
void getin(char **wrd1,char **wrd2);
void parsein(char *wd1buf, char *wd2buf, int maxwd1, int maxwd2);

#endif