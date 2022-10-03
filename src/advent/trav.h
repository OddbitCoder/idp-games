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

#define TT_BUFFER_SIZE 60

struct trav_ptr {
	UINT8 tverb;
	UINT16 tloc;
	UINT16 conditions;
	UINT8 *ptr;
	UINT8 *eod;
	UINT8 buffer[TT_BUFFER_SIZE];
};

struct trav_ptr *trav_start(struct trav_ptr *t_ptr, UINT8 idx);
struct trav_ptr *trav_next(struct trav_ptr *t_ptr);
struct trav_ptr *trav_set(struct trav_ptr *a, struct trav_ptr *b); // a = b
BOOL trav_last(struct trav_ptr *t_ptr);
BOOL trav_valid(struct trav_ptr *t_ptr);

#endif