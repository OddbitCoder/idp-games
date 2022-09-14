/*	$NetBSD: io.c,v 1.3 1995/04/24 12:21:37 cgd Exp $	*/

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
// static char sccsid[] = "@(#)io.c	8.1 (Berkeley) 5/31/93";
// #else
// static char rcsid[] = "$NetBSD: io.c,v 1.3 1995/04/24 12:21:37 cgd Exp $";
// #endif
// #endif /* not lint */

/*      Re-coding of advent in C: file i/o and user i/o                 */

// #include "hdr.h"
#include <stdio.h>
// #include <string.h>
#include "io.h"
#include "vocab.h"


// getin(wrd1,wrd2)                        /* get command from user        */
// char **wrd1,**wrd2;                     /* no prompt, usually           */
// {       register char *s;
// 	static char wd1buf[MAXSTR],wd2buf[MAXSTR];
// 	int first, numch;

// 	*wrd1=wd1buf;                   /* return ptr to internal string*/
// 	*wrd2=wd2buf;
// 	wd2buf[0]=0;                    /* in case it isn't set here    */
// 	for (s=wd1buf, first=1, numch=0;;)
// 	{       if ((*s=getchar())>='A' && *s <='Z') *s = *s - ('A' -'a');
// 					/* convert to upper case        */
// 		switch(*s)              /* start reading from user      */
// 		{   case '\n':
// 			*s=0;
// 			return;
// 		    case ' ':
// 			if (s==wd1buf||s==wd2buf)  /* initial blank   */
// 				continue;
// 			*s=0;
// 			if (first)      /* finished 1st wd; start 2nd   */
// 			{       first=numch=0;
// 				s=wd2buf;
// 				break;
// 			}
// 			else            /* finished 2nd word            */
// 			{       FLUSHLINE;
// 				*s=0;
// 				return;
// 			}
// 		    default:
// 			if (++numch>=MAXSTR)    /* string too long      */
// 			{       printf("Give me a break!!\n");
// 				wd1buf[0]=wd2buf[0]=0;
// 				FLUSHLINE;
// 				return;
// 			}
// 			s++;
// 		}
// 	}
// }


// confirm(mesg)                           /* confirm irreversible action  */
// char *mesg;
// {       register int result;
// 	printf("%s",mesg);              /* tell him what he did         */
// 	if (getchar()=='y')             /* was his first letter a 'y'?  */
// 		result=1;
// 	else    result=0;
// 	FLUSHLINE;
// 	return(result);
// }

// yes(x,y,z)                              /* confirm with rspeak          */
// int x,y,z;
// {       register int result;
// 	register char ch;
// 	for (;;)
// 	{       rspeak(x);                     /* tell him what we want*/
// 		if ((ch=getchar())=='y')
// 			result=TRUE;
// 		else if (ch=='n') result=FALSE;
// 		FLUSHLINE;
// 		if (ch=='y'|| ch=='n') break;
// 		printf("Please answer the question.\n");
// 	}
// 	if (result==TRUE) rspeak(y);
// 	if (result==FALSE) rspeak(z);
// 	return(result);
// }

// yesm(x,y,z)                             /* confirm with mspeak          */
// int x,y,z;
// {       register int result;
// 	register char ch;
// 	for (;;)
// 	{       mspeak(x);                     /* tell him what we want*/
// 		if ((ch=getchar())=='y')
// 			result=TRUE;
// 		else if (ch=='n') result=FALSE;
// 		FLUSHLINE;
// 		if (ch=='y'|| ch=='n') break;
// 		printf("Please answer the question.\n");
// 	}
// 	if (result==TRUE) mspeak(y);
// 	if (result==FALSE) mspeak(z);
// 	return(result);
// }

// /* FILE *inbuf,*outbuf; */

// char *inptr;                            /* Pointer into virtual disk    */

// int outsw = 0;				/* putting stuff to data file?  */

// char iotape[] = "Ax3F'\003tt$8h\315qer*h\017nGKrX\207:!l";
// char *tape = iotape;			/* pointer to encryption tape   */

// next()                                  /* next virtual char, bump adr  */
// {
// 	int ch;

// 	ch=(*inptr ^ random()) & 0xFF;  /* Decrypt input data           */
// 	if (outsw)                      /* putting data in tmp file     */
// 	{   if (*tape==0) tape=iotape;  /* rewind encryption tape       */
// 	    *inptr = ch ^ *tape++;      /* re-encrypt and replace value */
// 	}
// 	inptr++;
// 	return(ch);
// }

// char breakch;                           /* tell which char ended rnum   */

void rdata()                                 /* "read" data from virtual file*/
{       
	rvoc();
//	register int sect;
// 	register char ch;

// 	inptr = data_file;              /* Pointer to virtual data file */
// 	srandom(SEED);                  /* which is lightly encrypted.  */

// 	clsses=1;
// 	for (;;)                        /* read data sections           */
// 	{       sect=next()-'0';        /* 1st digit of section number  */
// #ifdef VERBOSE
// 		printf("Section %c",sect+'0');
// #endif
// 		if ((ch=next())!=LF)    /* is there a second digit?     */
// 		{
// 			FLUSHLF;
// #ifdef VERBOSE
// 			putchar(ch);
// #endif
// 			sect=10*sect+ch-'0';
// 		}
// #ifdef VERBOSE
// 		putchar('\n');
// #endif
// 		switch(sect)
// 		{   case 0:             /* finished reading database    */
// 			return;
// 		    case 1:             /* long form descriptions       */
// 			rdesc(1);
// 			break;
// 		    case 2:             /* short form descriptions      */
// 			rdesc(2);
// 			break;
// 		    case 3:             /* travel table                 */
// 			rtrav();   break;
// 		    case 4:             /* vocabulary                   */
// 			rvoc();
// 			break;
// 		    case 5:             /* object descriptions          */
// 			rdesc(5);
// 			break;
// 		    case 6:             /* arbitrary messages           */
// 			rdesc(6);
// 			break;
// 		    case 7:             /* object locations             */
// 			rlocs();   break;
// 		    case 8:             /* action defaults              */
// 			rdflt();   break;
// 		    case 9:             /* liquid assets                */
// 			rliq();    break;
// 		    case 10:            /* class messages               */
// 			rdesc(10);
// 			break;
// 		    case 11:            /* hints                        */
// 			rhints();  break;
// 		    case 12:            /* magic messages               */
// 			rdesc(12);
// 			break;
// 		    default:
// 			printf("Invalid data section number: %d\n",sect);
// 			for (;;) putchar(next());
// 		}
// 		if (breakch!=LF)        /* routines return after "-1"   */
// 			FLUSHLF;
// 	}
}

// char nbf[12];


// rnum()                                  /* read initial location num    */
// {       register char *s;
// 	tape = iotape;                  /* restart encryption tape      */
// 	for (s=nbf,*s=0;; s++)
// 		if ((*s=next())==TAB || *s=='\n' || *s==LF)
// 			break;
// 	breakch= *s;                    /* save char for rtrav()        */
// 	*s=0;                           /* got the number as ascii      */
// 	if (nbf[0]=='-') return(-1);    /* end of data                  */
// 	return(atoi(nbf));              /* convert it to integer        */
// }

// char *seekhere;

// rdesc(sect)                             /* read description-format msgs */
// int sect;
// {       register char *s,*t;
// 	register int locc;
// 	char *seekstart, *maystart, *adrstart;
// 	char *entry;

// 	seekhere = inptr;               /* Where are we in virtual file?*/
// 	outsw=1;                        /* these msgs go into tmp file  */
// 	for (oldloc= -1, seekstart=seekhere;;)
// 	{       maystart=inptr;         /* maybe starting new entry     */
// 		if ((locc=rnum())!=oldloc && oldloc>=0  /* finished msg */
// 		    && ! (sect==5 && (locc==0 || locc>=100)))/* unless sect 5*/
// 		{       switch(sect)    /* now put it into right table  */
// 			{   case 1:     /* long descriptions            */
// 				ltext[oldloc].seekadr=seekhere;
// 				ltext[oldloc].txtlen=maystart-seekstart;
// 				break;
// 			    case 2:     /* short descriptions           */
// 				stext[oldloc].seekadr=seekhere;
// 				stext[oldloc].txtlen=maystart-seekstart;
// 				break;
// 			    case 5:     /* object descriptions          */
// 				ptext[oldloc].seekadr=seekhere;
// 				ptext[oldloc].txtlen=maystart-seekstart;
// 				break;
// 			    case 6:     /* random messages              */
// 				if (oldloc>RTXSIZ)
// 				{       printf("Too many random msgs\n");
// 					exit(0);
// 				}
// 				rtext[oldloc].seekadr=seekhere;
// 				rtext[oldloc].txtlen=maystart-seekstart;
// 				break;
// 			    case 10:    /* class messages               */
// 				ctext[clsses].seekadr=seekhere;
// 				ctext[clsses].txtlen=maystart-seekstart;
// 				cval[clsses++]=oldloc;
// 				break;
// 			    case 12:    /* magic messages               */
// 				if (oldloc>MAGSIZ)
// 				{       printf("Too many magic msgs\n");
// 					exit(0);
// 				}
// 				mtext[oldloc].seekadr=seekhere;
// 				mtext[oldloc].txtlen=maystart-seekstart;
// 				break;
// 			    default:
// 				printf("rdesc called with bad section\n");
// 				exit(0);
// 			}
// 			seekhere += maystart-seekstart;
// 		}
// 		if (locc<0)
// 		{       outsw=0;        /* turn off output              */
// 			seekhere += 3;  /* -1<delimiter>                */
// 			return;
// 		}
// 		if (sect!=5 || (locc>0 && locc<100))
// 		{       if (oldloc!=locc)/* starting a new message       */
// 				seekstart=maystart;
// 			oldloc=locc;
// 		}
// 		FLUSHLF;                /* scan the line                */
// 	}
// }


// rtrav()                                 /* read travel table            */
// {       register int locc;
// 	register struct travlist *t;
// 	register char *s;
// 	char buf[12];
// 	int len,m,n,entries;
// 	for (oldloc= -1;;)              /* get another line             */
// 	{       if ((locc=rnum())!=oldloc && oldloc>=0) /* end of entry */
// 		{
// 			t->next = 0;    /* terminate the old entry      */
// 		/*      printf("%d:%d entries\n",oldloc,entries);       */
// 		/*      twrite(oldloc);                                 */
// 		}
// 		if (locc== -1) return;
// 		if (locc!=oldloc)        /* getting a new entry         */
// 		{       t=travel[locc]=(struct travlist *) malloc(sizeof (struct travlist));
// 		/*      printf("New travel list for %d\n",locc);        */
// 			entries=0;
// 			oldloc=locc;
// 		}
// 		for (s=buf;; *s++)      /* get the newloc number /ASCII */
// 			if ((*s=next())==TAB || *s==LF) break;
// 		*s=0;
// 		len=length(buf)-1;      /* quad long number handling    */
// 	/*      printf("Newloc: %s (%d chars)\n",buf,len);              */
// 		if (len<4)              /* no "m" conditions            */
// 		{       m=0;
// 			n=atoi(buf);    /* newloc mod 1000 = newloc     */
// 		}
// 		else                    /* a long integer               */
// 		{       n=atoi(buf+len-3);
// 			buf[len-3]=0;   /* terminate newloc/1000        */
// 			m=atoi(buf);
// 		}
// 		while (breakch!=LF)     /* only do one line at a time   */
// 		{       if (entries++) t=t->next=(struct travlist *) malloc(sizeof (struct travlist));
// 			t->tverb=rnum();/* get verb from the file       */
// 			t->tloc=n;      /* table entry mod 1000         */
// 			t->conditions=m;/* table entry / 1000           */
// 		/*      printf("entry %d for %d\n",entries,locc);       */
// 		}
// 	}
// }

// #ifdef DEBUG

// twrite(loq)                             /* travel options from this loc */
// int loq;
// {       register struct travlist *t;
// 	printf("If");
// 	speak(&ltext[loq]);
// 	printf("then\n");
// 	for (t=travel[loq]; t!=0; t=t->next)
// 	{       printf("verb %d takes you to ",t->tverb);
// 		if (t->tloc<=300)
// 			speak(&ltext[t->tloc]);
// 		else if (t->tloc<=500)
// 			printf("special code %d\n",t->tloc-300);
// 		else
// 			rspeak(t->tloc-500);
// 		printf("under conditions %d\n",t->conditions);
// 	}
// }

// #endif DEBUG

void rvoc()
{
	vocinit();
	vocab("road",-2,2);
	vocab("hill",-2,2);
	vocab("enter",-2,3);
	vocab("upstr",-2,4);
	vocab("downs",-2,5);
	vocab("fores",-2,6);
	vocab("forwa",-2,7);
	vocab("conti",-2,7);
	vocab("onwar",-2,7);
	vocab("back",-2,8);
	vocab("retur",-2,8);
	vocab("retre",-2,8);
	vocab("valle",-2,9);
	vocab("stair",-2,10);
	vocab("out",-2,11);
	vocab("outsi",-2,11);
	vocab("exit",-2,11);
	vocab("leave",-2,11);
	vocab("build",-2,12);
	vocab("house",-2,12);
	vocab("gully",-2,13);
	vocab("strea",-2,14);
	vocab("rock",-2,15);
	vocab("bed",-2,16);
	vocab("crawl",-2,17);
	vocab("cobbl",-2,18);
	vocab("inwar",-2,19);
	vocab("insid",-2,19);
	vocab("in",-2,19);
	vocab("surfa",-2,20);
	vocab("null",-2,21);
	vocab("nowhe",-2,21);
	vocab("dark",-2,22);
	vocab("passa",-2,23);
	vocab("tunne",-2,23);
	vocab("low",-2,24);
	vocab("canyo",-2,25);
	vocab("awkwa",-2,26);
	vocab("giant",-2,27);
	vocab("view",-2,28);
	vocab("upwar",-2,29);
	vocab("up",-2,29);
	vocab("u",-2,29);
	vocab("above",-2,29);
	vocab("ascen",-2,29);
	vocab("d",-2,30);
	vocab("downw",-2,30);
	vocab("down",-2,30);
	vocab("desce",-2,30);
	vocab("pit",-2,31);
	vocab("outdo",-2,32);
	vocab("crack",-2,33);
	vocab("steps",-2,34);
	vocab("dome",-2,35);
	vocab("left",-2,36);
	vocab("right",-2,37);
	vocab("hall",-2,38);
	vocab("jump",-2,39);
	vocab("barre",-2,40);
	vocab("over",-2,41);
	vocab("acros",-2,42);
	vocab("east",-2,43);
	vocab("e",-2,43);
	vocab("west",-2,44);
	vocab("w",-2,44);
	vocab("north",-2,45);
	vocab("n",-2,45);
	vocab("south",-2,46);
	vocab("s",-2,46);
	vocab("ne",-2,47);
	vocab("se",-2,48);
	vocab("sw",-2,49);
	vocab("nw",-2,50);
	vocab("debri",-2,51);
	vocab("hole",-2,52);
	vocab("wall",-2,53);
	vocab("broke",-2,54);
	vocab("y2",-2,55);
	vocab("climb",-2,56);
	vocab("look",-2,57);
	vocab("exami",-2,57);
	vocab("touch",-2,57);
	vocab("descr",-2,57);
	vocab("floor",-2,58);
	vocab("room",-2,59);
	vocab("slit",-2,60);
	vocab("slab",-2,61);
	vocab("slabr",-2,61);
	vocab("xyzzy",-2,62);
	vocab("depre",-2,63);
	vocab("entra",-2,64);
	vocab("plugh",-2,65);
	vocab("secre",-2,66);
	vocab("cave",-2,67);
	vocab("cross",-2,69);
	vocab("bedqu",-2,70);
	vocab("plove",-2,71);
	vocab("orien",-2,72);
	vocab("caver",-2,73);
	vocab("shell",-2,74);
	vocab("reser",-2,75);
	vocab("main",-2,76);
	vocab("offic",-2,76);
	vocab("fork",-2,77);
	vocab("keys",-2,1001);
	vocab("key",-2,1001);
	vocab("lamp",-2,1002);
	vocab("headl",-2,1002);
	vocab("lante",-2,1002);
	vocab("grate",-2,1003);
	vocab("cage",-2,1004);
	vocab("wand",-2,1005);
	vocab("rod",-2,1005);
	vocab("wand",-2,1006);
	vocab("rod",-2,1006);
	vocab("steps",-2,1007);
	vocab("bird",-2,1008);
	vocab("door",-2,1009);
	vocab("pillo",-2,1010);
	vocab("velve",-2,1010);
	vocab("snake",-2,1011);
	vocab("fissu",-2,1012);
	vocab("table",-2,1013);
	vocab("clam",-2,1014);
	vocab("oyste",-2,1015);
	vocab("magaz",-2,1016);
	vocab("issue",-2,1016);
	vocab("spelu",-2,1016);
	vocab("\"spel",-2,1016);
	vocab("dwarf",-2,1017);
	vocab("dwarv",-2,1017);
	vocab("knife",-2,1018);
	vocab("knive",-2,1018);
	vocab("food",-2,1019);
	vocab("ratio",-2,1019);
	vocab("bottl",-2,1020);
	vocab("jar",-2,1020);
	vocab("water",-2,1021);
	vocab("h2o",-2,1021);
	vocab("oil",-2,1022);
	vocab("mirro",-2,1023);
	vocab("plant",-2,1024);
	vocab("beans",-2,1024);
	vocab("plant",-2,1025);
	vocab("stala",-2,1026);
	vocab("shado",-2,1027);
	vocab("figur",-2,1027);
	vocab("axe",-2,1028);
	vocab("drawi",-2,1029);
	vocab("pirat",-2,1030);
	vocab("drago",-2,1031);
	vocab("chasm",-2,1032);
	vocab("troll",-2,1033);
	vocab("troll",-2,1034);
	vocab("bear",-2,1035);
	vocab("messa",-2,1036);
	vocab("volca",-2,1037);
	vocab("geyse",-2,1037);
	vocab("machi",-2,1038);
	vocab("vendi",-2,1038);
	vocab("batte",-2,1039);
	vocab("carpe",-2,1040);
	vocab("moss",-2,1040);
	vocab("gold",-2,1050);
	vocab("nugge",-2,1050);
	vocab("diamo",-2,1051);
	vocab("silve",-2,1052);
	vocab("bars",-2,1052);
	vocab("jewel",-2,1053);
	vocab("coins",-2,1054);
	vocab("chest",-2,1055);
	vocab("box",-2,1055);
	vocab("treas",-2,1055);
	vocab("eggs",-2,1056);
	vocab("egg",-2,1056);
	vocab("nest",-2,1056);
	vocab("tride",-2,1057);
	vocab("vase",-2,1058);
	vocab("ming",-2,1058);
	vocab("shard",-2,1058);
	vocab("potte",-2,1058);
	vocab("emera",-2,1059);
	vocab("plati",-2,1060);
	vocab("pyram",-2,1060);
	vocab("pearl",-2,1061);
	vocab("rug",-2,1062);
	vocab("persi",-2,1062);
	vocab("spice",-2,1063);
	vocab("chain",-2,1064);
	vocab("carry",-2,2001);
	vocab("take",-2,2001);
	vocab("keep",-2,2001);
	vocab("catch",-2,2001);
	vocab("steal",-2,2001);
	vocab("captu",-2,2001);
	vocab("get",-2,2001);
	vocab("tote",-2,2001);
	vocab("drop",-2,2002);
	vocab("relea",-2,2002);
	vocab("free",-2,2002);
	vocab("disca",-2,2002);
	vocab("dump",-2,2002);
	vocab("say",-2,2003);
	vocab("chant",-2,2003);
	vocab("sing",-2,2003);
	vocab("utter",-2,2003);
	vocab("mumbl",-2,2003);
	vocab("unloc",-2,2004);
	vocab("open",-2,2004);
	vocab("nothi",-2,2005);
	vocab("lock",-2,2006);
	vocab("close",-2,2006);
	vocab("light",-2,2007);
	vocab("on",-2,2007);
	vocab("extin",-2,2008);
	vocab("off",-2,2008);
	vocab("wave",-2,2009);
	vocab("shake",-2,2009);
	vocab("swing",-2,2009);
	vocab("calm",-2,2010);
	vocab("placa",-2,2010);
	vocab("tame",-2,2010);
	vocab("walk",-2,2011);
	vocab("run",-2,2011);
	vocab("trave",-2,2011);
	vocab("go",-2,2011);
	vocab("proce",-2,2011);
	vocab("conti",-2,2011);
	vocab("explo",-2,2011);
	vocab("goto",-2,2011);
	vocab("follo",-2,2011);
	vocab("turn",-2,2011);
	vocab("attac",-2,2012);
	vocab("kill",-2,2012);
	vocab("slay",-2,2012);
	vocab("fight",-2,2012);
	vocab("hit",-2,2012);
	vocab("strik",-2,2012);
	vocab("pour",-2,2013);
	vocab("eat",-2,2014);
	vocab("devou",-2,2014);
	vocab("drink",-2,2015);
	vocab("rub",-2,2016);
	vocab("throw",-2,2017);
	vocab("toss",-2,2017);
	vocab("quit",-2,2018);
	vocab("find",-2,2019);
	vocab("where",-2,2019);
	vocab("inven",-2,2020);
	vocab("inv",-2,2020);
	vocab("feed",-2,2021);
	vocab("fill",-2,2022);
	vocab("blast",-2,2023);
	vocab("deton",-2,2023);
	vocab("ignit",-2,2023);
	vocab("blowu",-2,2023);
	vocab("score",-2,2024);
	vocab("fee",-2,2025);
	vocab("fie",-2,2025);
	vocab("foe",-2,2025);
	vocab("foo",-2,2025);
	vocab("fum",-2,2025);
	vocab("brief",-2,2026);
	vocab("read",-2,2027);
	vocab("perus",-2,2027);
	vocab("break",-2,2028);
	vocab("shatt",-2,2028);
	vocab("smash",-2,2028);
	vocab("wake",-2,2029);
	vocab("distu",-2,2029);
	vocab("suspe",-2,2030);
	vocab("pause",-2,2030);
	vocab("save",-2,2030);
	vocab("hours",-2,2031);
	vocab("fee",-2,3001);
	vocab("fie",-2,3002);
	vocab("foe",-2,3003);
	vocab("foo",-2,3004);
	vocab("fum",-2,3005);
	vocab("sesam",-2,3050);
	vocab("opens",-2,3050);
	vocab("abra",-2,3050);
	vocab("abrac",-2,3050);
	vocab("shaza",-2,3050);
	vocab("hocus",-2,3050);
	vocab("pocus",-2,3050);
	vocab("help",-2,3051);
	vocab("?",-2,3051);
	vocab("tree",-2,3064);
	vocab("trees",-2,3064);
	vocab("dig",-2,3066);
	vocab("excav",-2,3066);
	vocab("lost",-2,3068);
	vocab("mist",-2,3069);
	vocab("fuck",-2,3079);
	vocab("stop",-2,3139);
	vocab("info",-2,3142);
	vocab("infor",-2,3142);
	vocab("swim",-2,3147);
}


// rlocs()                                 /* initial object locations     */
// {	for (;;)
// 	{       if ((obj=rnum())<0) break;
// 		plac[obj]=rnum();       /* initial loc for this obj     */
// 		if (breakch==TAB)       /* there's another entry        */
// 			fixd[obj]=rnum();
// 		else    fixd[obj]=0;
// 	}
// }

// rdflt()                                 /* default verb messages        */
// {	for (;;)
// 	{       if ((verb=rnum())<0) break;
// 		actspk[verb]=rnum();
// 	}
// }

// rliq()                                  /* liquid assets &c: cond bits  */
// {       register int bitnum;
// 	for (;;)                        /* read new bit list            */
// 	{       if ((bitnum=rnum())<0) break;
// 		for (;;)                /* read locs for bits           */
// 		{       cond[rnum()] |= setbit[bitnum];
// 			if (breakch==LF) break;
// 		}
// 	}
// }

// rhints()
// {       register int hintnum,i;
// 	hntmax=0;
// 	for (;;)
// 	{       if ((hintnum=rnum())<0) break;
// 		for (i=1; i<5; i++)
// 			hints[hintnum][i]=rnum();
// 		if (hintnum>hntmax) hntmax=hintnum;
// 	}
// }


void rspeak(int msg)
//int msg;
{       if (msg!=0) speak(&rtext[msg]);
}


void mspeak(int msg)
//int msg;
{       if (msg!=0) speak(&mtext[msg]);
}


void speak(struct text *msg)       /* read, decrypt, and print a message (not ptext)      */
//struct text *msg;/* msg is a pointer to seek address and length of mess */
{
	printf(msg->seekadr);
}


// pspeak(m,skip) /* read, decrypt an print a ptext message              */
// int m;         /* msg is the number of all the p msgs for this place  */
// int skip;       /* assumes object 1 doesn't have prop 1, obj 2 no prop 2 &c*/
// {
// 	register char *s,nonfirst;
// 	char *numst, save;
// 	struct text *msg;
// 	char *tbuf;

// 	msg = &ptext[m];
// 	if ((tbuf=(char *) malloc(msg->txtlen + 1)) == 0) bug(108);
// 	memcpy(tbuf, msg->seekadr, msg->txtlen + 1);   /* Room to null */
// 	s = tbuf;

// 	nonfirst=0;
// 	while (s - tbuf < msg->txtlen) /* read line at a time */
// 	{       tape=iotape;            /* restart decryption tape      */
// 		for (numst=s; (*s^= *tape++)!=TAB; s++); /* get number  */

// 		save = *s; /* Temporarily trash the string (cringe) */
// 		*s++ = 0; /* decrypting number within the string          */

// 		if (atoi(numst) != 100 * skip && skip >= 0)
// 		{       while ((*s++^*tape++)!=LF) /* flush the line    */
// 				if (*tape==0) tape=iotape;
// 			continue;
// 		}
// 		if ((*s^*tape)=='>' && (*(s+1)^*(tape+1))=='$' &&
// 			(*(s+2)^*(tape+2))=='<') break;
// 		if (blklin && ! nonfirst++) putchar('\n');
// 		do
// 		{       if (*tape==0) tape=iotape;
// 			putchar(*s^*tape);
// 		} while ((*s++^*tape++)!=LF);   /* better end with LF   */
// 		if (skip<0) break;
// 	}
// 	free(tbuf);
// }
