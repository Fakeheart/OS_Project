#include "stdio.h"
#include "time.h"
#include "DrawingTools.h"
#include "keyboard.h"

/*        常量声明         */
int CenterY[12] = {105, 115, 125, 135, 145, 155, 165, 175, 185, 195, 205, 215};
int CenterX[12] = {45, 55, 65, 75, 85, 95, 105, 115, 125, 135, 145, 155};
const int DIR[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

// option
typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

#define MAX_BYTE 10000

/*        全局变量声明         */
struct Command
{
  int x;
  int y;
  OPTION option;
};
struct Command preferedPos = {0, 0, 0};
int ChessWhiteX[16] = {0};
int ChessWhiteY[16] = {0};
char optionChess[6] = "";
char board[BOARD_SIZE][BOARD_SIZE] = {0};
char wboard[5][BOARD_SIZE][BOARD_SIZE];
int me_flag = BLACK;
int other_flag = WHITE;
int condenval = 0;
int WHITENUM = 0;
int chessgameOver = 0;
int chess_exit_flag = 0;
int step = 0;

/*        函数声明         */
PUBLIC void InitMap_chess();
PUBLIC void UpdateMap_chess();
void RecordWhitePos();
void UpdateWords(int flag, int num);
void userchoose();
void SetEmptyBoard();BOOL place(int x, int y, OPTION option, int cur_flag, char board[BOARD_SIZE][BOARD_SIZE]);
BOOL isInBound(int x, int y)
{
  return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

PUBLIC void GOCHESS()
{
  clear();         //清屏
  InitMap_chess(); //对地图进行初始化
  while (1)        //游戏主循环
  {
    RecordWhitePos(); //记录地图中己方的棋子位置
    int w = -1;       //为循环选择棋子定义变量
    int d = -1;       //为循环选择方向定义变量
    int checkX, checkY, checkDir;
    while (1)
    {
      if (WHITENUM < 4 || WHITENUM > 12)
      {
        break;
      }
      u32 key = GETINPUT();
      if (key == 'p' || key == 'P') //退出游戏
      {
        chess_exit_flag = 1;
        break;
      }
      if (key == 'q' || key == 'Q') //选择棋子
      {
        w++;
        if (w)
        {
          vm_op_circle(CenterY[ChessWhiteY[(w - 1) % WHITENUM]], CenterX[ChessWhiteX[(w - 1) % WHITENUM]], 4, 8);
        }
        vm_op_circle(CenterY[ChessWhiteY[w % WHITENUM]], CenterX[ChessWhiteX[w % WHITENUM]], 4, 1);
        UpdateWords(0, w); //更新左侧棋子有关信息栏信息
      }
      else if (key == 'w' || key == 'W') //选择方向
      {
        d++;
        UpdateWords(1, d % 8); //更新左侧方向有关信息栏信息
      }
      else if (key == 'd' || key == 'D') //走子
      {
        if (w > -1 && d > -1) //如果已经选择了棋子及方向
        {
          checkX = ChessWhiteX[w % WHITENUM];
          checkY = ChessWhiteY[w % WHITENUM];
          checkDir = d % 8;
          BOOL judge = place(checkX, checkY, checkDir, WHITE, board);
          if (judge == TRUE) //落子位置有效
          {
            RecordWhitePos();
            aiTurn(board, BLACK); //AI走子
            place(preferedPos.x, preferedPos.y, preferedPos.option, BLACK, board);
            w = -1;
            d = -1;            //恢复变量
            UpdateMap_chess(); //重画地图
          }
          else //落子位置无效
          {
            vm_print_letter(230, 80, 1, "Wrong Step!");
          }
        }
        else //棋子或方向还未选择
        {
          vm_print_letter(230, 80, 1, "How To Go?");
        }
      }
    }
    if (chess_exit_flag)
    {
      SetEmptyBoard();
      break;
    }
    userchoose();
    if (chessgameOver)
    {
      SetEmptyBoard();
      break;
    }
  }
  clearScreen();
}
void RecordWhitePos()
{
  WHITENUM = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      if (board[i][j] == WHITE)
      {
        ChessWhiteX[WHITENUM] = i;
        ChessWhiteY[WHITENUM] = j;
        WHITENUM++;
      }
  return;
}
void UpdateWords(int flag, int num)
{
  vm_op_rectangle(230, 80, 319, 96, 12);
  if (flag == 0) //对于棋子的文字信息更新
  {
    char *_ch1;
    char *_ch2;
    vm_op_rectangle(10, 56, 95, 72, 12); //棋子坐标信息覆盖
    vm_print_letter(10, 56, 0, iitoa(ChessWhiteX[num % WHITENUM], _ch1, 10));
    vm_print_letter(18, 56, 0, " ");
    vm_print_letter(26, 56, 0, iitoa(ChessWhiteY[num % WHITENUM], _ch2, 10));
  }
  else if (flag == 1)
  {
    vm_op_rectangle(10, 88, 95, 104, 12); //确定信息覆盖
    switch (num)
    {
    case 0:
      vm_print_letter(10, 88, 0, "UP");
      break;
    case 1:
      vm_print_letter(10, 88, 0, "DOWN");
      break;
    case 2:
      vm_print_letter(10, 88, 0, "LEFT");
      break;
    case 3:
      vm_print_letter(10, 88, 0, "RIGHT");
      break;
    case 4:
      vm_print_letter(10, 88, 0, "UP_LEFT");
      break;
    case 5:
      vm_print_letter(10, 88, 0, "UP_RIGHT");
      break;
    case 6:
      vm_print_letter(10, 88, 0, "DOWN_LEFT");
      break;
    case 7:
      vm_print_letter(10, 88, 0, "DOWN_RIGHT");
      break;
    default:
      break;
    }
  }
  else
    return;
  return;
}
void userchoose()
{
  if (WHITENUM < 4)
  {
    vm_print_letter(90, 80, 7, "YOU LOSE");
    vm_print_letter(90, 100, 7, "RESTART?[y/n]");
  }
  else
  {
    vm_print_letter(90, 80, 7, "YOU WIN");
    vm_print_letter(90, 100, 7, "RESTART?[y/n]");
  }
  u32 key = GETINPUT();
  if (key == 'n' || key == 'N')
  {
    chessgameOver == 1;
  }
}
void SetEmptyBoard()
{
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      board[i][j] = EMPTY;
}
PUBLIC void UpdateMap_chess()
{
  boxfill8(SCREEN_WIDTH, 12, 0, 40, 320, 160);
  vm_print_letter(0, 40, 0, "ChooseChess:");
  vm_print_letter(0, 72, 0, "ChooseDir:");

  for (int yy = 40; yy < 161; yy += 10)
    vm_op_line(100, yy, 220, yy, 0);

  for (int xx = 100; xx < 221; xx += 10)
    vm_op_line(xx, 40, xx, 160, 0);

  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      if (board[i][j] == WHITE)
      {
        vm_op_circle(CenterY[j], CenterX[i], 4, 8);
      }
      else if (board[i][j] == BLACK)
      {
        vm_op_circle(CenterY[j], CenterX[i], 4, 0);
      }
    }
}

PUBLIC void InitMap_chess()
{
  boxfill8(SCREEN_WIDTH, 12, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  vm_print_letter(0, 40, 0, "ChooseChess:");
  vm_print_letter(0, 72, 0, "ChooseDir:");
  vm_print_letter(75, 5, 4, "'TiaoJia'Chess Game!");
  vm_print_letter(70, 168, 13, "[Q]:ChooseChess;[D]:Go!");
  vm_print_letter(70, 184, 13, "[W]:ChooseDir;[P]:Exit");

  for (int yy = 40; yy < 161; yy += 10)
    vm_op_line(100, yy, 220, yy, 0);
  for (int xx = 100; xx < 221; xx += 10)
    vm_op_line(xx, 40, xx, 160, 0);
  for (int i = 0; i < 3; i++)
  {
    board[2][2 + i] = WHITE;
    board[6][6 + i] = WHITE;
    board[5][3 + i] = BLACK;
    board[9][7 + i] = BLACK;
    vm_op_circle(CenterY[2 + i], CenterX[2], 4, 8);
    vm_op_circle(CenterY[6 + i], CenterX[6], 4, 8);
    vm_op_circle(CenterY[3 + i], CenterX[5], 4, 0);
    vm_op_circle(CenterY[7 + i], CenterX[9], 4, 0);
  }

  for (int i = 0; i < 2; i++)
  {
    board[8 + i][2] = WHITE;
    board[2 + i][9] = BLACK;
    vm_op_circle(CenterY[2], CenterX[8 + i], 4, 8);
    vm_op_circle(CenterY[9], CenterX[2 + i], 4, 0);
  }
}

BOOL Place(int x, int y, OPTION option, int cur_flag, int m)
{

  if (wboard[m][x][y] != cur_flag)
  {
    return FALSE;
  }

  int new_x = x + DIR[option][0];
  int new_y = y + DIR[option][1];

  if (!isInBound(new_x, new_y) || wboard[m][new_x][new_y] != EMPTY)
  {
    return FALSE;
  }

  wboard[m][x][y] = EMPTY;
  wboard[m][new_x][new_y] = cur_flag;

  int cur_other_flag = 3 - cur_flag;

  int intervention_dir[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
  for (int i = 0; i < 4; i++)
  {
    int x1 = new_x + intervention_dir[i][0];
    int y1 = new_y + intervention_dir[i][1];
    int x2 = new_x - intervention_dir[i][0];
    int y2 = new_y - intervention_dir[i][1];
    if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[m][x1][y1] == cur_other_flag && wboard[m][x2][y2] == cur_other_flag)
    {
      wboard[m][x1][y1] = cur_flag;
      wboard[m][x2][y2] = cur_flag;
    }
  }

  int custodian_dir[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (int i = 0; i < 8; i++)
  {
    int x1 = new_x + custodian_dir[i][0];
    int y1 = new_y + custodian_dir[i][1];
    int x2 = new_x + custodian_dir[i][0] * 2;
    int y2 = new_y + custodian_dir[i][1] * 2;
    if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[m][x2][y2] == cur_flag && wboard[m][x1][y1] == cur_other_flag)
    {
      wboard[m][x1][y1] = cur_flag;
    }
  }

  return TRUE;
}
BOOL place(int x, int y, OPTION option, int cur_flag, char board[BOARD_SIZE][BOARD_SIZE])
{

  if (board[x][y] != cur_flag)
  {
    return FALSE;
  }

  int new_x = x + DIR[option][0];
  int new_y = y + DIR[option][1];

  if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY)
  {
    return FALSE;
  }

  board[x][y] = EMPTY;
  board[new_x][new_y] = cur_flag;

  int cur_other_flag = 3 - cur_flag;

  int intervention_dir[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
  for (int i = 0; i < 4; i++)
  {
    int x1 = new_x + intervention_dir[i][0];
    int y1 = new_y + intervention_dir[i][1];
    int x2 = new_x - intervention_dir[i][0];
    int y2 = new_y - intervention_dir[i][1];
    if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == cur_other_flag && board[x2][y2] == cur_other_flag)
    {
      board[x1][y1] = cur_flag;
      board[x2][y2] = cur_flag;
    }
  }

  int custodian_dir[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
  for (int i = 0; i < 8; i++)
  {
    int x1 = new_x + custodian_dir[i][0];
    int y1 = new_y + custodian_dir[i][1];
    int x2 = new_x + custodian_dir[i][0] * 2;
    int y2 = new_y + custodian_dir[i][1] * 2;
    if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == cur_flag && board[x1][y1] == cur_other_flag)
    {
      board[x1][y1] = cur_flag;
    }
  }

  return TRUE;
}
void Evaluate_2()
{
  condenval = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    for (int j = 0; j < BOARD_SIZE; j++)
    {
      if (wboard[2][i][j] != me_flag)
      {
        continue;
      }
      if (me_flag == 1)
      {
        int delta_x = max(i - 6, 6 - i);
        int delta_y = max(j - 6, 6 - j);
        condenval += max(delta_x, delta_y);
      }
      else
      {
        int delta_x = max(i - 5, 5 - i);
        int delta_y = max(j - 5, 5 - j);
        condenval += max(delta_x, delta_y);
      }
    }
  }
}
int livingnumval()
{
  int total = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      if (wboard[4][i][j] == me_flag)
        total += 1;
  return total;
}
int threatenval()
{
  int me_risk = 0;
  for (int x = 0; x < BOARD_SIZE; x++)
  {
    for (int y = 0; y < BOARD_SIZE; y++)
    {
      if (wboard[4][x][y] == me_flag)
      {
        int j = me_risk;
        for (int i = 0; i < 8; i++)
        {
          int x1 = x + DIR[i][0];
          int y1 = y + DIR[i][1];
          int x2 = x1 + DIR[i][0];
          int y2 = y1 + DIR[i][1];
          if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[4][x1][y1] == EMPTY && wboard[4][x2][y2] == me_flag)
          {
            for (int k = 0; k < 8; k++)
            {
              int x3 = x1 + DIR[k][0];
              int y3 = y1 + DIR[k][1];
              if (isInBound(x3, y3) && wboard[4][x3][y3] == other_flag)
              {
                me_risk++;
                break;
              }
            }
          }
          if (me_risk != j)
          {
            break;
          }
        }
        int k = me_risk;
        for (int i = 0; i < 8; i++)
        {
          int x1 = x + DIR[i][0];
          int y1 = y + DIR[i][1];
          int x2 = x - DIR[i][0];
          int y2 = y - DIR[i][1];
          if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[4][x1][y1] == other_flag && wboard[4][x2][y2] == EMPTY)
          {
            for (int k = 0; k < 8; k++)
            {
              int x3 = x2 + DIR[k][0];
              int y3 = y2 + DIR[k][1];
              if (isInBound(x3, y3) && wboard[4][x3][y3] == other_flag)
              {
                me_risk++;
                break;
              }
            }
          }
          if (me_risk != k)
          {
            break;
          }
        }
      }
    }
  }
  return me_risk;
}
int attackval()
{
  int other_risk = 0;
  for (int x = 0; x < BOARD_SIZE; x++)
  {
    for (int y = 0; y < BOARD_SIZE; y++)
    {
      if (wboard[4][x][y] == other_flag)
      {
        int j = other_risk;
        for (int i = 0; i < 8; i++)
        {
          int x1 = x + DIR[i][0];
          int y1 = y + DIR[i][1];
          int x2 = x1 + DIR[i][0];
          int y2 = y1 + DIR[i][1];
          if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[4][x1][y1] == EMPTY && wboard[4][x2][y2] == other_flag)
          {
            for (int k = 0; k < 8; k++)
            {
              int x3 = x1 + DIR[k][0];
              int y3 = y1 + DIR[k][1];
              if (isInBound(x3, y3) && wboard[4][x3][y3] == me_flag)
              {
                other_risk++;
                break;
              }
            }
          }
          if (other_risk != j)
          {
            break;
          }
        }
        int k = other_risk;
        for (int i = 0; i < 8; i++)
        {
          int x1 = x + DIR[i][0];
          int y1 = y + DIR[i][1];
          int x2 = x - DIR[i][0];
          int y2 = y - DIR[i][1];
          if (isInBound(x1, y1) && isInBound(x2, y2) && wboard[4][x1][y1] == me_flag && wboard[4][x2][y2] == EMPTY)
          {
            for (int k = 0; k < 8; k++)
            {
              int x3 = x2 + DIR[k][0];
              int y3 = y2 + DIR[k][1];
              if (isInBound(x3, y3) && wboard[4][x3][y3] == me_flag)
              {
                other_risk++;
                break;
              }
            }
          }
          if (other_risk != k)
          {
            break;
          }
        }
      }
    }
  }
  return other_risk;
}
int Evaluate()
{
  int all_value_me;
  if (attackval() < threatenval())
  {
    all_value_me = 8000 * livingnumval() - 200 * threatenval() - condenval;
  }
  else if (2 * attackval() >= 3 * threatenval())
  {
    all_value_me = 8000 * livingnumval() + 500 * attackval() - condenval;
  }
  else
    all_value_me = 8000 * livingnumval() + 500 * attackval() - 200 * threatenval() - condenval;
  return all_value_me;
}
int MiniMax_AlphaBeta(int depth, int Alpha, int Beta, int player)
{
  if (depth == 3)
  {
    return Evaluate();
  }
  if (depth == 2)
  {
    Evaluate_2();
  }
  for (int k = 7; k >= 0; k--)
  {
    for (int p = 0; p < BOARD_SIZE; p++)
    {
      for (int q = 0; q < BOARD_SIZE; q++)
      {
        if (wboard[depth][p][q] == player)
        {
          int new_p = p + DIR[k][0];
          int new_q = q + DIR[k][1];
          if (isInBound(new_p, new_q) && wboard[depth][new_p][new_q] == EMPTY)
          {
            for (int x = 0; x < BOARD_SIZE; x++)
              for (int y = 0; y < BOARD_SIZE; y++)
                wboard[depth + 1][x][y] = wboard[depth][x][y];
            Place(p, q, k, player, depth + 1);
            int value = -MiniMax_AlphaBeta(depth + 1, -Beta, -Alpha, 3 - player);
            if (value > Alpha)
            {
              Alpha = value;
              if (depth == 0)
              {
                preferedPos.x = p;
                preferedPos.y = q;
                preferedPos.option = k;
              }
            }
            if (value >= Beta)
            {
              return value;
            }
          }
        }
      }
    }
  }
  return Alpha;
}
int aiTurn(char _board[BOARD_SIZE][BOARD_SIZE], int me)
{
  if (_board[9][7] == me_flag && _board[8][6] == EMPTY)
  {
    preferedPos.x = 9;
    preferedPos.y = 7;
    preferedPos.option = 4;
  }
  else if (_board[3][9] == me_flag && _board[4][8] == EMPTY)
  {
    preferedPos.x = 3;
    preferedPos.y = 9;
    preferedPos.option = 6;
  }
  else
  {
    for (int x = 0; x < BOARD_SIZE; x++)
      for (int y = 0; y < BOARD_SIZE; y++)
        wboard[0][x][y] = _board[x][y];

    MiniMax_AlphaBeta(0, -0x3f3f3f3f, 0x3f3f3f3f, me);
  }

  return 1;
}