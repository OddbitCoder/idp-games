#ifndef __IO_H
#define __IO_H

#include "hdr.h"

void rdata();
void twrite(int loq);
void rvoc();
void rspeak(int msg);
void mspeak(int msg);
void speak(struct text *msg);

#endif