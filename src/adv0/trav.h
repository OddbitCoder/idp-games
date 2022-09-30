/*
 * trav.h
 *
 * Miha Grčar 2022
 *
 * Travel table "pointer"
 */

#ifndef __TRAV_H
#define __TRAV_H

#include "hdr.h"

struct travptr {
	UINT8 tverb;
	UINT16 tloc;
	UINT16 conditions;
	UINT8 *ptr;
	UINT8 *eod;
};

struct travptr *tstart(struct travptr *tptr, UINT8 idx);
struct travptr *tnext(struct travptr *tptr);
BOOL tlast(struct travptr *tptr);
BOOL tvalid(struct travptr *tptr);
void wtptr(struct travptr *tptr);
struct travptr *tset(struct travptr *a, struct travptr *b); // a = b

#endif