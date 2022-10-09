#ifndef __HDR_H
#define __HDR_H

/*	$NetBSD: hdr.h,v 1.2 1995/03/21 12:05:02 cgd Exp $	*/

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
 *
 *	@(#)hdr.h	8.1 (Berkeley) 5/31/93
 */

/*   ADVENTURE -- Jim Gillogly, Jul 1977
 * This program is a re-write of ADVENT, written in FORTRAN mostly by
 * Don Woods of SAIL.  In most places it is as nearly identical to the
 * original as possible given the language and word-size differences.
 * A few places, such as the message arrays and travel arrays were changed
 * to reflect the smaller core size and word size.  The labels of the
 * original are reflected in this version, so that the comments of the
 * fortran are still applicable here.
 *
 * The data file distributed with the fortran source is assumed to be called
 * "glorkz" in the directory where the program is first run.
 */

/* hdr.h: included by c advent files */

#pragma disable_warning 196

typedef unsigned int UINT16;
typedef signed int INT16;
typedef unsigned char UINT8;
typedef signed char INT8;
typedef INT8 BOOL;

#if DEBUG
#define L(x) x: printf(#x " "); 
#else
#define L(x) x: 
#endif

//
// int datfd;                              /* message file descriptor      */
extern int delhit;
extern int yea;
// extern char data_file[];                /* Virtual data file            */

// #define TAB     011
// #define LF      012
// #define FLUSHLINE while (getchar()!='\n')
// #define FLUSHLF   while (next()!=LF)

extern int loc,newloc,oldloc,oldlc2,wzdark,gaveup,kq,k,k2;
extern char *wd1,*wd2;                         /* the complete words           */
extern int verb,obj,spk;
extern int blklin;
extern int saved,savet,mxscor,latncy;

#define SHORT 50                        /* How short is a demo game?    */

#define MAXSTR  20                      /* max length of user's words   */

#define HTSIZE  128                     /* max number of vocab words    */
//struct hashtab                          /* hash table for vocabulary    */
//{       int val;                        /* word type &index (ktab)      */
//	char *atab;                     /* pointer to actual string     */
//} voc[HTSIZE];
extern const struct text voc[HTSIZE];

// #define SEED 1815622                    /* "Encryption" seed            */

struct text
#ifdef OLDSTUFF
{       int seekadr;                    /* DATFILE must be < 2**16      */
#endif //OLDSTUFF
{       char *seekadr;                  /* Msg start in virtual disk    */
	int txtlen;                     /* length of msg starting here  */
};

#define RTXSIZ  205
extern const struct text rtext[RTXSIZ];              /* random text messages         */

#define MAGSIZ  35
extern const struct text mtext[MAGSIZ];              /* magic messages               */

extern const int clsses;
#define CLSMAX  12
extern const struct text ctext[CLSMAX];              /* classes of adventurer        */
extern const int cval[CLSMAX];

extern const struct text ptext[101];                 /* object descriptions          */

#define LOCSIZ  141                     /* number of locations          */
extern const struct text ltext[LOCSIZ];              /* long loc description         */
extern const struct text stext[LOCSIZ];              /* short loc descriptions       */

//struct travlist                         /* direcs & conditions of travel*/
//{       struct travlist *next;          /* ptr to next list entry       */
//	int conditions;                 /* m in writeup (newloc / 1000) */
//	int tloc;                       /* n in writeup (newloc % 1000) */
//	int tverb;                      /* the verb that takes you there*/
//}; 
extern const struct text travel[LOCSIZ];
extern struct trav_ptr *tkk;
extern struct trav_ptr _tkk;
//
extern int atloc[LOCSIZ];

extern const int plac[101];                         /* initial object placement     */
extern const int fixd[101];
extern int fixed[101];              /* location fixed?              */

extern const int actspk[35];                         /* rtext msg for verb <n>       */

extern const int cond[LOCSIZ];                       /* various condition bits       */

extern int setbit[16];                  /* bit defn masks 1,2,4,...     */

extern const int hntmax;
extern const int hints[20][5];                       /* info on hints                */
extern int hinted[20],hintlc[20];

extern int place[101], prop[101],link[201];
extern int abb[LOCSIZ];

extern int maxtrs,tally,tally2;                /* treasure values              */

#define FALSE   0
#define TRUE    1

extern int keys,lamp,grate,cage,rod,rod2,steps,/* mnemonics                    */
	bird,door,pillow,snake,fissur,tablet,clam,oyster,magzin,
	dwarf,knife,food,bottle,water,oil,plant,plant2,axe,mirror,dragon,
	chasm,troll,troll2,bear,messag,vend,batter,
	nugget,coins,chest,eggs,tridnt,vase,emrald,pyram,pearl,rug,chain,
	spices,
	back,look,cave,null,entrnc,dprssn,
	enter, stream, pour,
	say,lock,throw,find,invent;

extern int chloc,chloc2,dseen[7],dloc[7],      /* dwarf stuff                  */
	odloc[7],dflag,daltlc;

extern int tk[21],stick,dtotal,attack;
extern int turns,lmwarn,iwest,knfloc,detail,   /* various flags & counters     */
	abbnum,maxdie,numdie,holdng,dkill,foobar,bonus,clock1,clock2,
	saved,closng,panic,closed,scorng;

extern int demo,newloc,limit;

// char *malloc();
char *decr(char a,char b,char c,char d,char e);
// unsigned long crc();

/* We need to get a little tricky to avoid strings */
#define DECR(a,b,c,d,e) decr((char)(a+'+'),(char)(b+'-'),(char)(c+'#'),(char)(d+'&'),(char)(e+'%'))

#define BUFFER_SIZE 1024

extern UINT8 buffer[BUFFER_SIZE];

#endif