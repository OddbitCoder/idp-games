//i,x;main(y){for(;i++<1360;)x=abs(i%80-9),y=abs(i/80-8),putchar(x>4&y>4|abs(x-y)>4?32:42);}
// independent of terminal size
i,x;main(y){for(;i++<306;)x=abs(i%18-9),y=abs(i/18-8),putchar(x>8?10:x>4&y>4|abs(x-y)>4?32:42);}
