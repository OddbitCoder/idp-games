// 117 characters
int putchar(int),abs(int),i,x,y;main(){for(;i++<1360;)x=abs(i%80-9),y=abs(i/80-8),putchar(x>4&y>4|abs(x-y)>4?32:42);}