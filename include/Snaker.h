#include "stdio.h"
#include "time.h"
#include "DrawingTools.h"
#include "keyboard.h"

/*        常量声明         */
#define SNAKESIZE 50  //蛇的身体最大节数
#define MAPWIDTH 24	  //宽度
#define MAPHEIGHT 39  //高度
#define OFFSETPIXEL 8 //偏移块数

/*        函数声明         */
PUBLIC void InitMap();	  //初始化贪吃蛇地图
PUBLIC void InitOthers(); //对地图外的其余元素进行初始化
PUBLIC void MOVE();		  //对移动进行判断
PUBLIC void INPUT();	  //输入判断
PUBLIC void BODERJUDGE(); //边界判断
PUBLIC char *iitoa(int num, char *str, int radix);

/*        全局变量声明         */
char DIRECTION = 'w'; //当前所移动的方向
int GAMEOVER = 0;	  //游戏结束标志
int EXITFLAG = 0;	  //游戏结束标志
int changeFlag = 0;	  //是否迟到了食物
int TOTALSCORE = 0;	  //当前获得的总分

//蛇的相关属性
struct
{
	int len;		  //蛇的长度
	int x[SNAKESIZE]; //组成蛇身的每一个小方块中x的坐标
	int y[SNAKESIZE]; //组成蛇身的每一个小方块中y的坐标

} snake;

//食物相关属性
struct
{
	int x;
	int y;
} food;

PUBLIC void GOSNAKER()
{
	clear(); //清屏
	TOTALSCORE = 0;
	GAMEOVER = 0;
	EXITFLAG = 0;
	InitMap();	  //对地图进行初始化
	InitOthers(); //字符初始化
	//游戏主循环
	while (1)
	{
		while (!GAMEOVER)
		{
			INPUT();	   //处理输入
			MOVE();		   //对蛇的移动进行处理
			BODERJUDGE();  //边界判定
			UPDATESCORE(); //对分数进行更新
		}
		if (EXITFLAG == 1) //退出标志
		{
			break;
		}
		vm_print_letter(90, 80, 7, "YOU LOSE");
		vm_print_letter(90, 100, 7, "RESTART?[y/n]");
		u32 key = GETINPUT();
		if (key == 'y' || key == 'Y')
		{
			GAMEOVER = 0;
			TOTALSCORE = 0;
			InitMap();	  //对地图进行初始化
			InitOthers(); //字符初始化
		}
		else
		{
			break;
		}
	}
	clearScreen();
}

void UPDATESCORE() //50*80
{
	char *_score;
	iitoa(TOTALSCORE, _score, 10);
	vm_op_rectangle(270, 10, 320, 40, 14);
	vm_print_letter(270, 10, 7, _score);
}

PUBLIC void InitMap()
{
	boxfill8(SCREEN_WIDTH, 14, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //清屏

	/*                   主地图                         */
	for (int x = OFFSETPIXEL - 1; x < MAPWIDTH / 2 + OFFSETPIXEL + 1; x++)
	{
		for (int y = OFFSETPIXEL + 1; y < MAPHEIGHT / 2 + OFFSETPIXEL; y++)
			vm_op_rec_64px(x, y, 6);
	}
	/*                   边界部分                        */
	for (int x = OFFSETPIXEL; x < MAPWIDTH / 2 + OFFSETPIXEL; x++)
		vm_op_rec_64px(x, MAPHEIGHT / 2 + OFFSETPIXEL - 1, 15);

	for (int x = OFFSETPIXEL; x < MAPWIDTH / 2 + OFFSETPIXEL; x++)
		vm_op_rec_64px(x, OFFSETPIXEL + 1, 15);

	for (int y = OFFSETPIXEL + 1; y < MAPHEIGHT / 2 + OFFSETPIXEL; y++)
		vm_op_rec_64px(MAPWIDTH / 2 + OFFSETPIXEL, y, 15);

	for (int y = OFFSETPIXEL + 1; y < MAPHEIGHT / 2 + OFFSETPIXEL; y++)
		vm_op_rec_64px(OFFSETPIXEL - 1, y, 15);

	vm_op_rec_64px(MAPWIDTH / 2 + OFFSETPIXEL - 1, MAPHEIGHT / 2 + OFFSETPIXEL - 1, 15); //边角

	/*                   食物初始化                        */
	while (1)
	{
		/*生成的食物横坐标的奇偶必须和初始时蛇头所在坐标的奇偶一致
		，若不一致会导致吃食物的时候只吃到一半*/
		srand(TTime());
		food.x = OFFSETPIXEL + rand() % (MAPWIDTH / 2 - 1);
		food.y = OFFSETPIXEL + 2 + rand() % (MAPHEIGHT / 2 - 3);
		if (food.x % 2 == 0)
			break;
	}
	vm_op_rec_64px(food.x, food.y, 3); //打印食物

	/*                   蛇初始化                         */
	snake.len = 3;			   //蛇的长度
	snake.x[0] = MAPWIDTH / 2; //x坐标为偶数
	snake.y[0] = MAPHEIGHT / 2;
	vm_op_rec_64px(snake.x[0], snake.y[0], 7); //打印蛇头
	for (int i = 1; i < snake.len; i++)
	{
		//蛇身的打印，纵坐标不变，横坐标为上一节蛇身的坐标值+1
		snake.x[i] = snake.x[i - 1] + 1;
		snake.y[i] = snake.y[i - 1];
		vm_op_rec_64px(snake.x[i], snake.y[i], 7);
	}
}

PUBLIC void InitOthers()
{
	vm_print_letter(40, 10, 13, "EASY Retro Snaker!");
	vm_print_letter(220, 10, 7, "Score:");
	vm_print_letter(270, 10, 7, "0");
	vm_print_letter(50, 170, 7, "UP[W],DOWN[S],LEFT[A],RIGHT[D]");
	vm_print_letter(100, 186, 7, "EXIT[Enter]");
}

PUBLIC void MOVE()
{
	/*
	*蛇移动时候先擦去蛇尾的一节
	*changeFlag为0表明此时没有吃到食物，因此每走一步就要擦除掉蛇尾，以此营造一个移动的效果
	*为1表明吃到了食物，就不需要擦除蛇尾，以此营造一个蛇身增长的效果
	*/
	if (changeFlag == 0)
	{
		vm_op_rec_64px(snake.x[snake.len - 1], snake.y[snake.len - 1], 6);
	}

	//将蛇的每一节依次向前移动一节（蛇头除外）
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}

	//判断蛇头应该往哪个方向移动
	switch (DIRECTION)
	{
	case 'a':
		snake.y[0] -= 1; //往左
		break;
	case 'd':
		snake.y[0] += 1; //往右
		break;
	case 'w':
		snake.x[0]--; //往上
		break;
	case 's':
		snake.x[0]++; //往下
		break;
	}
	//打印出蛇头
	vm_op_rec_64px(snake.x[0], snake.y[0], 7);
	//由于目前没有吃到食物，changFlag值为0

	changeFlag = 0;

	if (snake.x[0] == food.x && snake.y[0] == food.y) //蛇头碰到食物
	{
		//蛇头碰到食物即为要吃掉这个食物了，因此需要再次生成一个食物
		while (1)
		{
			int flag = 1;
			srand(TTime());

			food.x = OFFSETPIXEL + rand() % (MAPWIDTH / 2 - 1);
			food.y = OFFSETPIXEL + 2 + rand() % (MAPHEIGHT / 2 - 3);

			//随机生成的食物不能在蛇的身体上
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					flag = 0;
					break;
				}
			}
			//随机生成的食物不能横坐标为奇数，也不能在蛇身，否则重新生成
			if (flag && food.x % 2 == 0)
				break;
		}
		//绘制食物
		vm_op_rec_64px(food.x, food.y, 3);
		TOTALSCORE += 10;
		snake.len++;	//吃到食物，蛇身长度加1
		changeFlag = 1; //很重要，因为吃到了食物，就不用再擦除蛇尾的那一节，以此来造成蛇身体增长的效果
	}
	return;
}

PUBLIC void INPUT()
{
	u32 key = GETINPUT();
	if (!(key & FLAG_EXT)) //表示当前字符是一个可打印字符；
	{
		switch (key)
		{
		case 'a':
		case 'A':
			if (DIRECTION != 'd')
			{
				DIRECTION = 'a';
			}
			break;
		case 'w':
		case 'W':
			if (DIRECTION != 's')
			{
				DIRECTION = 'w';
			}
			break;
		case 'd':
		case 'D':
			if (DIRECTION != 'a')
			{
				DIRECTION = 'd';
			}
			break;
		case 's':
		case 'S':
			if (DIRECTION != 'w')
			{
				DIRECTION = 's';
			}
			break;
		}
	}
	else
	{
		int raw_code = key & MASK_RAW;
		switch (raw_code)
		{

		case ENTER:
			GAMEOVER = 1;
			EXITFLAG = 1;
			break;
		}
	}
	return;
}

PUBLIC void BODERJUDGE()
{

	//蛇头碰到上下边界，游戏结束
	if (snake.y[0] == OFFSETPIXEL + 1 || snake.y[0] == MAPHEIGHT / 2 + OFFSETPIXEL - 1)
		GAMEOVER = 1;
	//蛇头碰到左右边界，游戏结束
	if (snake.x[0] == OFFSETPIXEL - 1 || snake.x[0] == MAPWIDTH / 2 + OFFSETPIXEL)
		GAMEOVER = 1;
	//蛇头碰到蛇身，游戏结束
	for (int i = 1; i < snake.len; i++)
	{
		if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
			GAMEOVER = 1;
	}
}

PUBLIC char *iitoa(int num, char *str, int radix)
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