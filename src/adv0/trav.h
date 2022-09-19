#ifndef __TRAV_H
#define __TRAV_H

#include <stdbool.h>
#include "hdr.h"

struct travptr {
	byte tverb;
	ushort tloc;
	ushort conditions;
	char *ptr;
	char *ptr_eod;
};

struct travptr *tstart(struct travptr *tptr, byte idx);
struct travptr *tnext(struct travptr *tptr);
bool tlast(struct travptr *tptr);
bool tvalid(struct travptr *tptr);
void tptrwrite(struct travptr *tptr);

#endif