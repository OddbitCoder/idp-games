/*
 * trav.c
 *
 * Miha Grčar 2022
 *
 * Travel table "pointer"
 */

#include <stdio.h>
#include "trav.h"
#include "utils.h"

struct travptr *tstart(struct travptr *tptr, UINT8 idx) {
	fread(TRAV_BIN, tptr->buffer, (UINT16)travel[idx].seekadr, travel[idx].txtlen);
	tptr->ptr = buffer;
	tptr->eod = tptr->ptr + travel[idx].txtlen;
	tptr->tverb = *tptr->ptr & 127;
	tptr->ptr++;
	tptr->tloc = *(UINT16 *)tptr->ptr;
	tptr->ptr += 2;
	tptr->conditions = *(UINT16 *)tptr->ptr;
	tptr->ptr += 2;
	return tptr;
}

struct travptr *tnext(struct travptr *tptr) {
	if (tlast(tptr)) {
		tptr++;
		return 0;
	}
	tptr->tverb = *tptr->ptr;
	tptr->ptr++;
	if ((tptr->tverb & 128) != 0) {
		tptr->tloc = *(UINT16 *)tptr->ptr;
		tptr->ptr += 2;
		tptr->conditions = *(UINT16 *)tptr->ptr;
		tptr->ptr += 2;
	}	
	tptr->tverb &= 127;
	return tptr;
}

struct travptr *tset(struct travptr *a, struct travptr *b) {
	if (b == 0) {
		a->ptr = (UINT8 *)1;
		a->eod = 0;
	} else {
		a->tverb = b->tverb;
		a->tloc = b->tloc;
		a->conditions = b->conditions;
		a->ptr = b->ptr;
		a->eod = b->eod;
	}
	return a;
}

BOOL tlast(struct travptr *tptr) {
	return tptr->ptr == tptr->eod;
}

BOOL tvalid(struct travptr *tptr) {
	return tptr->ptr <= tptr->eod;
}

void wtptr(struct travptr *tptr) {
	printf("%d (%d %d) ", tptr->tverb, tptr->tloc, tptr->conditions);
}