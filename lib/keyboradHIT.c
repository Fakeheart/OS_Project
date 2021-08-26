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

PUBLIC u32 GETINPUT()
{
	MESSAGE msg;
	msg.type = KBHIT;
	msg.M4P1 =0;
	send_recv(BOTH, TASK_TTY, &msg);
	return msg.RETVAL;
}
