#ifndef __REVERSE_POLISH_NOTATION__HH__
#define __REVERSE_POLISH_NOTATION__HH__

#include   "stdio.h"   
#include   "string.h"   
#include   "math.h"   


void   init();

//�ж��Ƿ�Ϊ����С���� 
int   isnumber(char   ch);

//�ж��Ƿ��ǺϷ��������,�����ԷǷ�����   
int   isch(char   ch);

//�õ�ջ�����ȼ�   
int   get_in_level(char   ch);

//�õ�ջ�����ȼ�  
int   get_out_level(char   ch);

//���ֽ�ջ 
void   push_num(char   ch);

//��ջ����   
void   count();


#endif