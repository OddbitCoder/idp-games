#ifndef __CONIO_H
#define __CONIO_H

void setWindowSize();
void clearScreen();
void setPos(int x, int y);
void getPos(int *x, int *y);
void setColor(int color);
void writeHeader(int line, const char *leftText, const char *rightText);
void delay(int ms);
char getKey();
char getKeySilent();
char getOneOf(char key1, char key2, char key3);
char *getString(char *buffer, int max);

#endif