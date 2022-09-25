#ifndef __TRAV_H
#define __TRAV_H

#include <stdbool.h>
#include "hdr.h"

struct travptr {
	char tverb;
	uint tloc;
	uint conditions;
	char *ptr;
	char *eod;
};

struct travptr *tstart(struct travptr *tptr, char idx);
struct travptr *tnext(struct travptr *tptr);
bool tlast(struct travptr *tptr);
bool tvalid(struct travptr *tptr);
void wtptr(struct travptr *tptr);
struct travptr *tset(struct travptr *a, struct travptr *b); // a = b

#endif