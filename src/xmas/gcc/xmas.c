i,x,y;main(){for(;i++<1360;){x=abs(i%80-9);y=abs(i/80-8);putchar(x<5&x>y-5|y<5&y>x-5?42:32);}}