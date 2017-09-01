

#include <stdio.h>

int main()
{
	int n=0,val;
	char c;	
	int a[20];//最多排序20个元素
	printf("input sort number:");
	while(scanf("%d%c",&val,&c)){	
		if(c=='\n')
			break;	
		a[n] = val;
		n++;//要排序的元素的个数
	}	

	int t;
	for(int i=0;i<n-1;i++)//要进行n-1趟，每趟冒泡1个元素
	{
		for(int j=0;j<n-i;j++)//每趟比较n-i个数，其他i个已经冒泡排好
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