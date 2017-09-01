
#include   "rpn.h"   

#define   MAX   100				//栈的深度
double		rpnList[MAX];		//定义对象栈   
int			p_rpnList;			//栈顶指针
char		opStack[MAX];		//定义符号栈   
int			p_opStack;			//栈顶指针 
double		isfloat = 0;    //判断是否出现小数点   
int			minus;          //判断是否是数字本身带的负数而不是减号   
char		ch, last;       //分别存当前输入的字符和上一次输入的字符   

void init() 
{
	memset(rpnList, 0, sizeof(rpnList));
	memset(opStack, 0, sizeof(opStack));
	opStack[0] = '(';
	p_rpnList = 0;
	p_opStack = 1;
	ch = last = '+';
	minus = 0;             //初使化输入为正数   
}

//判断是否为数或小数点 
int   irpnListber(char   ch)
{
	int   n;
	n = ch - '0';                       //将字符转换成数字   
	if (n >= 0 && n <= 9)
		return   1;

	if (ch == '.')                 //如果是小数点的话,用isfloat变量标记   
	{
		isfloat = 0.1;
		return   2;
	}
	return   0;
}

//判断是否是合法的运算符,即忽略非法符号   
int   iopStack(char   ch)
{
	if (ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == ';')
		return   1;
	else
		return   0;
}

//得到栈内优先级   
int   get_in_level(char   ch)
{
	if (ch == '(')
		return   0;
	if (ch == '+' || ch == '-')
		return   1;
	if (ch == '*' || ch == '/')
		return   2;
	if (ch == '^')
		return   3;
	return   0;
}
//得到栈外优先级  
int   get_out_level(char   ch)
{
	if (ch == '(')
		return   4;
	if (ch == '+' || ch == '-')
		return   1;
	if (ch == '*' || ch == '/')
		return   2;
	if (ch == '^')
		return   4;
	return   0;
}

//数字进栈 
void   push_num(char   ch)
{
	int   i;
	double   a;
	a = ch - '0';             //将字符转换成数字   
	if (minus)           //如果负数标志变量minus为1的话,则变成负数,同时将负数标志还原为0   
	{
		a *= -1;
		minus = 0;
	}
	if (irpnListber(last) != 1)       //如果上一个字符也是数字,则表示这两个数字是一个数.   
	{
		rpnList[p_rpnList++] = a;//如果不是数字的话,表示这是一个新的数字,将此数字入栈   
	}
	else
	{
		i = p_rpnList - 1;
		if (isfloat == 0)
			rpnList[i] = rpnList[i] * 10 + a;       //如果不是小数,则乘10加上此数   
		else
		{
			rpnList[i] = rpnList[i] + isfloat*a;         //如果是小数,则乘以小数标志变量isfloat,加上原数   
			isfloat = isfloat*0.1;     //同时小数标志变量乘以0.1   
		}
	}
}

//出栈运算   
void   count()
{
	double   x, y, z;
	char   ch;
	int   i, j;
	p_opStack--;
	ch = opStack[p_opStack];                     //运算符出栈   
	if (ch == '(')   return;
	p_rpnList--;
	y = rpnList[p_rpnList];
	p_rpnList--;
	x = rpnList[p_rpnList]; //运算数出栈   
	if (ch == '+')
		z = x + y;         //计算   
	else   if (ch == '-')
		z = x - y;
	else   if (ch == '*')
		z = x*y;
	else   if (ch == '/')
		z = x / y;
	else   if (ch == '^')                 //用一个循环来计算指数运算   
	{
		j = (int)y;
		z = 1;
		for (i = 1; i <= j; i++)   z *= x;
	}
	rpnList[p_rpnList++] = z;         //将结果入栈   
}


int zh_main()     //***********       主函数       **************   
{

	init();       //调用初使化过程   
	scanf("%c", &ch);
	while (1)
	{
		if (irpnListber(ch) == 1)       //如果是数字,入栈   
		{
			push_num(ch);
		}
		else   if (iopStack(ch) == 1)     //如果是字符,则进一步分情况   
		{
			if (ch == ')')
			{
				while (opStack[p_opStack] != '(')   count();     //如果是')',则算到栈中出现'('为止.   
			}
			else   if (ch == ';')
			{
				while (p_opStack != 0)   count();       //如果是';',则算到栈底.   
				break; //退出循环   
			}
			else
			{
				if (ch == '-'   &&   iopStack(last))       //如果负号前又是一个运算符,说明这是一个负号而不是减号   
				{
					minus = 1;
				}
				else
				{
					while (get_out_level(ch) <= get_in_level(opStack[p_opStack - 1]))     //判断优化级并运算   
					{
						count();
					}
					opStack[p_opStack++] = ch;
				}
			}
		}


		last = ch;
		scanf("%c", &ch);
	}
	printf("The   answer   is   :   %.2lf\n", rpnList[0]);     //输出结果   

	return   0;
}

