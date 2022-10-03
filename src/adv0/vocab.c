/*	$NetBSD: vocab.c,v 1.2 1995/03/21 12:05:13 cgd Exp $	*/

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
// static char sccsid[] = "@(#)vocab.c	8.1 (Berkeley) 5/31/93";
// #else
// static char rcsid[] = "$NetBSD: vocab.c,v 1.2 1995/03/21 12:05:13 cgd Exp $";
// #endif
// #endif /* not lint */

/*      Re-coding of advent in C: data structure routines               */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "hdr.h"
#include "vocab.h"
#include "utils.h"

//dstroy
void dstroy(int object)
//int object;
{       move(object,0);
}

//juggle
void juggle(int object)
//int object;
{       register int i,j;

	i=place[object];
	j=fixed[object];
	move(object,i);
	move(object+100,j);
}


//move
void move(int object,int where)
//int object,where;
{       register int from;

	if (object<=100)
		from=place[object];
	else
		from=fixed[object-100];
	if (from>0 && from<=300) carry(object,from);
	drop(object,where);
}


//put
int put(int object,int where,int pval)
//int object,where,pval;
{       move(object,where);
	return(-1-pval);
}

//carry
void carry(int object,int where)
//int object,where;
{       register int temp;

	if (object<=100)
	{       if (place[object]== -1) return;
		place[object] = -1;
		holdng++;
	}
	if (atloc[where]==object)
	{       atloc[where]=link[object];
		return;
	}
	for (temp=atloc[where]; link[temp]!=object; temp=link[temp]);
	link[temp]=link[object];
}


//drop
void drop(int object,int where)
//int object,where;
{	if (object>100) fixed[object-100]=where;
	else
	{       if (place[object]== -1) holdng--;
		place[object]=where;
	}
	if (where<=0) return;
	link[object]=atloc[where];
	atloc[where]=object;
}


//vocab
int vocab(char *word,int type)                  /* look up a word      */ 
//char *word;
//int type;       /* -1 for user word, >=0 for canned lookup*/
{
	//printf("%s. ", word);
	UINT16 hc = 0;
	// compute hash code
	for (char *p_ch = word; *p_ch; p_ch++) {
		hc <<= 2;
		hc += *p_ch;
	}
	hc &= HTSIZE - 1;
	fread("VOC.BIN", buffer, (UINT16)voc[hc].seekadr, voc[hc].txtlen);
	UINT8 *eod = buffer + voc[hc].txtlen;
	// parse HT entry
	for (UINT8 *ptr = buffer; ptr < eod; ptr++) {
		UINT8 *w_start = ptr;
		for (; *ptr != '\0'; ptr++);
		//printf("%s* ", w_start);
		if (weq(word, w_start)) { // check if we have a match
			UINT16 val = *(UINT16 *)(ptr + 1);
			//printf("(weq %d %d) ", type, val);
			if (type == -1) { return val; }
			if (type == val / 1000) { return val % 1000; }
		}
		ptr += 2; // skip value
	}
	return 0;
}


//copystr
void copystr(char *w1,char *w2)                          /* copy one string to another   */
//char *w1,*w2;
{       register char *s,*t;
	for (s=w1,t=w2; *s;)
		*t++ = *s++;
	*t=0;
}

//weq
int weq(char *w1,char *w2)                              /* compare words                */
//char *w1,*w2;                           /* w1 is user, w2 is system     */
{       register char *s,*t;
	register int i;
	s=w1;
	t=w2;
	for (i=0; i<5; i++)             /* compare at most 5 chars      */
	{       if (*t==0 && *s==0)
			return(TRUE);
		if (*s++ != *t++) return(FALSE);
	}
	return(TRUE);
}


//length
int length(char *str)                             /* includes 0 at end            */
{       register char *s;
	register int n;
	for (n=0,s=str; *s++;) n++;
	return(n+1);
}

//prht
// void prht()                                  /* print hash table             */
// {       register int i,j,l;
// 	char *c;
// 	struct hashtab *h;
// 	for (i=0; i<HTSIZE/10+1; i++)
// 	{       printf("%4d",i*10);
// 		for (j=0; j<10; j++)
// 		{       if (i*10+j>=HTSIZE) break;
// 			h= &voc[i*10+j];
// 			putchar(' ');
// 			if (h->val==0)
// 			{       printf("-----");
// 				continue;
// 			}
// 			for (l=0, c=h->atab; l<5; l++)
// 				if ((*c ^ '=')) putchar(*c++ ^ '=');
// 				else putchar(' ');
// 		}
// 		putchar('\n');
// 	}
// }
