#ifndef	_DRAWING_TOOLS_H_
#define	_DRAWING_TOOLS_H_

#define	VGA_MEM_BASE	0xa0000
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define SCREEN_SIZE 320*200
#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#include "type.h"
/* lib/vmop.c*/
PUBLIC void vm_print_letter(int PositionX, int PositionY, char LetterColor, char *TargetString);
PUBLIC void vm_op_pixel(int PositionX, int PositionY,char PixelColor);
PUBLIC void vm_op_line(int StartPosX, int StartPosY, int EndPosX, int EndPosY,char LineColor);
PUBLIC void vm_op_rectangle(int StartPosX, int StartPosY, int EndPosX, int EndPosY,char RecColor);
PUBLIC void vm_op_circle(int CirCenterX, int CirCenterY, int Radius, char CirColor);
PUBLIC void vm_op_rec_64px(int StartPosX, int StartPosY,char Color);
/* vm/Tools.c*/
PUBLIC void clearScreen();
PUBLIC void pixel(int x,int y,char c);
PUBLIC void boxfill64(int x,int y,char c);
PUBLIC void boxfill8(int xsize, unsigned char c, int x0, int y0, int x1, int y1);
PUBLIC void putfont8(int x, int y, char c, char *font);
PUBLIC void putfonts8_char(int x, int y, char c, char ch);
PUBLIC void putfonts8_asc(int x, int y, char c, unsigned char *s);

#endif 