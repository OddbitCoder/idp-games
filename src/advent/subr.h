/*
 * vocab.h
 *
 * Miha Grčar 2022
 *
 * Various subroutines
 */

#ifndef __SUBR_H
#define __SUBR_H

#include "hdr.h"

BOOL toting(int objj);
BOOL here(int objj);
BOOL at(int objj);
int liq2(int pbotl);
int liq();
int liqloc(int locc);
BOOL bitset(int l,int n);
BOOL forced(int locc);
BOOL dark();
BOOL pct(int n);
int fdwarf();
int march();
int mback();
int specials();
int trbridge();
int badmove();
void bug(int n);
void checkhints();
int trsay();
int trtake();
int dropper();
int trdrop();
int tropen();
int trkill();
int trtoss();
int trfeed();
int trfill();
int closing();
int caveclose();

#endif