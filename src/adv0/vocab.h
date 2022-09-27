#ifndef __VOCAB_H
#define __VOCAB_H

void drop(int object,int where);
int vocab(char *word,int type,int value);
int length(char *str);
void move(int object,int where);
void carry(int object,int where);
void juggle(int object);
void copystr(char *w1,char *w2);
void dstroy(int object);
int weq(char *w1,char *w2);
int put(int object,int where,int pval);
void prht();

#endif