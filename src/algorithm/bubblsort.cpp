

#include <stdio.h>

int main()
{
	int n=0,val;
	char c;	
	int a[20];//�������20��Ԫ��
	printf("input sort number:");
	while(scanf("%d%c",&val,&c)){	
		if(c=='\n')
			break;	
		a[n] = val;
		n++;//Ҫ�����Ԫ�صĸ���
	}	

	int t;
	for(int i=0;i<n-1;i++)//Ҫ����n-1�ˣ�ÿ��ð��1��Ԫ��
	{
		for(int j=0;j<n-i;j++)//ÿ�˱Ƚ�n-i����������i���Ѿ�ð���ź�
		{
			if(a[j] < a[j+1]){
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}
		}
	}
	
	printf("result:");
	for(int i=0;i<n-1;i++){
		printf("%d ",a[i]);
	}
}