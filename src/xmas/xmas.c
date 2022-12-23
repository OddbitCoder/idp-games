#include<stdio.h>
#define X(x,y) y>4&&x>y-5&&x<23-y||y<14&&x>13-y&&x<5+y
main(){
	puts("#include<stdio.h>");
	puts("#define X(x,y) y>4&&x>y-5&&x<23-y||y<14&&x>13-y&&x<5+y");
	puts("main(){int x,y=0;for(;y++<17;puts(\"\"))for(x=0;x++<17;)putchar(X(x,y)||X(y,x)?42:32);}");
	int x,y=0;for(;y++<17;puts(""))for(x=0;x++<17;)putchar(X(x,y)||X(y,x)?42:32);
}