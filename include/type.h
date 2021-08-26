
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            type.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#ifndef	_ORANGES_TYPE_H_
#define	_ORANGES_TYPE_H_

/* routine types */
#define	PUBLIC		/* PUBLIC is the opposite of PRIVATE */
#define	PRIVATE	static	/* PRIVATE x limits the scope of x */
#define true 1
#define false 0

typedef	unsigned long long	u64;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef unsigned char bool;

typedef	char *			va_list;

typedef	void	(*int_handler)	();
typedef	void	(*task_f)	();
typedef	void	(*irq_handler)	(int irq);

typedef void*	system_call;


/**
 * MESSAGE mechanism is borrowed from MINIX
 */
struct mess1 {
	int m1i1;
	int m1i2;
	int m1i3;
	int m1c1;
	int m1c2;
	int m1c3;
};
struct mess2 {
	void* m2p1;
	void* m2p2;
	void* m2p3;
	void* m2p4;
};
struct mess3 {
	int	m3i1;
	int	m3i2;
	int	m3i3;
	int	m3i4;
	u64	m3l1;
	u64	m3l2;
	void*	m3p1;
	void*	m3p2;
};
struct mess4 {
	char m4c1;
	char m4c2;
	char m4c3;
	int m4i1;
	int m4i2;
	int m4i3;
	int m4i4;
	int m4p1;
};
typedef struct {
	int source;
	int type;
	union {
		struct mess1 m1;
		struct mess2 m2;
		struct mess3 m3;
		struct mess4 m4;
	} u;
} MESSAGE;



struct boot_params {
    int     mem_size;   /* memory size */
    unsigned char * kernel_file;    /* addr of kernel file */
};


#endif /* _ORANGES_TYPE_H_ */
