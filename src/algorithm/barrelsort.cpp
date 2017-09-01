
//桶排序算法
/*
只适用于不大于max的数值排序<bucktes[max]>。
*/
#include <stdio.h>

int main()
{
	int bucktes[100]={0};//可以最多排序100个小于100的数【限制】
	int t;

	for(int i=0;i<100;i++){
		scanf("%d",&t);//读入100个数
		bucktes[t]++;//bucktes[t]中记下t出现的次数
	}
	
	for(i=0;i<100;i++)
		for(int j=0;j<bucktes[i];j++)
			printf("%d ",i);
}
