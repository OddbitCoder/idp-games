#define X(x,y)x<5&x>y-5|y<5&y>x-5
x;main(){for(;x++<1360;)putchar(X(abs(x%80-9),abs(x/80-8))?42:32);}