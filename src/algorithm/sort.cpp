
#include "sort.h"

void quicksort(int arr[],int left,int right)
{
	int baselinedata, i, j;
	if(left > right)
		return;
	
	baselinedata = arr[left];//选取基准数，可随便选取
	i=left;
	j=right;
	
	while(i != j)
	{
		while (arr[j] >= baselinedata && i<j)//从右边开始找出第一个小于基准值的元素
			j--;
		while (arr[i] <= baselinedata && i<j)//从左边开始找出第一个大于基准值的元素
			i++;
			
		if(i < j){//左右交换，使左边的值都小于基准值，右边的都大于基准值
			int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
		}
	}
	//此时i == j，基准值归位到i处
	arr[left] = arr[i];
	arr[i] = baselinedata;
	
	//递归i的左右部分
	quicksort(arr,left, i - 1);
	quicksort(arr,i + 1, right);
	
}

void selectsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int index_min = left,min = arr[index_min];//index_min为最小值的索引，初始化为左边第一个元素
	for (int i = left; i <= right; i++)
	{
		index_min = i;//从左开始查找数组中的最小值，记录其索引值
		for (int j = index_min + 1; j <= right; j++)
		{
			if (arr[j] < arr[index_min])
			{
				index_min = j;
			}
		}
		min = arr[index_min];//找到最小值后，交换位置，排列在数组左边第一位置处，继续从第二位置的右边查找第二小元素，排列在左边第二个位置，依此类推
		arr[index_min] = arr[i];
		arr[i] = min;		
	}
}


void selectsort_recur(int arr[], int left, int right)
{
	if (left > right)
		return;
	int min = arr[right];//从最右边元素开始比较，查找最小值，使其冒泡到左边第一位置处
	for (int i = right; i >= left; i--)
	{
		if (arr[i] < arr[i - 1])
		{
			min = arr[i];
			arr[i] = arr[i-1];
			arr[i-1] = min;
		}
	}

	selectsort(arr, left + 1, right);//第一位置处已经是最小值，递归右边，依次找到第二小、第三小元素
}

//将右边选出的每个元素与左边有序元素冒泡比较，之后插入
void insertsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int t = arr[left];
	for (int i = left + 1; i <= right; i++)//从第2个元素开始，依次摘取右边的元素
	{	
		for (int j = i; j >= left; j--)//将摘取的元素在左边已经排序的数列中冒泡插入
		{
			if (arr[j] < arr[j - 1])
			{
				t = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = t;
			}
		}
	}
}


void insert_sort(int arr[], int n)
{
	int i,j;	
	int tmp;
	for(i=1; i <= n; i++)
	{
		tmp = arr[i];
		j = i-1;
		while (j >= 0 && tmp < arr[j])
			arr[j + 1] = arr[j--];
		arr[j + 1] = tmp;
	}
}
//arr[h+1,h+2,h+3,h+4,h+5,,,,,,]与
//arr[  1,  2,  3,  4,  5,,,,,,] 间的比较和位置互换
void shellsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int len = right - left + 1;
	int h = 1;
	while (h < len / 3)//分成h数组
	{
		h = 3 * h + 1;//1,4,13,40,121,364,1093......
	}

	int t = arr[left];
	while (h >= 1)//步长缩小（h数组的h变小）
	{
		for (int i = h; i < len; i++)
		{
			for (int j = i; j >= h; j -= h)//作用是使a[2h+x],a[h+x]比较互换，之后再使a[h+x]，a[0+x]比较互换
			{
				if (arr[j] < arr[j - h])//比较a[h+x]和a[x],互换位置,段之间的比较，可使大元素移动h距离
				{
					t = arr[j];
					arr[j] = arr[j - h];
					arr[j - h] = t;
				}
			}
		}
		h = h / 3;
	}
}

//假设arr1，arr2已经有序，归并后存入res
void mergesort(int arr1[],int len1, int arr2[],int len2,int *res)// int left, int mid, int right,
{
	int *arrt = new int[ len1+len2 ];
	int *p = arrt;
	for (int i = 0; i < len1; i++){
		*(p++) = *(arr1++);
	}
	for (int j = 0; j < len2; j++){
		*(p++) = *(arr2++);
	}

	int i = 0, mid = len1, j = mid;
	for (int k = 0; k <= len1+len2-1; k++)
	{
		if (i > mid)//左半部分用尽，取右边，取完后移动指向
		{
			res[k] = arrt[j++];
		}
		else if (j > len1 + len2 - 1)//右半部分用尽，取左边，取完后移动指向
		{
			res[k] = arrt[i++];
		}
		else if (arrt[i] > arrt[j])//左右比较取小者，取完后移动指向
		{
			res[k] = arrt[j++];
		}
		else//同样，左右比较取小者，取完后移动指向
		{
			res[k] = arrt[i++];
		}
	}

}
//将归并后的结果存回arr
void mergesort(int arr[], int lo, int mid, int hi)// int left, int mid, int right,
{
	int i = lo,j = mid+1;
	/*int *parr = new int[hi-lo+1];*/
	int parr[50];
	for (int k = lo; k <= hi; k++)
		parr[k] = arr[k];

	for (int k = lo; k <= hi; k++)
	{
		if (i > mid)//左半部分用尽，取右边，取完后移动指向
			arr[k] = parr[j++];
		else if (j > hi)//右半部分用尽，取左边，取完后移动指向
			arr[k] = parr[i++];
		else if (parr[i] > parr[j])//左右比较取小者，取完后移动指向
			arr[k] = parr[j++];
		else//同样，左右比较取小者，取完后移动指向
			arr[k] = parr[i++];
	}
	//delete parr;
}

int minof(int a, int b)
{
	return (a > b) ? b : a;
}

//自底向上归并，1-1，2-2，4-4，8-8归并
void mergesortBU(int arr[],int len )
{
	//int len = sizeof(arr) / sizeof(int); ===1 ###
	for (int sz = 1; sz < len; sz *= 2)
		for (int lo = 0; lo < len - sz; lo += (sz * 2))
			mergesort(arr, lo, lo + sz - 1, minof(lo + sz * 2 - 1, len - 1));
}

//自顶向下，递归划分左右，排序后归并
void mergesortUB(int arr[],int lo,int hi)
{
	if (hi <= lo)
		return;
	int mid = lo + (hi - lo) / 2;
	mergesortUB(arr, lo, mid);
	mergesortUB(arr, mid + 1, hi);
	mergesort(arr, lo, mid, hi);
}