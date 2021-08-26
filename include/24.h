#include "stdio.h"
#include "time.h"
#include "DrawingTools.h"
#include "keyboard.h"

/*        常量声明         */
#define MAPWIDTH_24 14	 //宽度
#define MAPHEIGHT_24 30	 //高度
#define OFFSETPIXEL_24 5 //偏移块数

/*        函数声明         */
PUBLIC void InitMap_24();	 //初始化贪吃蛇地图
PUBLIC void InitOthers_24(); //对地图外的其余元素进行初始化
PUBLIC void MOVE_24();		 //对移动进行判断
PUBLIC void INPUT_24();		 //输入判断
PUBLIC char *iitoa_24(int num, char *str, int radix);

/*        全局变量声明         */
char option = 'y';	 //当前所移动的方向
int gameover_24 = 0; //游戏结束标志
int exitflag_24 = 0; //游戏结束标志
int score_24 = 0;	 //当前获得的总分

char op[5] = {
	'#',
	'+',
	'-',
	'*',
	'/',
};
float cal(float x, float y, int op)
{
	switch (op) //将运算符初始化为1,2,3,4四个数字
	{
	case 1:
		return x + y;
	case 2:
		return x - y;
	case 3:
		return x * y;
	case 4:
		return x / y;
	}
}
//对应表达式((AoB)oC)oD
float m1(float i, float j, float k, float t, int op1, int op2, int op3)
{
	float r1, r2, r3;
	r1 = cal(i, j, op1);
	r2 = cal(r1, k, op2);
	r3 = cal(r2, t, op3);
	return r3;
}
//对应表达式(Ao(BoC))oD
float m2(float i, float j, float k, float t, int op1, int op2, int op3)
{
	float r1, r2, r3;
	r1 = cal(j, k, op2);
	r2 = cal(i, r1, op1);
	r3 = cal(r2, t, op3);
	return r3;
}
//对应表达式Ao(Bo(CoD))
float m3(float i, float j, float k, float t, int op1, int op2, int op3)
{
	float r1, r2, r3;
	r1 = cal(k, t, op3);
	r2 = cal(j, r1, op2);
	r3 = cal(i, r2, op1);
	return r3;
}
//对应表达式Ao((BoC)oD)
float m4(float i, float j, float k, float t, int op1, int op2, int op3)
{
	float r1, r2, r3;
	r1 = cal(j, k, op2);
	r2 = cal(r1, t, op3);
	r3 = cal(i, r2, op1);
	return r3;
}
//对应表达式(AoB)o(CoD)
float m5(float i, float j, float k, float t, int op1, int op2, int op3)
{
	float r1, r2, r3;
	r1 = cal(i, j, op1);
	r2 = cal(k, t, op3);
	r3 = cal(r1, r2, op2);
	return r3;
}
int get24(int i, int j, int k, int t)
{
	int op1, op2, op3;
	int flag = 0;
	for (op1 = 1; op1 <= 4; op1++)
	{ //通过一个三重循坏列举了符号的变化情况
		for (op2 = 1; op2 <= 4; op2++)
		{
			for (op3 = 1; op3 <= 4; op3++)
			{
				if (m1(i, j, k, t, op1, op2, op3) == 24)
				{

					flag = 1;
				}
				if (m2(i, j, k, t, op1, op2, op3) == 24)
				{

					flag = 1;
				}
				if (m3(i, j, k, t, op1, op2, op3) == 24)
				{

					flag = 1;
				}
				if (m4(i, j, k, t, op1, op2, op3) == 24)
				{

					flag = 1;
				}
				if (m5(i, j, k, t, op1, op2, op3) == 24)
				{

					flag = 1;
				}
			}
		}
	}
	return flag;
}

void UPDATESCORE_24() //50*80
{
	char *_score;
	iitoa_24(score_24, _score, 10);
	vm_op_rectangle(270, 10, 320, 26, 15);
	vm_print_letter(270, 10, 7, _score);
}
PUBLIC void InitMap_24()
{
	clearScreen(); //清_24InitOthers_24屏

	/*                   主地图                         */
	for (int x = OFFSETPIXEL_24 - 1; x < MAPWIDTH_24 + OFFSETPIXEL_24 + 1; x++)
	{
		for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
			vm_op_rec_64px(x, y, 200);
	}
	/*                   边界部分                        */
	for (int x = OFFSETPIXEL_24; x < MAPWIDTH_24 + OFFSETPIXEL_24; x++)
		vm_op_rec_64px(x, MAPHEIGHT_24 + OFFSETPIXEL_24 - 1, 70);

	for (int x = OFFSETPIXEL_24; x < MAPWIDTH_24 + OFFSETPIXEL_24; x++)
		vm_op_rec_64px(x, OFFSETPIXEL_24 + 1, 70);

	for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
		vm_op_rec_64px(MAPWIDTH_24 + OFFSETPIXEL_24, y, 70);

	for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
		vm_op_rec_64px(OFFSETPIXEL_24 - 1, y, 70);

	vm_op_rec_64px(MAPWIDTH_24 + OFFSETPIXEL_24 - 1, MAPHEIGHT_24 + OFFSETPIXEL_24 - 1, 70); //边角
}

PUBLIC void InitOthers_24()
{

	vm_print_letter(70, 10, 13, "24points Games!");
	vm_print_letter(220, 10, 7, "Score:");
	vm_print_letter(270, 10, 7, "0");
	vm_print_letter(50, 170, 7, "if numbers can be 24[y],or[n]");
	vm_print_letter(100, 186, 7, "EXIT[Enter]");
}

PUBLIC void INPUT_24()
{
	u32 key = GETINPUT();
	if (!(key & FLAG_EXT)) //表示当前字符是一个可打印字符；
	{
		switch (key)
		{
		case 'y':
		case 'Y':

			option = 'y';

			break;
		case 'n':
		case 'N':

			option = 'n';

			break;
		}
	}
	else
	{
		int raw_code = key & MASK_RAW;
		switch (raw_code)
		{

		case ENTER:
			gameover_24 = 1;
			exitflag_24 = 1;
			break;
		}
	}
	return;
}
PUBLIC char *iitoa_24(int num, char *str, int radix)
{
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //索引表
	unsigned unum;										   //存放要转换的整数的绝对值,转换的整数可能是负数
	int i = 0, j, k;									   //i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。

	//获取要转换的整数的绝对值
	if (radix == 10 && num < 0) //要转换成十进制数并且是负数
	{
		unum = (unsigned)-num; //将num的绝对值赋给unum
		str[i++] = '-';		   //在字符串最前面设置为'-'号，并且索引加1
	}
	else
		unum = (unsigned)num; //若是num为正，直接赋值给unum

	//转换部分，注意转换后是逆序的
	do
	{
		str[i++] = index[unum % (unsigned)radix]; //取unum的最后一位，并设置为str对应位，指示索引加1
		unum /= radix;							  //unum去掉最后一位

	} while (unum); //直至unum为0退出循环

	str[i] = '\0'; //在字符串最后添加'\0'字符，c语言字符串以'\0'结束。

	//将顺序调整过来
	if (str[0] == '-')
		k = 1; //如果是负数，符号不用调整，从符号后面开始调整
	else
		k = 0; //不是负数，全部都要调整

	char temp;						   //临时变量，交换两个值时用到
	for (j = k; j <= (i - 1) / 2; j++) //头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
	{
		temp = str[j];				 //头部赋值给临时变量
		str[j] = str[i - 1 + k - j]; //尾部赋值给头部
		str[i - 1 + k - j] = temp;	 //将临时变量的值(其实就是之前的头部值)赋给尾部
	}

	return str; //返回转换后的字符串
}
PUBLIC void Drawnum(int num, int x, int y)
{
	/*
if(num==1){
	for(int i=0;i<5;i++){

vm_op_rec_64px(x-2+i, y+3, 100);
	}
	
}
else if(num==2){
	for(int i=0;i<3;i++){
vm_op_rec_64px(x-2, y+1+i, 100);
	}
		for(int i=0;i<3;i++){
vm_op_rec_64px(x, y+1+i, 100);
	}
		for(int i=0;i<3;i++){
vm_op_rec_64px(x+2, y+1+i, 100);
	}
vm_op_rec_64px(x-1, y+3, 100);
vm_op_rec_64px(x+1, y+1, 100);	
}
else if(num==3){
	for(int i=0;i<3;i++){
vm_op_rec_64px(x-2, y+1+i, 100);
	}
		for(int i=0;i<3;i++){
vm_op_rec_64px(x, y+1+i, 100);
	}
		for(int i=0;i<3;i++){
vm_op_rec_64px(x+2, y+1+i, 100);
	}
vm_op_rec_64px(x-1, y+3, 100);
vm_op_rec_64px(x+1, y+3, 100);	
}
else if(num==4){
	for(int i=0;i<5;i++){

vm_op_rec_64px(x-2+i, y+3, 100);
	}
for(int i=0;i<3;i++){
vm_op_rec_64px(x-2+i, y+1, 100);	
}
vm_op_rec_64px(x, y+2, 100);	

}
*/

	if (num == 1)
	{
		vm_print_letter(x, y, 7, "1");
	}
	else if (num == 2)
	{
		vm_print_letter(x, y, 7, "2");
	}
	else if (num == 3)
	{
		vm_print_letter(x, y, 7, "3");
	}
	else if (num == 4)
	{
		vm_print_letter(x, y, 7, "4");
	}
	else if (num == 5)
	{
		vm_print_letter(x, y, 7, "5");
	}
	else if (num == 6)
	{
		vm_print_letter(x, y, 7, "6");
	}
	else if (num == 7)
	{
		vm_print_letter(x, y, 7, "7");
	}
	else if (num == 8)
	{
		vm_print_letter(x, y, 7, "8");
	}
	else if (num == 9)
	{
		vm_print_letter(x, y, 7, "9");
	}
	else if (num == 10)
	{
		vm_print_letter(x, y, 7, "10");
	}
	else if (num == 11)
	{
		vm_print_letter(x, y, 7, "11");
	}
	else if (num == 12)
	{
		vm_print_letter(x, y, 7, "12");
	}
	else if (num == 13)
	{
		vm_print_letter(x, y, 7, "13");
	}
	return;
}

PUBLIC void GOPOINTS()
{
	clear(); //清屏
	score_24 = 0;
	gameover_24 = 0;
	exitflag_24 = 0;
	InitMap_24();	 //对地图进行初始化
	InitOthers_24(); //字符初始化
	//游戏主循环
	while (1)
	{
		while (!gameover_24)
		{

           /*                   主地图                         */
	for (int x = OFFSETPIXEL_24 - 1; x < MAPWIDTH_24 + OFFSETPIXEL_24 + 1; x++)
	{
		for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
			vm_op_rec_64px(x, y, 200);
	}
	/*                   边界部分                        */
	for (int x = OFFSETPIXEL_24; x < MAPWIDTH_24 + OFFSETPIXEL_24; x++)
		vm_op_rec_64px(x, MAPHEIGHT_24 + OFFSETPIXEL_24 - 1, 70);

	for (int x = OFFSETPIXEL_24; x < MAPWIDTH_24 + OFFSETPIXEL_24; x++)
		vm_op_rec_64px(x, OFFSETPIXEL_24 + 1, 70);

	for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
		vm_op_rec_64px(MAPWIDTH_24 + OFFSETPIXEL_24, y, 70);

	for (int y = OFFSETPIXEL_24 + 1; y < MAPHEIGHT_24 + OFFSETPIXEL_24; y++)
		vm_op_rec_64px(OFFSETPIXEL_24 - 1, y, 70);

	vm_op_rec_64px(MAPWIDTH_24 + OFFSETPIXEL_24 - 1, MAPHEIGHT_24 + OFFSETPIXEL_24 - 1, 70); //边角
           
			int i, j, k, t;
			srand(TTime());
			i = 1 + (rand() % 13);
			j = 1 + (rand() % 13);
			k = 1 + (rand() % 13);
			t = 1 + (rand() % 13);
			vm_op_rectangle(75, 50, 320, 40, 14);
			vm_print_letter(75, 50, 7, "A=");
			Drawnum(i, 100, 50);
			vm_print_letter(175, 50, 7, "B=");
			Drawnum(j, 200, 50);
			vm_print_letter(75, 100, 7, "C=");
			Drawnum(k, 100, 100);
			vm_print_letter(175, 100, 7, "D=");
			Drawnum(t, 200, 100);

			/*
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("A= %d\n", i);
			printf("B= %d\n", j);
			printf("C= %d\n", k);
			printf("D= %d\n", t);
*/
			INPUT_24(); //处理输入
			/*
			char *o;
			vm_print_letter(100, 50, 7, "option=");
			iitoa_24(option, o, 10);
			vm_op_rectangle(200, 50, 320, 40, 14);
			vm_print_letter(200, 50, 7, o);
			*/
			if (get24(i, j, k, t))
			{
				if (option == 'y')
				{
					score_24 += 100;
				}
				else
				{
					score_24 -= 200;
				}
			}
			else
			{
				if (option == 'y')
				{
					score_24 -= 200;
				}
				else
				{
					score_24 += 100;
				}
			}
			UPDATESCORE_24();
			if (score_24 < 0)
			{
				gameover_24 -= 1;
			}
			milli_delay(1000);
		}
		if (exitflag_24 == 1)
		{
			break;
		}

		vm_print_letter(90, 80, 7, "YOU LOSE");
		vm_print_letter(90, 100, 7, "RESTART?[y/n]");
		u32 key = GETINPUT();
		if (key == 'y' || key == 'Y')
		{
			gameover_24 = 0;
			score_24 = 0;
			clear();		 //清屏
			InitMap_24();	 //对地图进行初始化
			InitOthers_24(); //字符初始化
		}
		else
		{
			break;
		}
	}

	clear();
}
