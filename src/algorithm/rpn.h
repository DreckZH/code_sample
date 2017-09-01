#ifndef __REVERSE_POLISH_NOTATION__HH__
#define __REVERSE_POLISH_NOTATION__HH__

#include   "stdio.h"   
#include   "string.h"   
#include   "math.h"   


void   init();

//判断是否为数或小数点 
int   isnumber(char   ch);

//判断是否是合法的运算符,即忽略非法符号   
int   isch(char   ch);

//得到栈内优先级   
int   get_in_level(char   ch);

//得到栈外优先级  
int   get_out_level(char   ch);

//数字进栈 
void   push_num(char   ch);

//出栈运算   
void   count();


#endif