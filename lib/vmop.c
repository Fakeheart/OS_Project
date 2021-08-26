#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"
#include "DrawingTools.h"

/*****************************************************************************
 *                                vm_print_letter
 *****************************************************************************/
/**
 * 在200*320的屏幕上任意位置打印字符串.
 * 
 * @param PositionX  打印字母所在的X轴像素位置.
 * @param PositionY  打印字母所在的Y轴像素位置.
 * @param LetterColor  要打印的目标字符串颜色.
 * @param TargetString  要打印的目标字符串.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_print_letter(int PositionX, int PositionY, char LetterColor, char *TargetString)
{
	MESSAGE msg; //定义进程通信间的消息体
	msg.type = PRINTLETTER;//定义消息的类型
	msg.LETTERPOSITIONX = PositionX;
	msg.LETTERPOSITIONY = PositionY;
	msg.LETTERCOLOR = LetterColor;
	msg.STRINGLEN = strlen(TargetString);
	msg.TARGETSTRING = (void*)TargetString;
	send_recv(BOTH, TASK_VM, &msg);
}

/*****************************************************************************
 *                                vm_op_pixel
 *****************************************************************************/
/**
 * 在200*320的屏幕上绘制任意像素点.
 * 
 * @param PositionX  绘制像素所在的X轴位置.(0~320)
 * @param PositionY  绘制像素所在的Y轴位置.(0~200)
 * @param PixelColor 像素点颜色.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_op_pixel(int PositionX, int PositionY,char PixelColor)
{
	MESSAGE msg;
	msg.type = OP_PIXEL;
	msg.PIXELPOSX = PositionX;
	msg.PIXELPOSY = PositionY;
	msg.PIXELCOLOR = PixelColor;
	send_recv(BOTH, TASK_VM, &msg);
}
/*****************************************************************************
 *                                vm_op_line
 *****************************************************************************/
/**
 * 在200*320的屏幕上绘制直线.
 * 
 * @param StartPosX  绘制直线所在起始的X轴位置.(0~320)
 * @param StartPosY  绘制直线所在起始的Y轴位置.(0~200)
 * @param EndPosX  绘制直线所在结束的X轴位置.(0~320)
 * @param EndPosY  绘制直线所在结束的Y轴位置.(0~200)
 * @param LineColor  绘制直线的颜色.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_op_line(int StartPosX, int StartPosY, int EndPosX, int EndPosY,char LineColor)
{
	MESSAGE msg;
	msg.type = LINE;	
	msg.SPOSX = StartPosX ;
	msg.SPOSY = StartPosY ;
	msg.EPOSX = EndPosX ;
	msg.EPOSY = EndPosY ;
	msg.LCOLOR = LineColor;
	send_recv(BOTH, TASK_VM, &msg);
}
/*****************************************************************************
 *                                vm_op_rectangle
 *****************************************************************************/
/**
 * 在200*320的屏幕上绘制矩形.
 * 
 * @param StartPosX  绘制矩形所在左上角X轴位置.(0~320)
 * @param StartPosY  绘制矩形所在左上角Y轴位置.(0~200)
 * @param EndPosX  绘制矩形所在右下角X轴位置.(0~320)
 * @param EndPosY  绘制矩形所在右下角Y轴位置.(0~200)
 * @param RecColor  绘制矩形的颜色.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_op_rectangle(int StartPosX, int StartPosY, int EndPosX, int EndPosY,char RecColor)
{
	MESSAGE msg;
	msg.type = RECTAN;
	msg.SPOSX = StartPosX ;
	msg.SPOSY = StartPosY ;
	msg.EPOSX = EndPosX ;
	msg.EPOSY = EndPosY ;
	msg.RCOLOR = RecColor;
	send_recv(BOTH, TASK_VM, &msg);
}

/*****************************************************************************
 *                                vm_op_circle
 *****************************************************************************/
/**
 * 在200*320的屏幕上绘制圆形.
 * 
 * @param CirCenterX  绘制圆的圆心所在X轴的位置(0~320)
 * @param CirCenterY  绘制圆的圆心所在Y轴的位置.(0~200)
 * @param Radius  绘制圆的半径.
 * @param CirColor  绘制圆的颜色.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_op_circle(int CirCenterX, int CirCenterY, int Radius, char CirColor)
{
	MESSAGE msg;
	msg.type = CIRCLE ;
	msg.CIRCENTERX = CirCenterX ;
	msg.CIRCENTERY = CirCenterY ;
	msg.RADIUS = Radius ;
	msg.CCOLOR = CirColor ;
	send_recv(BOTH, TASK_VM, &msg);
}

/*****************************************************************************
 *                                vm_op_rec_64px
 *****************************************************************************/
/**
 * 在200*320的屏幕上绘制8px*8px的矩形.
 * 
 * @param StartPosX  绘制8px*8px矩形所在左上角X轴位置.(0~320)
 * @param StartPosY  绘制8px*8px矩形所在左上角Y轴位置.(0~200)
 * @param RecColor   绘制8px*8px矩形的颜色.
 *
 * @return None.
 *****************************************************************************/
PUBLIC void vm_op_rec_64px(int StartPosX, int StartPosY,char Color)
{
	MESSAGE msg;
	msg.type = REC64;
	msg.SPOSX = StartPosX ;
	msg.SPOSY = StartPosY ;
	msg.RCOLOR = Color;
	send_recv(BOTH, TASK_VM, &msg);
}