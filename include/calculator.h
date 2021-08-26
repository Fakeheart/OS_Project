#include "stdio.h"
#include "DrawingTools.h"
#include "keyboard.h"

struct rec_pos {
	int x_start;
	int y_start;
	int x_end;
	int y_end;
};

struct num_pos {
	int x_num;
	int y_num;
};

/*        ��������         */
char NUM[16] = { '0','.','=', '/' ,'1','2','3','*','4','5','6','-','7','8','9','+' };
char formula[100] = {};
char e[100], f[100];
struct rec_pos REC[16] = { {60,150,90,180},{90,150,120,180},{120,150,150,180},{150,150,180,180},{60,120,90,150},{90,120,120,150},{120,120,150,150},{150,120,180,150},{60,90,90,120},{90,90,120,120},{120,90,150,120},{150,90,180,120},{60,60,90,90},{90,60,120,90},{120,60,150,90},{150,60,180,90} };
struct num_pos NUMPOS[16] = { {85, 160},{85, 160},{130, 160,},{160,160},{70, 130},{100,130},{130,130},{160,130},{70,100},{100,100},{130,100},{160,100},{70,70},{100,70},{130,70},{160,70} };

/*        ��������         */
PUBLIC void InitCal();//��ʼ��
PUBLIC double readnum(int* i);
PUBLIC double evalpost();
PUBLIC int is_operation(char op);
PUBLIC int priority(char op);
PUBLIC void postfix();
PUBLIC void transform(char g[]);

PUBLIC void InitCal() {
	boxfill8(SCREEN_WIDTH, 14, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);	//����

	vm_op_line(60, 30, 180, 30, 0);
	vm_op_line(60, 60, 180, 60, 0);
	vm_op_line(60, 90, 180, 90, 0);
	vm_op_line(60, 120, 180, 120, 0);
	vm_op_line(60, 150, 180, 150, 0);
	vm_op_line(60, 180, 180, 180, 0);

	vm_op_line(60, 30, 60, 180, 0);
	vm_op_line(90, 60, 90, 180, 0);
	vm_op_line(120, 60, 120, 180, 0);
	vm_op_line(150, 60, 150, 180, 0);
	vm_op_line(180, 30, 180, 180, 0);

	vm_print_letter(70, 70, 0, "7");
	vm_print_letter(100, 70, 0, "8");
	vm_print_letter(130, 70, 0, "9");
	vm_print_letter(160, 70, 0, "+");
	vm_print_letter(70, 100, 0, "4");
	vm_print_letter(100, 100, 0, "5");
	vm_print_letter(130, 100, 0, "6");
	vm_print_letter(160, 100, 0, "-");
	vm_print_letter(70, 130, 0, "1");
	vm_print_letter(100, 130, 0, "2");
	vm_print_letter(130, 130, 0, "3");
	vm_print_letter(160, 130, 0, "*");
	vm_print_letter(70, 160, 0, "0");
        vm_print_letter(100, 160, 0, ".");
	vm_print_letter(130, 160, 0, "=");
	vm_print_letter(160, 160, 0, "/");
}

PUBLIC double readnum(int* i)
{
	int x = 0.0;
	int k = 0;
	while (f[*i] >= '0' && f[*i] <= '9')
	{
		x = x * 10 + (f[*i] - '0');
		(*i)++;
	}
	if (f[*i] == '.')
	{
		(*i)++;
		while (f[*i] >= '0' && f[*i] <= '9')
		{
			x = x * 10 + (f[*i] - '0');
			(*i)++;
			k++;
		}
	}
	while (k-- > 0)
	{
		x = x / 10;
	}
	return (x);
}

/* �����׺����ʽ��ֵ */
PUBLIC double evalpost()
{
	double obst[10];
	int top = 0;
	int i = 0;
	double x1, x2;
	while (f[i] != '=')
	{
		if (f[i] >= '0' && f[i] <= '9')
		{
			obst[top] = readnum(&i); top++;
		}
		else if (f[i] == ' ')
			i++;
		else if (f[i] == '+')
		{
			x1 = obst[--top];
			x2 = obst[--top];
			obst[top] = x1 + x2;
			i++;
			top++;
		}
		else if (f[i] == '-')
		{
			x1 = obst[--top];
			x2 = obst[--top];
			obst[top] = x2 - x1;
			i++;
			top++;
		}
		else if (f[i] == '*')
		{
			x1 = obst[--top];
			x2 = obst[--top];
			obst[top] = x1 * x2;
			i++;
			top++;
		}
		else if (f[i] == '/')
		{
			x1 = obst[--top];
			x2 = obst[--top];
			obst[top] = x2 / x1;
			i++;
			top++;
		}
	}
	return obst[0];
}

/* �ж��ַ��Ƿ�Ϊ�����ַ� */
PUBLIC int is_operation(char op)
{
	switch (op)
	{
	case'^':
	case'K':
	case'+':
	case'-':
	case'*':
	case'/': return 1;
	default: return 0;
	}
}

/* �ж��ַ������ȼ� */
PUBLIC int priority(char op)
{
	switch (op)
	{
	case'=': return -1;
	case'(': return 0;
	case'+':
	case'-': return 1;
	case'*':
	case'/': return 2;
	default: return -1;
	}
}

/* ��׺����ʽת���ɺ�׺����ʽ*/
PUBLIC void postfix()
{
	int i = 0, j = 0, k = 0;
	char opst[100];
	int top = 0;
	opst[0] = '='; top++;
	while (e[i] != '=')
	{
		if ((e[i] >= '0' && e[i] <= '9') || e[i] == '.')
			f[j++] = e[i];
		else if (e[i] == '(')
		{
			opst[top] = e[i]; top++;
		}
		else if (e[i] == ')')
		{
			k = top - 1;
			while (opst[k] != '(') { f[j++] = opst[--top]; k = top - 1; }
			top--;
		}
		else if (is_operation(e[i]))
		{
			f[j++] = ' ';
			while (priority(opst[top - 1]) >= priority(e[i]))
				f[j++] = opst[--top];
			opst[top] = e[i];
			top++;
		}
		i++;
	}
	while (top) f[j++] = opst[--top]; f[j] = '\0';
}

/* ת�� */
PUBLIC void transform(char g[])
{
	int k, i, j = 0;
	for (i = 0; g[i] != '='; i++)
	{
		k = i + 1;
		if (g[i] == '(' && g[k] == '-')
		{
			e[j++] = g[i];
			e[j++] = '0';
		}
		else e[j++] = g[i];
	}
	e[j] = '=';
}

PUBLIC void calculate()
{
	clear();
	int flag;
	do
	{
		InitCal();
				vm_op_line(REC[0].x_start, REC[0].y_start, REC[0].x_start + 30, REC[0].y_start, 6);
		vm_op_line(REC[0].x_start, REC[0].y_start, REC[0].x_start, REC[0].y_start + 30, 6);
		vm_op_line(REC[0].x_start + 30, REC[0].y_start, REC[0].x_start + 30, REC[0].y_start + 30, 6);
		vm_op_line(REC[0].x_start, REC[0].y_start + 30, REC[0].x_start + 30, REC[0].y_start + 30, 6);
		int choose = 0;
		int count = -1;
		while (1) {
			u32 key = GETINPUT();
			if (key == 'p' || key == 'P') {
				count++;
				formula[count] = NUM[choose];
				if (choose == 2)
					break;
				else {
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 0);
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 0);
					vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
					vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
					choose = 0;
				}
			}

			else if (key == 'w' || key == 'W') {
				if (choose != 12 && choose != 13 && choose != 14 && choose != 15) {
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 0);
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 0);
					vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
					vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
					choose += 4;
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 6);
					vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 6);
					vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
					vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
				}
}
				else if (key == 's' || key == 'S') {
					if (choose != 0 && choose != 1 && choose != 2 && choose != 3) {
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						choose -= 4;
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
					}
				}
				else if (key == 'a' || key == 'A') {
					if (choose != 0 && choose != 4 && choose != 8 && choose != 12) {
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						choose--;
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
					}
				}
				else if (key == 'd' || key == 'D') {
					if (choose != 3 && choose != 7 && choose != 11 && choose != 15) {
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 0);
						choose++;
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start, REC[choose].x_start, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start + 30, REC[choose].y_start, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
						vm_op_line(REC[choose].x_start, REC[choose].y_start + 30, REC[choose].x_start + 30, REC[choose].y_start + 30, 6);
					}
				}
			}

			transform(formula);
			postfix();
			int answer = evalpost();
			char *c1;
			vm_print_letter(75, 45, 0, iitoa(answer, *c1, 10));
			while (1)
			{
				flag = 3;
				vm_print_letter(50, 0, 0, "continue/exit? y/n ? ");
				u32 sign = GETINPUT();
				if (sign == 'y' || sign == 'Y') {
					flag = 1;
					break;
				}
				else {
					flag = 0;
					break;
				}
				if (flag == 1 || flag == 0)break;
			}
		} while (flag == 1);
		clearScreen();
	}


