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
#include "Snaker.h"
#include "TiaoJia_Chess.h"
#include "Jop.h"
#include "24.h"
#include "fiveinaRow.h"
#include "calculator.h"
#include "palette.h"
#include "DrawingTools.h"

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
    init_palette();
    struct task* p_task;
    struct proc* p= proc_table;
    char* p_task_stack = task_stack + STACK_SIZE_TOTAL;
    // u16   selector_ldt = SELECTOR_LDT_FIRST;
    u8    privilege;
    u8    rpl;
    int   eflags;
    int   i, j;
    int   prio;
    for (i = 0; i < NR_TASKS+NR_PROCS; i++)
    {
        if (i >= NR_TASKS + NR_NATIVE_PROCS) {
            p->p_flags = FREE_SLOT;
            p++;
            p_task++;
            continue;
        }
        if (i < NR_TASKS)
        {     /* 任务 */
            p_task    = task_table + i;
            privilege = PRIVILEGE_TASK;
            rpl       = RPL_TASK;
            eflags    = 0x1202; /* IF=1, IOPL=1, bit 2 is always 1   1 0010 0000 0010(2)*/
            prio      = 15;     //设定优先级为15
        }
        else
        {                  /* 用户进程 */
            p_task    = user_proc_table + (i - NR_TASKS);
            privilege = PRIVILEGE_USER;
            rpl       = RPL_USER;
            eflags    = 0x202; /* IF=1, bit 2 is always 1              0010 0000 0010(2)*/
            prio      = 5;     //设定优先级为5
        }

        strcpy(p->name, p_task->name); /* 设定进程名称 */
        p->p_parent = NO_TASK;

        if (strcmp(p_task->name, "Init") != 0) {
            p->ldts[INDEX_LDT_C]  = gdt[SELECTOR_KERNEL_CS >> 3];
            p->ldts[INDEX_LDT_RW] = gdt[SELECTOR_KERNEL_DS >> 3];

            /* change the DPLs */
            p->ldts[INDEX_LDT_C].attr1  = DA_C   | privilege << 5;
            p->ldts[INDEX_LDT_RW].attr1 = DA_DRW | privilege << 5;
        }
        else {      /* INIT process */
            unsigned int k_base;
            unsigned int k_limit;
            int ret = get_kernel_map(&k_base, &k_limit);
            assert(ret == 0);
            init_descriptor(&p->ldts[INDEX_LDT_C],
                  0, /* bytes before the entry point
                      * are useless (wasted) for the
                      * INIT process, doesn't matter
                      */
                  (k_base + k_limit) >> LIMIT_4K_SHIFT,
                  DA_32 | DA_LIMIT_4K | DA_C | privilege << 5);

            init_descriptor(&p->ldts[INDEX_LDT_RW],
                  0, /* bytes before the entry point
                      * are useless (wasted) for the
                      * INIT process, doesn't matter
                      */
                  (k_base + k_limit) >> LIMIT_4K_SHIFT,
                  DA_32 | DA_LIMIT_4K | DA_DRW | privilege << 5);
        }

        p->regs.cs = INDEX_LDT_C << 3 | SA_TIL | rpl;
        p->regs.ds =
        p->regs.es =
        p->regs.fs =
        p->regs.ss = INDEX_LDT_RW << 3 | SA_TIL | rpl;
        p->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;

        p->regs.eip = (u32)p_task->initial_eip;
        p->regs.esp = (u32)p_task_stack;
        p->regs.eflags = eflags;

        p->p_flags = 0;
        p->p_msg = 0;
        p->p_recvfrom = NO_TASK;
        p->p_sendto = NO_TASK;
        p->has_int_msg = 0;
        p->q_sending = 0;
        p->next_sending = 0;
        p->pid = i;
        p->p_suspended = 0;

        for (j = 0; j < NR_FILES; j++)
            p->filp[j] = 0;

        p->ticks = p->priority = prio;

        p_task_stack -= p_task->stacksize;
        p++;
        p_task++;
    }
    k_reenter = 0;
    ticks = 0;
    p_proc_ready = proc_table;
    clearScreen();
    init_clock();
    init_keyboard();
    restart();
    while(1){}
}

/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
    MESSAGE msg;
    reset_msg(&msg);
    msg.type = GET_TICKS;
    send_recv(BOTH, TASK_SYS, &msg);
    return msg.RETVAL;
}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
    int i;
    char buf[256];

    /* 4 is the size of fmt in the stack */
    va_list arg = (va_list)((char*)&fmt + 4);

    i = vsprintf(buf, fmt, arg);

    printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

    /* should never arrive here */
    __asm__ __volatile__("ud2");
}

/*****************************************************************************
 *                                HELPALL
 *****************************************************************************/
void HELPALL()
{
    printf("======================================\n");
    printf("======14th_Group's OS help Screen=====\n");
    printf("======================================\n");
    printf("Type these to view specific help:     \n");
    printf("                                      \n");
    printf("1. help-proess         :Proess Manage \n");
    printf("2. help-file           :File Manage   \n");
    printf("3. help-game           :Run Game!     \n");
    printf("4. help-others         :Other Help    \n");
    printf("======================================\n");
}

/*****************************************************************************
 *                                help
 *****************************************************************************/
void help(int Choice)
{
    clearScreen();
    switch (Choice)
    {
    case 1:
            printf("======================================\n");
            printf("======14th_Group's OS help Screen=====\n");
            printf("=1st:Process Manage===================\n");
            printf("1.'proc-show':Get current process list\n");
            printf("2.'proc-suspend [pid]':suspend process\n");
            printf("3.'proc-regain [pid]': regain process \n");
            printf("4.'proc-stop [pid]':   stop process   \n");
            printf("======================================\n");
        break;
    case 2:
            printf("======================================\n");
            printf("======14th_Group's OS help Screen=====\n");
            printf("=2nd:File Manage======================\n");
            printf("1.'cd ...':     Go to the Catalog     \n");
            printf("2.'file-show':  List all current files\n");
            printf("3.'mkdir ...':  Create Catalog        \n");
            printf("4.'create ...': Create New Files      \n");
            printf("5.'rename ...': Rename Files          \n");
            printf("6.'delete ...': Delete FilesOrCatalog \n");
            printf("7.'read ...':   Read Files            \n");
            printf("8.'write ...':  Write Files           \n");
            printf("======================================\n");
        break;
    case 3:
            printf("======================================\n");
            printf("======14th_Group's OS help Screen=====\n");
            printf("=3rd:Run Game!========================\n");
            printf("1.'game-snaker': Run Retro Snaker Game\n");
            printf("2.'game-tjchess':Run TiaoJiaChess Game\n");
            printf("3.'game-jop':    Run Plane Wars Game  \n");
            printf("4.'game-24':     Run 24 Point Game    \n");
            printf("5.'game-cal':    Run Calculator       \n");
            printf("6.'game-gobang': Run GoBang Game      \n");
            printf("======================================\n");
        break;
    case 4:
            printf("======================================\n");
            printf("======14th_Group's OS help Screen=====\n");
            printf("=4th:Other Help=======================\n");
            printf("1.'clear':                            \n");
            printf("                Clear console contents\n");
            printf("======================================\n");
        break;
    default:
        break;
    }
}
/*======================================================================*
                               clear
 *======================================================================*/
void clear()
{
    clearScreen();
    console_table[current_console].crtc_start = 0;
    console_table[current_console].cursor = 0;
}
/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
    char tty_name[] = "/dev_tty0";
    char buffer[256];
    char _input[20];char _input_part1[128];char _input_part2[128];
    char current_dirr[512] = "/";  // 记录当前路径（其实路径字符长度上限为MAX_PATH）
    int fd_stdin  = open(tty_name, O_RDWR);
    assert(fd_stdin  == 0);
    int fd_stdout = open(tty_name, O_RDWR);
    assert(fd_stdout == 1);

    while (1) {
        printf("@14th_Group's OS machine: %s", current_dirr);  // 打印当前路径        
        int r = read(fd_stdin, buffer, 512);
        buffer[r] = 0;
        // 解析命令
        int pos = 0;
        while (buffer[pos] != ' ' && buffer[pos] != 0)  // 读取指令
        {
            _input[pos] = buffer[pos];
            pos++;
        }
        _input[pos] = 0;
        if (buffer[pos] != 0)  // 指令还未结束
        {
            pos++;
            int len = pos;
            while (buffer[pos] != ' ' && buffer[pos] != 0)  // 读取第一个文件名
            {
                _input_part1[pos - len] = buffer[pos];
                pos++;
            }
            _input_part1[pos - len] = 0;
        }
        if (buffer[pos] != 0)  // 指令还未结束
        {
            pos++;
            int len = pos;
            while (buffer[pos] != ' ' && buffer[pos] != 0)  // 读取第二个文件名
            {
                _input_part2[pos - len] = buffer[pos];
                pos++;
            }
            _input_part2[pos - len] = 0;
        }
        if (strcmp(_input, "help") == 0)
        {
            HELPALL();
        }
        else if (strcmp(_input, "help-process") == 0)
        {
            help(1);
        }
        else if (strcmp(_input, "help-file") == 0)
        {
            help(2);
        }
        else if (strcmp(_input, "help-game") == 0)
        {
            help(3);
        }
        else if (strcmp(_input, "help-others") == 0)
        {
            help(4);
        }
        else if (strcmp(_input, "proc-show") == 0)
        {
            process_show();
        }
        else if (strcmp(_input, "proc-suspend") == 0)
        {
            char *p = _input_part1;
            process_suspend(p);
        }
        else if (strcmp(_input, "proc-regain") == 0)
        {
            char *p = _input_part1;
            process_regain(p);
        }
        else if (strcmp(_input, "proc-stop") == 0)
        {
            char *p = _input_part1;
            process_stop(p);
        }
        else if (strcmp(_input, "file-show") == 0)
        {
            ShowFile(current_dirr);
        }
        else if (strcmp(_input, "create") == 0)  // 创建文件
        {
            CreateFile(current_dirr, _input_part1);
        }
        else if (strcmp(_input, "delete") == 0)  // 删除文件
        {
            DeleteFile(current_dirr, _input_part1);
        }
        else if (strcmp(_input, "read") == 0)  // 打印文件内容
        {
            ReadFile(current_dirr, _input_part1);
        }
        else if (strcmp(_input, "write") == 0)  // 重写文件
        {
            WriteFile(current_dirr, _input_part1);
        }
        else if(strcmp(_input, "rename") == 0) // 重命名文件
        {           
	        RenameFile(current_dirr, _input_part1, _input_part2);
        }
        else if(strcmp(_input, "mkdir") == 0)  // 创建目录
        {
            CreateDir(current_dirr, _input_part1);
        }
        else if(strcmp(_input, "cd") == 0)
        {
            EnterDir(current_dirr, _input_part1);
        }
        else if (strcmp(_input, "game-snaker") == 0)
        {
            GOSNAKER();
        }
        else if (strcmp(_input, "game-tjchess") == 0)
        {
            GOCHESS(fd_stdin,fd_stdout);
        }
        else if (strcmp(_input, "game-jop") == 0)
        {
            GOJOP();
        }
        else if (strcmp(_input, "game-24") == 0)
        {
            GOPOINTS();
        }
        else if (strcmp(_input, "game-cal") == 0)
        {
            calculate();
        }
        else if (strcmp(_input, "game-gobang") == 0)
        {
            fiveInaRow();
        }
        else if (strcmp(_input, "clear") == 0)
        {
            clear();
        }
        else
        {
            printf("Command not found\n");
            printf("Try to type 'help' to view All command.\n");
        }
    }
}

/*======================================================================*
                               TestB
 *======================================================================*/
void TestB()
{
    char tty_name[] = "/dev_tty1";
    char BUFFER[128];
    int fd_stdin  = open(tty_name, O_RDWR);
    assert(fd_stdin  == 0);
    int fd_stdout = open(tty_name, O_RDWR);
    assert(fd_stdout == 1);
    while (1) {
        boot_animation();//开机动画
        printf("======================================\n");
        printf("                Welcome!              \n");
        printf("         This is 14th Group OS!       \n");
        printf("      Type 'help' to view command     \n");
        printf("                Enjoy it!             \n");
        printf("======================================\n");
        printf("@14th_Group's OS machine:");
        int _b = read(fd_stdin, BUFFER, 70);
        BUFFER[_b] = 0;
    }
}
/*======================================================================*
                               TestC
 *======================================================================*/
void TestC()
{
    for(;;);
}
/*======================================================================*
                               TestD
 *======================================================================*/
void TestD()
{
	for(;;);
}
