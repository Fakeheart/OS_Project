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
 *                                merge_path
 *                      将传入的路径和文件名组合成一个完整的绝对路径
 *****************************************************************************/
PUBLIC void merge_path(char* dest, char* path, char* file)
{
    int i=0, j=0;
    while (path[i] != 0)  // 写入路径
    {
        dest[j] = path[i];
        j++;
        i++;
    }
    i = 0;
    while (file[i] != 0)  // 写入文件名
    {
        dest[j] = file[i];
        j++;
        i++;
    }
    dest[j] = 0;  // 结束符
}
void CreateFile(char* path, char* file)
{
    char absoPath[512];
    merge_path(absoPath, path, file);
    int fd = open(absoPath, O_CREAT | O_RDWR);
    if (fd == -1)
    {
        printf("Failed to create a new file with name %s\n", file);
        return;
    }

    char buf[1] = {0};
    write(fd, buf, 1);
    printf("File created: %s (fd %d)\n", file, fd);
    close(fd);
}

void DeleteFile(char* path, char* file)
{
    char absoPath[512];
    merge_path(absoPath, path, file);
    int m=unlink(absoPath);
    if (m == 0)
        printf("%s deleted!\n", file);
    else
        printf("Failed to delete %s!\n", file);
}

void ReadFile(char* path, char* file)
{
    char absoPath[512];
    merge_path(absoPath, path, file);
    int fd = open(absoPath, O_RDWR);
    if (fd == -1)
    {
        printf("Failed to open %s!\n", file);
        return;
    }

    char buf[4096];
    int n = read(fd, buf, 4096);
    if (n == -1)  // 读取文件内容失败
    {
        printf("An error has occured in reading the file!\n");
        close(fd);
        return;
    }

    printf("%s\n", buf);
    close(fd);
}

void WriteFile(char* path, char* file)
{
    char absoPath[512];
    merge_path(absoPath, path, file);
    int fd = open(absoPath, O_RDWR);
    if (fd == -1)
    {
        printf("Failed to open %s!\n", file);
        return;
    }

    char tty_name[] = "/dev_tty0";
    int fd_stdin  = open(tty_name, O_RDWR);
    if (fd_stdin == -1)
    {
        printf("An error has occured in writing the file!\n");
        return;
    }
    char writeBuf[4096];  // 写缓冲区
    int endPos = read(fd_stdin, writeBuf, 4096);
    writeBuf[endPos] = 0;
    write(fd, writeBuf, endPos + 1);  // 结束符也应写入
    close(fd);
}

void RenameFile(char* path, char* file1, char* file2)
{
    char absoPath[512];
    merge_path(absoPath, path, file1);
    int fd = rename(absoPath, file2);
    if(fd == -1)
    {
        printf("No such file or directory\n");
    }
    else
    {
        printf("File %s renames to: %s\n", file1, file2);
    }
}

void CreateDir(char* path, char* file)
{
    char absoPath[512];
    merge_path(absoPath, path, file);
    int fd = open(absoPath, O_RDWR);
    if (fd != -1)
    {
        printf("Failed to create a new directory with name %s\n", file);
        return;
    }
    mkdir(absoPath);
}

void EnterDir(char* path, char* file)
{
    int flag = 0;  // 判断是进入下一级目录还是返回上一级目录
    char newPath[512] = {0};
    if (file[0] == '.' && file[1] == '.')  // cd ..返回上一级目录
    {
        flag = 1;
        int pos_path = 0;
        int pos_new = 0;
        int i = 0;
        char temp[128] = {0};  // 用于存放某一级目录的名称
        while (path[pos_path] != 0)
        {
            if (path[pos_path] == '/')
            {
                pos_path++;
                if (path[pos_path] == 0)  // 已到达结尾
                    break;
                else
                {
                    temp[i] = '/';
                    temp[i + 1] = 0;
                    i = 0;
                    while (temp[i] != 0)
                    {
                        newPath[pos_new] = temp[i];
                        temp[i] = 0;  // 抹掉
                        pos_new++;
                        i++;
                    }
                    i = 0;
                }
            }
            else
            {
                temp[i] = path[pos_path];
                i++;
                pos_path++;
            }
        }
    }
    char absoPath[512];
    char temp[512];
    int pos = 0;
    while (file[pos] != 0)
    {
        temp[pos] = file[pos];
        pos++;
    }
    temp[pos] = '/';
    temp[pos + 1] = 0;
    if (flag == 1)  // 返回上一级目录
    {
        temp[0] = 0;
        merge_path(absoPath, newPath, temp);
    }
    else  // 进入下一级目录
        merge_path(absoPath, path, temp);
    int fd = open(absoPath, O_RDWR);
    if (fd == -1)
        printf("%s is not a directory!\n", absoPath);
    else
        memcpy(path, absoPath, 512);
}