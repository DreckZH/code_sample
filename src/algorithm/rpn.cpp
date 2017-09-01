
#include   "rpn.h"   

#define   MAX   100				//ջ�����
double		rpnList[MAX];		//�������ջ   
int			p_rpnList;			//ջ��ָ��
char		opStack[MAX];		//�������ջ   
int			p_opStack;			//ջ��ָ�� 
double		isfloat = 0;    //�ж��Ƿ����С����   
int			minus;          //�ж��Ƿ������ֱ�����ĸ��������Ǽ���   
char		ch, last;       //�ֱ�浱ǰ������ַ�����һ��������ַ�   

void init() 
{
	memset(rpnList, 0, sizeof(rpnList));
	memset(opStack, 0, sizeof(opStack));
	opStack[0] = '(';
	p_rpnList = 0;
	p_opStack = 1;
	ch = last = '+';
	minus = 0;             //��ʹ������Ϊ����   
}

//�ж��Ƿ�Ϊ����С���� 
int   irpnListber(char   ch)
{
	int   n;
	n = ch - '0';                       //���ַ�ת��������   
	if (n >= 0 && n <= 9)
		return   1;

	if (ch == '.')                 //�����С����Ļ�,��isfloat�������   
	{
		isfloat = 0.1;
		return   2;
	}
	return   0;
}

//�ж��Ƿ��ǺϷ��������,�����ԷǷ�����   
int   iopStack(char   ch)
{
	if (ch == '(' || ch == ')' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == ';')
		return   1;
	else
		return   0;
}

//�õ�ջ�����ȼ�   
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
//�õ�ջ�����ȼ�  
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

//���ֽ�ջ 
void   push_num(char   ch)
{
	int   i;
	double   a;
	a = ch - '0';             //���ַ�ת��������   
	if (minus)           //���������־����minusΪ1�Ļ�,���ɸ���,ͬʱ��������־��ԭΪ0   
	{
		a *= -1;
		minus = 0;
	}
	if (irpnListber(last) != 1)       //�����һ���ַ�Ҳ������,���ʾ������������һ����.   
	{
		rpnList[p_rpnList++] = a;//����������ֵĻ�,��ʾ����һ���µ�����,����������ջ   
	}
	else
	{
		i = p_rpnList - 1;
		if (isfloat == 0)
			rpnList[i] = rpnList[i] * 10 + a;       //�������С��,���10���ϴ���   
		else
		{
			rpnList[i] = rpnList[i] + isfloat*a;         //�����С��,�����С����־����isfloat,����ԭ��   
			isfloat = isfloat*0.1;     //ͬʱС����־��������0.1   
		}
	}
}

//��ջ����   
void   count()
{
	double   x, y, z;
	char   ch;
	int   i, j;
	p_opStack--;
	ch = opStack[p_opStack];                     //�������ջ   
	if (ch == '(')   return;
	p_rpnList--;
	y = rpnList[p_rpnList];
	p_rpnList--;
	x = rpnList[p_rpnList]; //��������ջ   
	if (ch == '+')
		z = x + y;         //����   
	else   if (ch == '-')
		z = x - y;
	else   if (ch == '*')
		z = x*y;
	else   if (ch == '/')
		z = x / y;
	else   if (ch == '^')                 //��һ��ѭ��������ָ������   
	{
		j = (int)y;
		z = 1;
		for (i = 1; i <= j; i++)   z *= x;
	}
	rpnList[p_rpnList++] = z;         //�������ջ   
}


int zh_main()     //***********       ������       **************   
{

	init();       //���ó�ʹ������   
	scanf("%c", &ch);
	while (1)
	{
		if (irpnListber(ch) == 1)       //���������,��ջ   
		{
			push_num(ch);
		}
		else   if (iopStack(ch) == 1)     //������ַ�,���һ�������   
		{
			if (ch == ')')
			{
				while (opStack[p_opStack] != '(')   count();     //�����')',���㵽ջ�г���'('Ϊֹ.   
			}
			else   if (ch == ';')
			{
				while (p_opStack != 0)   count();       //�����';',���㵽ջ��.   
				break; //�˳�ѭ��   
			}
			else
			{
				if (ch == '-'   &&   iopStack(last))       //�������ǰ����һ�������,˵������һ�����Ŷ����Ǽ���   
				{
					minus = 1;
				}
				else
				{
					while (get_out_level(ch) <= get_in_level(opStack[p_opStack - 1]))     //�ж��Ż���������   
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
	printf("The   answer   is   :   %.2lf\n", rpnList[0]);     //������   

	return   0;
}

