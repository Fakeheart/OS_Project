#include"stdio.h"
#include"DrawingTools.h"
#include"keyboard.h"

#define ROW 15//棋盘行数
#define COL 15//棋盘列数
#define BLACK 1
#define WHITE 2

char board1[ROW][COL] = { 0 };
char ret = 0;
int over = 0;

/*        函数声明         */
PUBLIC void initBoard();
PUBLIC void printBoard();
PUBLIC void PlayerMove(int player);
PUBLIC static int IsFull();
PUBLIC char isWin();

PUBLIC void fiveInaRow()
{
	clear();
	initBoard();

	// 下棋
	while (1)
	{
		if(over){over=0;break;}
		PlayerMove(0); //玩家a走
		ret = isWin();
		if (ret != ' ')
		{
			break;
		}
		boxfill8(SCREEN_WIDTH, 14, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	//清屏
		printBoard(); //打印棋盘

		PlayerMove(1); //玩家b走
		ret = isWin();
		if (ret != ' ')
		{
			break;
		}
		printBoard();  //打印棋盘
	}

	// 判断输赢或平局
	if (ret == 'p')
	{ 
		printBoard(); //打印棋盘
                vm_print_letter(90,0,7,"DRAW!"); 
	}
	else if (ret == BLACK)
	{
		printBoard(); //打印棋盘
		vm_print_letter(80,0,7,"PLAYER A WIN!"); 
	}
	else if (ret == WHITE)
	{
		printBoard();  //打印棋盘
		vm_print_letter(80,0,7,"PLAYER B WIN!"); 
	}
	clearScreen();
}

//初始化
PUBLIC void initBoard()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			board1[i][j] = '0';
		}
	}
	boxfill8(SCREEN_WIDTH, 14, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	//����

	vm_op_line(100, 40, 250, 40, 0);
	vm_op_line(100, 50, 250, 50, 0);
	vm_op_line(100, 60, 250, 60, 0);
	vm_op_line(100, 70, 250, 70, 0);
	vm_op_line(100, 80, 250, 80, 0);
	vm_op_line(100, 90, 250, 90, 0);
	vm_op_line(100, 100, 250, 100, 0);
	vm_op_line(100, 110, 250, 110, 0);
	vm_op_line(100, 120, 250, 120, 0);
	vm_op_line(100, 130, 250, 130, 0);
	vm_op_line(100, 140, 250, 140, 0);
	vm_op_line(100, 150, 250, 150, 0);
	vm_op_line(100, 160, 250, 160, 0);
	vm_op_line(100, 170, 250, 170, 0);
	vm_op_line(100, 180, 250, 180, 0);
	vm_op_line(100, 190, 250, 190, 0);

	vm_op_line(100, 40, 100, 190, 0);
	vm_op_line(110, 40, 110, 190, 0);
	vm_op_line(120, 40, 120, 190, 0);
	vm_op_line(130, 40, 130, 190, 0);
	vm_op_line(140, 40, 140, 190, 0);
	vm_op_line(150, 40, 150, 190, 0);
	vm_op_line(160, 40, 160, 190, 0);
	vm_op_line(170, 40, 170, 190, 0);
	vm_op_line(180, 40, 180, 190, 0);
	vm_op_line(190, 40, 190, 190, 0);
	vm_op_line(200, 40, 200, 190, 0);
	vm_op_line(210, 40, 210, 190, 0);
	vm_op_line(220, 40, 220, 190, 0);
	vm_op_line(230, 40, 230, 190, 0);
	vm_op_line(240, 40, 240, 190, 0);
	vm_op_line(250, 40, 250, 190, 0);
}

//打印棋盘 
PUBLIC void printBoard()
{
	boxfill8(SCREEN_WIDTH, 14, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	//����

	vm_op_line(100, 40, 250, 40, 0);
	vm_op_line(100, 50, 250, 50, 0);
	vm_op_line(100, 60, 250, 60, 0);
	vm_op_line(100, 70, 250, 70, 0);
	vm_op_line(100, 80, 250, 80, 0);
	vm_op_line(100, 90, 250, 90, 0);
	vm_op_line(100, 100, 250, 100, 0);
	vm_op_line(100, 110, 250, 110, 0);
	vm_op_line(100, 120, 250, 120, 0);
	vm_op_line(100, 130, 250, 130, 0);
	vm_op_line(100, 140, 250, 140, 0);
	vm_op_line(100, 150, 250, 150, 0);
	vm_op_line(100, 160, 250, 160, 0);
	vm_op_line(100, 170, 250, 170, 0);
	vm_op_line(100, 180, 250, 180, 0);
	vm_op_line(100, 190, 250, 190, 0);

	vm_op_line(100, 40, 100, 190, 0);
	vm_op_line(110, 40, 110, 190, 0);
	vm_op_line(120, 40, 120, 190, 0);
	vm_op_line(130, 40, 130, 190, 0);
	vm_op_line(140, 40, 140, 190, 0);
	vm_op_line(150, 40, 150, 190, 0);
	vm_op_line(160, 40, 160, 190, 0);
	vm_op_line(170, 40, 170, 190, 0);
	vm_op_line(180, 40, 180, 190, 0);
	vm_op_line(190, 40, 190, 190, 0);
	vm_op_line(200, 40, 200, 190, 0);
	vm_op_line(210, 40, 210, 190, 0);
	vm_op_line(220, 40, 220, 190, 0);
	vm_op_line(230, 40, 230, 190, 0);
	vm_op_line(240, 40, 240, 190, 0);
	vm_op_line(250, 40, 250, 190, 0);

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (board1[i][j] == WHITE)
			{
				int new_y = 45 + 10*j;
				int new_x = 105 + 10*i;
				vm_op_circle(new_x, new_y, 4, 8);
			}
			else if (board1[i][j] == BLACK)
			{
				int new_y = 45 + 10*j;
				int new_x = 105 + 10*i;
				vm_op_circle(new_x, new_y, 4, 0);

			}

		}
	}
}

//玩家下棋
PUBLIC void PlayerMove(int player)
{
	int color;
	int color_num;
	if (player == 0) {
		vm_print_letter(0, 0, 0, "Player A go.");
                color = BLACK;
		color_num = 0;
	}
	if (player == 1) {
		vm_print_letter(0, 0, 0, "Player B go.");
		color = WHITE;
		color_num = 8;
	}
	vm_print_letter(0, 20, 0, "Please choose a location.");
	int x = 8;
	int y = 8;
	if (board1[7][7] == '0') {
		vm_op_circle(175, 115, 4, color_num);
	}

	while (1)
	{
		u32 key = GETINPUT();

		//确定棋子位置
		if (key == 'p' || key == 'P') {
			if (x >= 1 && x <= ROW && y >= 1 && y <= COL)
			{
				// 成功下棋
				if (board1[x - 1][y - 1] == '0'){
					board1[x - 1][y - 1] = color;
					vm_op_circle(95 + 10 * x, 35 + 10 * y, 4, color_num);
					break;
				}
				// 输入坐标已被占用
				else{
					vm_print_letter(0, 70, 0, "occupied!");

				}
			}
			// 输入坐标非法判断
			else{
				vm_print_letter(0, 70, 0, "illegal!");
			}
		}

		else if (key == 'w' || key == 'W') {
			if (board1[x - 1][y - 1] == '0') {
				vm_op_rectangle(91 + 10 * x, 31 + 10 * y, 99 + 10 * x, 39 + 10 * y, 14);
			}
			y--;
			if (x >= 1 && x <= ROW && y >= 1 && y <= COL) {
				if (board1[x - 1][y - 1] == '0') {
					vm_op_circle(95 + 10*x, 35 + 10*y, 4, color_num);
				}
				// 输入坐标有棋子，则不输出棋子
			}
			else
				y++;

		}

		else if (key == 's' || key == 'S') {
			if (board1[x - 1][y - 1] == '0') {
				vm_op_rectangle(91 + 10 * x, 31 + 10 * y, 99 + 10 * x, 39 + 10 * y, 14);
			}
			y++;
			if (x >= 1 && x <= ROW && y >= 1 && y <= COL) {
				if (board1[x - 1][y - 1] == '0') {
					vm_op_circle(95 + 10*x, 35 + 10*y, 4, color_num);
				}
				// 输入坐标有棋子，则不输出棋子
			}
			else
				y--;
		}

		else if (key == 'a' || key == 'A') {
			if (board1[x - 1][y - 1] == '0') {
				vm_op_rectangle(91 + 10 * x, 31 + 10 * y, 99 + 10 * x, 39 + 10 * y, 14);
			}
			x--;
			if (x >= 1 && x <= ROW && y >= 1 && y <= COL) {
				if (board1[x - 1][y - 1] == '0') {
					vm_op_circle(95 + 10*x, 35 + 10*y, 4, color_num);
				}
				// 输入坐标有棋子，则不输出棋子
			}
			else
				x++;
		}

		else if (key == 'd' || key == 'D') {
			if (board1[x - 1][y - 1] == '0') {
				vm_op_rectangle(91 + 10 * x, 31 + 10 * y, 99 + 10 * x, 39 + 10 * y, 14);
			}
			x++;
			if (x >= 1 && x <= ROW && y >= 1 && y <= COL) {
				if (board1[x - 1][y - 1] == '0') {
					vm_op_circle(95 + 10*x, 35 + 10*y, 4, color_num);
				}
				// 输入坐标有棋子，则不输出棋子
			}
			else
				x--;
		}
		else if (key == 'z' || key == 'Z') {
			over=1;
			break;
		}
	}

}

//判断平局
PUBLIC static int IsFull()
{
	int i = 0;
	int j = 0;

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (board1[i][j] == '0')
			{
				return 0;
			}
		}
	}

	//棋盘没有空位置了还没有判断出输赢，则平局
	return 1;
}

//判断输赢
PUBLIC char isWin()
{
	int i = 0;
	int j = 0;

	// 横线上五子连成一线，赢家产生
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL - 4; j++)
		{
			if (board1[i][j] == board1[i][j + 1]
				&& board1[i][j + 1] == board1[i][j + 2]
				&& board1[i][j + 2] == board1[i][j + 3]
				&& board1[i][j + 3] == board1[i][j + 4]
				&& board1[i][j] != '0')
			{
				return board1[i][j];
			}
		}
	}

	// 竖线上五子连成一线，赢家产生
	for (j = 0; j < COL; j++)
	{
		for (i = 0; i < ROW - 4; i++)
		{
			if (board1[i][j] == board1[i + 1][j]
				&& board1[i + 1][j] == board1[i + 2][j]
				&& board1[i + 2][j] == board1[i + 3][j]
				&& board1[i + 3][j] == board1[i + 4][j]
				&& board1[i][j] != '0')
			{
				return board1[i][j];
			}
		}
	}

	// 斜线上五子连成一线，赢家产生
	for (i = 0; i < ROW - 4; i++)
	{
		if (board1[i][i] == board1[i + 1][i + 1]
			&& board1[i + 1][i + 1] == board1[i + 2][i + 2]
			&& board1[i + 2][i + 2] == board1[i + 3][i + 3]
			&& board1[i + 3][i + 3] == board1[i + 4][i + 4]
			&& board1[i][i] != '0')
		{
			return board1[i][i];
		}

		if (board1[i][i + 4] == board1[i + 1][i + 3]
			&& board1[i + 1][i + 3] == board1[i + 2][i + 2]
			&& board1[i + 2][i + 2] == board1[i + 3][i + 1]
			&& board1[i + 3][i + 1] == board1[i + 4][i]
			&& board1[i][i + 4] != '0')
		{
			return board1[i][i + 4];
		}
	}

	//游戏平局
	if (IsFull())
	{
		return 'p';
	}

	//游戏结束
	return ' ';

}
