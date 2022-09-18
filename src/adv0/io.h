#ifndef __IO_H
#define __IO_H

#include "hdr.h"

void rdata();
void trav(uint8_t locc, uint16_t tloc, uint16_t conditions, uint8_t tverb_len, uint8_t *tverb);
void rtrav();
void twrite(int loq);
void rvoc();
void rspeak(int msg);
void mspeak(int msg);
void speak(struct text *msg);

#endif