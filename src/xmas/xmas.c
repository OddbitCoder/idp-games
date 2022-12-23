#include<stdio.h>
#define X(x,y)y>4&x>y-5&x<23-y|y<14&x>13-y&x<5+y
int x;main(){for(;x++<1440;)putchar(X(x%80,x/80)|X(x/80,x%80)?42:32);}