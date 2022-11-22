/*	$NetBSD: save.c,v 1.2 1995/03/21 12:05:08 cgd Exp $	*/

/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * The game adventure was originally written in Fortran by Will Crowther
 * and Don Woods.  It was later translated to C and enhanced by Jim
 * Gillogly.  This code is derived from software contributed to Berkeley
 * by Jim Gillogly at The Rand Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// #ifndef lint
// #if 0
// static char sccsid[] = "@(#)save.c	8.1 (Berkeley) 5/31/93";
// #else
// static char rcsid[] = "$NetBSD: save.c,v 1.2 1995/03/21 12:05:08 cgd Exp $";
// #endif
// #endif /* not lint */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hdr.h"
#include "utils.h"
#include "vocab.h"
#include "trav.h"
#include "save.h"

#define SAVE_GAME_SIZE 1524
#define SIG_SIZE 10

char sig[] = "ADVENT_IDP";

struct savestruct
{
	void *address;
	int width;
};

struct savestruct save_array[] =
{
	{ sig,            SIG_SIZE },
	{ &abbnum,        sizeof(abbnum) },
	{ &attack,        sizeof(attack) },
	{ &blklin,        sizeof(blklin) },
	{ &bonus,         sizeof(bonus) },
	{ &chloc,         sizeof(chloc) },
	{ &chloc2,        sizeof(chloc2) },
	{ &clock1,        sizeof(clock1) },
	{ &clock2,        sizeof(clock2) },
	{ &closed,        sizeof(closed) },
	{ &closng,        sizeof(closng) },
	{ &daltlc,        sizeof(daltlc) },
	{ &demo,          sizeof(demo) },
	{ &detail,        sizeof(detail) },
	{ &dflag,         sizeof(dflag) },
	{ &dkill,         sizeof(dkill) },
	{ &dtotal,        sizeof(dtotal) },
	{ &foobar,        sizeof(foobar) },
	{ &gaveup,        sizeof(gaveup) },
	{ &holdng,        sizeof(holdng) },
	{ &iwest,         sizeof(iwest) },
	{ &k,             sizeof(k) },
	{ &k2,            sizeof(k2) },
	{ &knfloc,        sizeof(knfloc) },
	{ &kq,            sizeof(kq) },
	{ &latncy,        sizeof(latncy) },
	{ &limit,         sizeof(limit) },
	{ &lmwarn,        sizeof(lmwarn) },
	{ &loc,           sizeof(loc) },
	{ &maxdie,        sizeof(maxdie) },
	{ &mxscor,        sizeof(mxscor) },
	{ &newloc,        sizeof(newloc) },
	{ &numdie,        sizeof(numdie) },
	{ &obj,           sizeof(obj) },
	{ &oldlc2,        sizeof(oldlc2) },
	{ &oldloc,        sizeof(oldloc) },
	{ &panic,         sizeof(panic) },
	{ &saved,         sizeof(saved) },
	{ &savet,         sizeof(savet) },
	{ &scorng,        sizeof(scorng) },
	{ &spk,           sizeof(spk) },
	{ &stick,         sizeof(stick) },
	{ &tally,         sizeof(tally) },
	{ &tally2,        sizeof(tally2) },
	{ &_tkk,          sizeof(_tkk) },
	{ &turns,         sizeof(turns) },
	{ &verb,          sizeof(verb) },
	{ &wd1,           sizeof(wd1) },
	{ &wd2,           sizeof(wd2) },
	{ &wzdark,        sizeof(wzdark) },
	{ &yea,           sizeof(yea) },
	{ atloc,          sizeof(atloc) },
	{ dloc,           sizeof(dloc) },
	{ dseen,          sizeof(dseen) },
	{ fixed,          sizeof(fixed) },
	{ hinted,         sizeof(hinted) },
	{ link,           sizeof(link) },
	{ odloc,          sizeof(odloc) },
	{ place,          sizeof(place) },
	{ prop,           sizeof(prop) },
	{ tk,             sizeof(tk) },

	{ NULL,   0 }
};

int save(char *outfile)   /* Two passes on data: first to get size, */
//char *outfile;  /* second to output the data */
{
	UINT8 *p_buf = buffer;
	// write game state
	for (struct savestruct *p = save_array; p->address != NULL; p++) {
		memcpy(p_buf, p->address, p->width);
		p_buf += p->width;
	}
	/*if (!__fwrite(outfile, buffer, SAVE_GAME_SIZE)) {
		printf("Hmm.  The name \"%s\" appears to be magically blocked.\n\r", outfile);
		return 1;
	}*/
	return 0;
}

int restore(char *infile)
// char *infile;
{
	UINT8 *p_buf = buffer;
	/*if (!__fread(infile, buffer, 0, SIG_SIZE)) {
		goto restore_fail;
	}*/
	buffer[SIG_SIZE] = '\0';
	if (length(buffer) - 1 != SIG_SIZE) {
		return 2;
	}
	for (UINT8 i = 0; *p_buf; p_buf++, i++) {
		if (*p_buf != sig[i]) {
			return 2;
		}
	}
	/*if (!__fread(infile, buffer, 0, SAVE_GAME_SIZE)) {
		goto restore_fail;
	}*/
	p_buf = buffer;
	for (struct savestruct *p = save_array; p->address != NULL; p++) {
		memcpy(p->address, p_buf, p->width);
		p_buf += p->width;
	}
	return 0;
restore_fail:
	printf("Hmm.  The name \"%s\" appears to be magically blocked.\n\r", infile);
	return 1;
}
