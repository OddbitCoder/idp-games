/*
 * trav.c
 *
 * Miha Grčar 2022
 *
 * Travel table "pointer"
 */

#include <string.h>
#include "utils.h"
#include "io.h"
#include "trav.h"

struct trav_ptr *trav_start(struct trav_ptr *t_ptr, UINT8 idx) {
	memcpy(t_ptr->buffer, travel[idx].seekadr, travel[idx].txtlen + 1);
	//__fread(TRAV_BIN, t_ptr->buffer, (UINT16)travel[idx].seekadr, travel[idx].txtlen);
	t_ptr->ptr = t_ptr->buffer;
	t_ptr->eod = t_ptr->ptr + travel[idx].txtlen;
	t_ptr->tverb = *t_ptr->ptr & 127;
	t_ptr->ptr++;
	t_ptr->tloc = AS_UINT16(t_ptr->ptr);
	t_ptr->ptr += 2;
	t_ptr->conditions = AS_UINT16(t_ptr->ptr);
	t_ptr->ptr += 2;
	return t_ptr;
}

struct trav_ptr *trav_next(struct trav_ptr *t_ptr) {
	if (trav_last(t_ptr)) {
		t_ptr->ptr++;
		return 0;
	}
	t_ptr->tverb = *t_ptr->ptr;
	t_ptr->ptr++;
	if ((t_ptr->tverb & 128) != 0) {
		t_ptr->tloc = AS_UINT16(t_ptr->ptr);
		t_ptr->ptr += 2;
		t_ptr->conditions = AS_UINT16(t_ptr->ptr);
		t_ptr->ptr += 2;
	}	
	t_ptr->tverb &= 127;
	return t_ptr;
}

struct trav_ptr *trav_set(struct trav_ptr *a, struct trav_ptr *b) {
	if (b == 0) {
		a->ptr = 0;
		a->eod = 0;
	} else {
		a->tverb = b->tverb;
		a->tloc = b->tloc;
		a->conditions = b->conditions;
		a->ptr = b->ptr;
		a->eod = b->eod;
		memcpy(a->buffer, b->buffer, TT_BUFFER_SIZE);
	}
	return a;
}

BOOL trav_last(struct trav_ptr *t_ptr) {
	return t_ptr->ptr == (t_ptr->eod - 1);
}

BOOL trav_valid(struct trav_ptr *t_ptr) {
	return t_ptr->ptr < t_ptr->eod;
}

