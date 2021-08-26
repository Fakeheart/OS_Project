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

/*****************************************************************************
 *                                rename
 *****************************************************************************/
/**
 * set a file's name to another one.
 * 
 * @param pathname  The full path of the file to be renamed.
 * @param buf       The new name of the file.
 * 
 * @return File descriptor if successful, otherwise -1.
 *****************************************************************************/
PUBLIC int rename(const char *pathname, const char *buf)
{
	MESSAGE msg;

	msg.type	= RENAME;

	msg.PATHNAME	= (void*)pathname;
	msg.BUF		= (void*)buf;
	msg.NAME_LEN	= strlen(pathname);
	msg.BUF_LEN	= strlen(buf);
	
	send_recv(BOTH, TASK_FS, &msg);
	assert(msg.type == SYSCALL_RET);

	return msg.RETVAL;
}