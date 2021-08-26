#include "DrawingTools.h"
#include "type.h"
#include "config.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
PUBLIC void task_vm()
{
	while (1) {
		send_recv(RECEIVE, ANY, &vm_msg);
		int src = vm_msg.source;
		int reply = 1;
		int msgtype = vm_msg.type;
		switch (msgtype) {
		case PRINTLETTER:
			do_vm_pl();
			break;
		case RECTAN:
			do_vm_rec();
			break;
		case REC64:
			do_vm_rec64px();
			break;
		case OP_PIXEL:
			do_vm_pixel();
			break;
		case LINE:	
			do_vm_line();
			break;
		case CIRCLE:
			do_drawCircle();
			break;
		default:
			dump_msg("unknown msg from vm", &vm_msg);
			assert(0);
			break;
		}
		if (reply) {
			vm_msg.type = SYSCALL_RET;
			send_recv(SEND, src, &vm_msg);
		}
	}
}
void do_vm_pl()
{
	unsigned char _s[MAX_PATH];
	assert(vm_msg.STRINGLEN < MAX_PATH);
	phys_copy((void*)va2la(TASK_VM, _s),(void*)va2la(vm_msg.source, vm_msg.TARGETSTRING),vm_msg.STRINGLEN);
	_s[vm_msg.STRINGLEN] = 0;
	putfonts8_asc(vm_msg.LETTERPOSITIONX, vm_msg.LETTERPOSITIONY, vm_msg.LETTERCOLOR, _s);
}
void do_vm_rec()
{
	boxfill8(SCREEN_WIDTH,vm_msg.RCOLOR,vm_msg.SPOSX,vm_msg.SPOSY,vm_msg.EPOSX,vm_msg.EPOSY);
}
void do_vm_rec64px()
{
	boxfill64(vm_msg.SPOSX,vm_msg.SPOSY,vm_msg.RCOLOR);
	return;
}
void do_vm_pixel()
{
	pixel(vm_msg.PIXELPOSX,vm_msg.PIXELPOSY,vm_msg.PIXELCOLOR);
	return;
}

void do_vm_line()
{
	int x0 = vm_msg.SPOSX;int y0 = vm_msg.SPOSY;
	int x1 = vm_msg.EPOSX;int y1 = vm_msg.EPOSY;
	char c = vm_msg.LCOLOR;
	int i,j,temp;
	if(x0==x1)
	{
		if(y0>y1)
		{
			temp=y0;y0=y1;y1=temp;
		}
		for(i=y0;i<=y1;i++)
			pixel(x0,i,c);
		return;
	}
	else if(y0==y1)
	{
		if(x0>x1)
		{
			temp=x0;x0=y1;x1=temp;
		}
		for(i=x0;i<=x1;i++)
			pixel(i,y0,c);
		return;
	}
	else			
		return;
}

void do_drawCircle()
{
	int cx=vm_msg.CIRCENTERX;int cy=vm_msg.CIRCENTERY;
	int radius=vm_msg.RADIUS;int radius_2=vm_msg.RADIUS*vm_msg.RADIUS;
	char c=vm_msg.CCOLOR;
	int leftpos=cx-radius;int rightpos=cx+radius;
	int uppos=cy-radius;int downpos=cy+radius;
	leftpos=(leftpos>=0)?leftpos:0;rightpos=(rightpos<320)?rightpos:319;
	uppos=(uppos>=0)?uppos:0;downpos=(downpos<200)?downpos:199;
	for(int i=leftpos;i<=rightpos;i++)
	{
		for(int j=uppos;j<=downpos;j++)
		{
			int R =(i-cx)*(i-cx)+(j-cy)*(j-cy);
			if(R<=radius_2)
				pixel(i,j,c);
		}
	}
}