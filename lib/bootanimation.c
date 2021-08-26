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
PUBLIC void boot_animation()
{
	char* p = (char *) 0xa0000;
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 320; ++x) {
            p[y*320 + x] = 15;//灰色 
        }
    }
    milli_delay(10000);
    vm_print_letter(15,5,0," ***     ***         ******* ******");
    milli_delay(10000);
    vm_print_letter(15,21,0,"****   *****         **   ** *     ");
    milli_delay(10000);
    vm_print_letter(15,37,0," ***  **  **    ---  **   ** ******");
    milli_delay(10000);
    vm_print_letter(15,53,0," *** *** *** **      **   **     **");
    milli_delay(10000);
    vm_print_letter(15,69,0," ***      **         ******* ******");
    milli_delay(10000);
    vm_print_letter(30,100,0,"Created By:");
    vm_print_letter(60,116,0,"1851634 Zhang XiangLong");
    vm_print_letter(60,132,0,"1754025 Xu HanZhi");
    vm_print_letter(60,148,0,"1853243 Nan WenYing");
    vm_print_letter(60,164,0,"1853435 Xu ChenYang");
    vm_print_letter(60,180,0,"1853410 Dong EnQi");
    milli_delay(30000);
    clear();
    for (int y = 0; y < 200; ++y) {
        for (int x = 0; x < 320; ++x) {
            p[y*320 + x] = 15;
        }
    }
}
