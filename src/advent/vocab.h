/*
 * vocab.h
 *
 * Miha Grčar 2022
 *
 * Vocabulary lookup and inventory functions
 */

#ifndef __VOCAB_H
#define __VOCAB_H

void dstroy(int object);
void juggle(int object);
void move(int object,int where);
int put(int object,int where,int pval);
void carry(int object,int where);
void drop(int object,int where);

int vocab(char *word,int type);

void copystr(char *w1,char *w2);
int weq(char *w1,char *w2);
int length(char *str);

#endif