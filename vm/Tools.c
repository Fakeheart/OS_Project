#include "DrawingTools.h"
#include "stdio.h"
#include "string.h"
#include "font.h"
PUBLIC void boxfill64(int x,int y,char c)
{
	char* p = (char *) 0xa0000;
    	for(int i=0;i<8;i++)
       {
           for (int j=0;j<8;j++)
           {
               p[(x*8+i)*320+(y*8+j)] = c;
           }
       }
	   return;
}
PUBLIC void boxfill8(int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x,y,pos;
	char* p = (char *) 0xa0000;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
		{
			pos=y * xsize + x;
			if(pos>=SCREEN_SIZE) continue;
			p[pos]=c;
		}
	}
	return;
}
PUBLIC void clearScreen()
{
	boxfill8(SCREEN_WIDTH,15,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);	
}
PUBLIC void pixel(int x,int y,char c)
{
	char* p = (char *) 0xa0000;
	if(x<0||y<0) return;
	int pos=y*SCREEN_WIDTH+x;
	if(pos<SCREEN_SIZE)
	{	
		p[pos]=c;
		return;
	}
	else
		return;
}
PUBLIC void putfont8(int x, int y, char c, char *font)
{
	char *p, d;
	for (int i = 0; i < 16; i++) {
		p = 0xa0000 + (y + i) *SCREEN_WIDTH + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}
PUBLIC void putfonts8_char(int x, int y, char c, char ch)
{
	putfont8(x, y, c, hankaku + ch * 16);
}

PUBLIC void putfonts8_asc(int x, int y, char c, unsigned char *s)
{
	for (; *s != 0x00; s++) {
		putfont8(x, y, c, hankaku + *s * 16);
		x += FONT_WIDTH;
	}
	return;
}

