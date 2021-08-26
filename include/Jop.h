#include "stdio.h"
#include "time.h"
#include "DrawingTools.h"
#include "keyboard.h"


 /*        常量声明         */
#define AIRPLANT 4//蛇的身体最大节数
#define MAPWITH_JOP 24 //宽度
#define MAPHIGH_JOP 39//高度
#define OFFSET 8 //偏移块数

 /*        函数声明         */
PUBLIC void initmap_jop();//初始化贪吃蛇地图
PUBLIC void initothers_jop();//对地图外的其余元素进行初始化
PUBLIC void move_jop();//对移动进行判断
PUBLIC void input_jop();//输入判断
PUBLIC void bojudge_jop();//边界判断

 /*        全局变量声明         */
char direction='w';//当前所移动的方向
int gameover=0;//游戏结束标志
int exitflag=0;//游戏结束标志
int changeflag=0;//是否迟到了食物
int score=0;//当前获得的总分
int emenycount;//敌机计数器
int bulletcount = 0;//子弹计数器

 
//定义坐标结构体
struct Pos_jop {
	int x;
	int y;
}bullet[1000];
//蛇的相关属性
struct  {
	int len;//蛇的长度
	int x[AIRPLANT];//组成蛇身的每一个小方块中x的坐标
	int y[AIRPLANT];//组成蛇身的每一个小方块中y的坐标
 
}myair;

//食物相关属性
struct enemy{
	int x;
	int y;
	int life;//敌机血量
	int vy;//敌机移动速度
}Enemy[5];
 
PUBLIC void GOJOP() {
    clear();//清屏
	score=0;
	gameover=0;
	exitflag=0;
    initmap_jop();//对地图进行初始化
	initothers_jop();//字符初始化

	//游戏主循环
	while(1)
	{
		 while (!gameover)
		  {
			  input_jop();//处理输入
			  move_jop();
			  ReprintBullet();//重绘子弹
	          reprintEmeny();//重绘敌机
			  bojudge_jop();
			  UPDATESCORE_jop();
		  }
		if(exitflag==1)
		{
			break;
		}
	
	 vm_print_letter(90,80,7,"YOU LOSE");		
	 vm_print_letter(90,100,7,"RESTART?[y/n]");
			u32 key=GETINPUT();
			if(key=='y'||key=='Y')
			{
				gameover=0;
				clear();//清屏
                initmap_jop();//对地图进行初始化
	            initothers_jop();//字符初始化
			}
			else
			{
				break;
			}

	}
	for(int i=0;i<5;i++){
		Enemy[i].x=0;
		Enemy[i].y=0;
	}
	for(int i=0;i<bulletcount;i++){
		bullet[i].x=0;
		bullet[i].y=0;
	}


    clear();
}
void UPDATESCORE_jop() //50*80
{
   	char *_score ;
	   iitoa(score,_score,10);
	   vm_op_rectangle(270,10,320,40,15);
	   vm_print_letter(270,10,7,_score);
}


PUBLIC void initmap_jop(){
	clearScreen();//清屏

	 /*                   主地图                         */
	for(int x=OFFSET-1;x<MAPWITH_JOP/2+OFFSET+1;x++)
	{
		for (int y=OFFSET+1;y<MAPHIGH_JOP/2+OFFSET;y++)
		vm_op_rec_64px(x,y,6);
	}

	
  
	/*                   食物初始化                        */
emenycount = 5;//敌机数量
for (int i = 0; i < emenycount; i++){
while (1)
	{



		/*生成的食物横坐标的奇偶必须和初始时蛇头所在坐标的奇偶一致
		，若不一致会导致吃食物的时候只吃到一半*/
		srand(TTime());
		Enemy[i].x = OFFSET;
		Enemy[i].y = OFFSET +1+ rand() % ( MAPHIGH_JOP/2-3);
		Enemy[i].vy = 1;
		if (Enemy[i].x % 2 == 0)
			break;
	}
    vm_op_rec_64px(Enemy[i].x,Enemy[i].y,130);	//打印食物
    vm_op_rec_64px(Enemy[i].x+1,Enemy[i].y,130);
	vm_op_rec_64px(Enemy[i].x,Enemy[i].y-1,130);
	vm_op_rec_64px(Enemy[i].x,Enemy[i].y+1,130);		
		}
	

	/*                   蛇初始化                         */
	myair.len = 4;//蛇的长度
	myair.x[0] = MAPWITH_JOP-OFFSET;//x坐标为偶数
	myair.y[0] = MAPHIGH_JOP/2;
	myair.x[1]=myair.x[0]-1;
	myair.y[1]=myair.y[0];
	myair.x[2]=myair.x[0];
	myair.y[2]=myair.y[0]+1;
	myair.x[3]=myair.x[0];
	myair.y[3]=myair.y[0]-1;

	
    vm_op_rec_64px(myair.x[0],myair.y[0],10);	//打印蛇头
	vm_op_rec_64px(myair.x[1],myair.y[1],10);
	vm_op_rec_64px(myair.x[2],myair.y[2],10);
	vm_op_rec_64px(myair.x[3],myair.y[3],10);

}

PUBLIC void initothers_jop()
{
    vm_print_letter(60,10,13,"Aircraft War Game!");
	vm_print_letter(220,10,7,"Score:");
	vm_print_letter(270,10,7,"0");
	vm_print_letter(50,170,7,"UP[W],DOWN[S],LEFT[A],RIGHT[D]");
	vm_print_letter(100,186,7,"EXIT[Enter]");
}

PUBLIC void move_jop()
{

//清除原先飞机
	for (int i = AIRPLANT - 1; i>=0; i--) {
		vm_op_rec_64px(myair.x[i], myair.y[i], 6);
	}
    
	//判断蛇头应该往哪个方向移动
	switch (direction)
	{
	case 'a':
		for (int i = AIRPLANT - 1; i>=0; i--) {
		myair.y[i] -= 1;//往左
	}
	
		
		break;
	case 'd':
			for (int i = AIRPLANT - 1; i>=0; i--) {
		myair.y[i] += 1;//往右
	}
		
		break;
	case 'w':
				for (int i = AIRPLANT - 1; i>=0; i--) {
		myair.x[i]--;//往上
	}
		
		break;
	case 's':
				for (int i = AIRPLANT - 1; i>=0; i--) {
				myair.x[i]++;//往下
	}

		break;
	}
	//打印出蛇头
	for(int i = AIRPLANT - 1; i>=0; i--){
        vm_op_rec_64px(myair.x[i],myair.y[i],10);
	
	}


	return;
}

//发射子弹
PUBLIC void firebullet()
{
		bullet[bulletcount].x = myair.x[0]-1;
		bullet[bulletcount].y = myair.y[0];
		bulletcount++;

}

PUBLIC void CleanmyBullet(int x, int y)
{
//清除己方炮弹
	vm_op_rec_64px(x, y, 6);
}

PUBLIC void printBullet(int x, int y)
{
	for (int i = 0; i < bulletcount; i++)
	{
		vm_op_rec_64px(x, y, 300);

	}
}
PUBLIC void collion()
{
	for (int i = 0; i < emenycount; i++)
	{
		for (int j = 0; j < bulletcount; j++)
		{
			if (bullet[j].y >= Enemy[i].y-2&&bullet[j].y <= Enemy[i].y+2&&  bullet[j].x == Enemy[i].x)//当击中时
			{

		vm_op_rec_64px(Enemy[i].x, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x + 1, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y+1, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y-1, 6);
		        srand(TTime());
		        Enemy[i].x = OFFSET;
		        Enemy[i].y = OFFSET +1+ rand() % ( MAPHIGH_JOP/ 2 -3);
		        Enemy[i].vy = 1;
				score += 100;
			

			}
		}
	}

}


PUBLIC void ReprintBullet()//重绘子弹
{

	//开火 fire！！！
	firebullet();
	//清除当前所有子弹
	for (int i = 0; i < bulletcount - 1; i++)
	{
		CleanmyBullet(bullet[i].x, bullet[i].y);
	}
	//当子弹到达边界，删除子弹
	for (int i = 0; i < bulletcount; i++)
	{
		if (bullet[i].x > OFFSET)
		{
			bullet[i].x--;

		}
		else
		{
			if (bullet[i].x <= OFFSET)
			{
				for (int i = 0; i < bulletcount - 1; i++)
				{
					bullet[i] = bullet[i + 1];
				}
				bulletcount--;

			}
		}
	}


	for (int i = 0; i < bulletcount - 1; i++)
	{

		printBullet(bullet[i].x, bullet[i].y);

	}
	collion();
}

PUBLIC void cleanEmeny()//清除敌机（绘制清除）
{
	for (int i = 0; i < emenycount; i++)
	{

		vm_op_rec_64px(Enemy[i].x, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x + 1, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y+1, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y-1, 6);
	}

}

PUBLIC void printEnemy(struct enemy myEnemy)
{

	vm_op_rec_64px(myEnemy.x, myEnemy.y, 130);
	vm_op_rec_64px(myEnemy.x+1, myEnemy.y, 130);
	vm_op_rec_64px(myEnemy.x, myEnemy.y-1, 130);
	vm_op_rec_64px(myEnemy.x, myEnemy.y+1, 130);

}

PUBLIC void reprintEmeny()//重绘敌机
{
	cleanEmeny();
	for (int i = 0; i < emenycount; i++)
	{
		if (Enemy[i].x < MAPWITH_JOP/2+OFFSET-1)
		{
			Enemy[i].vy =  1;
			Enemy[i].x += Enemy[i].vy;

		}
		else if(Enemy[i].x>=OFFSET+MAPWITH_JOP/2-1){
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x + 1, Enemy[i].y, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y+1, 6);
		vm_op_rec_64px(Enemy[i].x, Enemy[i].y-1, 6);
		        srand(TTime());
		        Enemy[i].x = OFFSET;
		        Enemy[i].y = OFFSET +1+ rand() % ( MAPHIGH_JOP/ 2 -3);
		        Enemy[i].vy = 1;
		}
		else
		{
		srand(TTime());
		Enemy[i].x = OFFSET;
		Enemy[i].y = OFFSET + rand() % ( MAPHIGH_JOP/ 2 - 3);
		Enemy[i].vy = 1;

		}
	}
	for (int i = 0; i < emenycount; i++)
	{
		printEnemy(Enemy[i]);
	}
}

PUBLIC void input_jop()
{
   u32 key=GETINPUT();
	if (!(key & FLAG_EXT))//表示当前字符是一个可打印字符；
	 {
		switch (key)
		{
			case 'a':
			case 'A':
				
					direction = 'a';
				
				break;
			case 'w':
			case 'W':
				
					direction = 'w';
				
				break;
			case 'd':
			case 'D':
				
					direction = 'd';
				
				break;
			case 's':
			case 'S':
				
					direction = 's';
				
				break;

	}
	 }
else
	{
		int raw_code = key & MASK_RAW;
		switch(raw_code) {
			
			case ENTER:
			    gameover=1;
				exitflag=1;
				break;
		}
	}
	return;
}

PUBLIC void bojudge_jop()
{

for(int i=0;i<emenycount;i++){
	if((Enemy[i].x==myair.x[0])&&(Enemy[i].y==myair.y[0]))
	gameover=1;
}
	//蛇头碰到上下边界，游戏结束
	if (myair.y[0] == OFFSET+1 || myair.y[0] == MAPHIGH_JOP/2+OFFSET-1)
		gameover=1;
	//蛇头碰到左右边界，游戏结束
	if (myair.x[0] == OFFSET-1 || myair.x[0] == MAPWITH_JOP/2+OFFSET)
		gameover=1;
	//蛇头碰到蛇身，游戏结束
	for (int i = 1; i < myair.len; i++)
	{
		if (myair.x[i] == myair.x[0] && myair.y[i] == myair.y[0])
			gameover=1;
	}
}

