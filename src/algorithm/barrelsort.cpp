
//Ͱ�����㷨
/*
ֻ�����ڲ�����max����ֵ����<bucktes[max]>��
*/
#include <stdio.h>

int main()
{
	int bucktes[100]={0};//�����������100��С��100���������ơ�
	int t;

	for(int i=0;i<100;i++){
		scanf("%d",&t);//����100����
		bucktes[t]++;//bucktes[t]�м���t���ֵĴ���
	}
	
	for(i=0;i<100;i++)
		for(int j=0;j<bucktes[i];j++)
			printf("%d ",i);
}
