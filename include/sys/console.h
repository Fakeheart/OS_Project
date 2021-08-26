
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			      console.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
						    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef _ORANGES_CONSOLE_H_
#define _ORANGES_CONSOLE_H_

#define SCR_UP	1	/* scroll upward */
#define SCR_DN	-1	/* scroll downward */

#define SCR_SIZE		(40 * 12)
#define SCR_WIDTH		 40
#define PIX_WIDTH		 320
#define PIX_HEIGHT		 200

#define DEFAULT_CHAR_COLOR	(MAKE_COLOR(BLACK, WHITE))
#define GRAY_CHAR		(MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR		(MAKE_COLOR(BLUE, RED) | BRIGHT)

/* CONSOLE */
typedef struct s_console
{
	unsigned int	crtc_start; /* set CRTC start addr reg */
	unsigned int	orig;	    /* start addr of the console */
	unsigned int	con_size;   /* how many words does the console have */
	unsigned int	cursor;
	int		is_full;
	char cbuf[SCR_SIZE];
}CONSOLE;

#endif /* _ORANGES_CONSOLE_H_ */
