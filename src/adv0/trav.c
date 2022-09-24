#include <stdio.h>
#include "trav.h"

struct travptr *tstart(struct travptr *tptr, char idx) {
	tptr->ptr = newtravel[idx];
	tptr->eod = tptr->ptr + *tptr->ptr;
	tptr->ptr++;
	tptr->tverb = *tptr->ptr & 127;
	tptr->ptr++;
	tptr->tloc = *(uint *)tptr->ptr;
	tptr->ptr += 2;
	tptr->conditions = *(uint *)tptr->ptr;
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
		tptr->tloc = *(uint *)tptr->ptr;
		tptr->ptr += 2;
		tptr->conditions = *(uint *)tptr->ptr;
		tptr->ptr += 2;
	}	
	tptr->tverb &= 127;
	return tptr;
}

bool tlast(struct travptr *tptr) {
	return tptr->ptr == tptr->eod;
}

bool tvalid(struct travptr *tptr) {
	return tptr->ptr <= tptr->eod;
}

void wtptr(struct travptr *tptr) {
	printf("%d (%d %d) ", tptr->tverb, tptr->tloc, tptr->conditions);
}