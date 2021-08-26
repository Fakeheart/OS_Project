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

void process_show()
{
    char* status;
    printf("=14th_Group's OS Process List=========\n");
    printf("======================================\n");
    printf("    PID    |    name    |    Status   \n");
    printf("======================================\n");
    for (int i = 0 ; i < NR_TASKS + NR_NATIVE_PROCS ; ++i )
    {
        if(proc_table[i].p_flags!=FREE_SLOT)
        {
            if(proc_table[i].p_suspended==0)
            {
                printf("  %d         %s         %s \n", proc_table[i].pid, proc_table[i].name,"running");
            }
            else 
            {
                printf("  %d         %s         %s \n", proc_table[i].pid, proc_table[i].name,"suspend");
            }
        }  
    }
    printf("======================================\n");
    return;
}
void process_suspend(char* p)
{
    int index=*p-48;
    if(index <= 0|| index > NR_TASKS + NR_NATIVE_PROCS)
    {
        printf("Invalid index!\n");
    }
    else if(index < NR_TASKS)
    {
        printf("Access Denied!\n");
    }
    else
    {
        proc_table[index].p_suspended=1;
        printf("Process %s has suspended!\n",proc_table[index].name);
    }
}
void process_regain(char* p)
{
    int index=*p-48;
    if(index <= 0|| index > NR_TASKS + NR_NATIVE_PROCS)
    {
        printf("Invalid index!\n");
    }
    else if(index < NR_TASKS)
    {
        printf("Access Denied!\n");
    }
    else
    {
        if(proc_table[index].p_suspended==0)
        {
            printf("Process %s is already running\n",proc_table[index].name);
        }
        else
        {
            proc_table[index].p_suspended=0;
            printf("Process %s is running now!\n",proc_table[index].name);
        }
    }
}
void process_stop(char* p)
{
    int index=*p-48;
    if(index <= 0|| index > NR_TASKS + NR_NATIVE_PROCS)
    {
        printf("Invalid index!\n");
    }
    else if(index < NR_TASKS)
    {
        printf("Access Denied!\n");
    }
    else
    {
        proc_table[index].p_flags=FREE_SLOT;
        printf("Process %s is already stopped\n",proc_table[index].name);
    }
}