
#include "sort.h"

void quicksort(int arr[],int left,int right)
{
	int baselinedata, i, j;
	if(left > right)
		return;
	
	baselinedata = arr[left];//ѡȡ��׼���������ѡȡ
	i=left;
	j=right;
	
	while(i != j)
	{
		while (arr[j] >= baselinedata && i<j)//���ұ߿�ʼ�ҳ���һ��С�ڻ�׼ֵ��Ԫ��
			j--;
		while (arr[i] <= baselinedata && i<j)//����߿�ʼ�ҳ���һ�����ڻ�׼ֵ��Ԫ��
			i++;
			
		if(i < j){//���ҽ�����ʹ��ߵ�ֵ��С�ڻ�׼ֵ���ұߵĶ����ڻ�׼ֵ
			int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
		}
	}
	//��ʱi == j����׼ֵ��λ��i��
	arr[left] = arr[i];
	arr[i] = baselinedata;
	
	//�ݹ�i�����Ҳ���
	quicksort(arr,left, i - 1);
	quicksort(arr,i + 1, right);
	
}

void selectsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int index_min = left,min = arr[index_min];//index_minΪ��Сֵ����������ʼ��Ϊ��ߵ�һ��Ԫ��
	for (int i = left; i <= right; i++)
	{
		index_min = i;//����ʼ���������е���Сֵ����¼������ֵ
		for (int j = index_min + 1; j <= right; j++)
		{
			if (arr[j] < arr[index_min])
			{
				index_min = j;
			}
		}
		min = arr[index_min];//�ҵ���Сֵ�󣬽���λ�ã�������������ߵ�һλ�ô��������ӵڶ�λ�õ��ұ߲��ҵڶ�СԪ�أ���������ߵڶ���λ�ã���������
		arr[index_min] = arr[i];
		arr[i] = min;		
	}
}


void selectsort_recur(int arr[], int left, int right)
{
	if (left > right)
		return;
	int min = arr[right];//�����ұ�Ԫ�ؿ�ʼ�Ƚϣ�������Сֵ��ʹ��ð�ݵ���ߵ�һλ�ô�
	for (int i = right; i >= left; i--)
	{
		if (arr[i] < arr[i - 1])
		{
			min = arr[i];
			arr[i] = arr[i-1];
			arr[i-1] = min;
		}
	}

	selectsort(arr, left + 1, right);//��һλ�ô��Ѿ�����Сֵ���ݹ��ұߣ������ҵ��ڶ�С������СԪ��
}

//���ұ�ѡ����ÿ��Ԫ�����������Ԫ��ð�ݱȽϣ�֮�����
void insertsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int t = arr[left];
	for (int i = left + 1; i <= right; i++)//�ӵ�2��Ԫ�ؿ�ʼ������ժȡ�ұߵ�Ԫ��
	{	
		for (int j = i; j >= left; j--)//��ժȡ��Ԫ��������Ѿ������������ð�ݲ���
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
//arr[h+1,h+2,h+3,h+4,h+5,,,,,,]��
//arr[  1,  2,  3,  4,  5,,,,,,] ��ıȽϺ�λ�û���
void shellsort(int arr[], int left, int right)
{
	if (left > right)
		return;

	int len = right - left + 1;
	int h = 1;
	while (h < len / 3)//�ֳ�h����
	{
		h = 3 * h + 1;//1,4,13,40,121,364,1093......
	}

	int t = arr[left];
	while (h >= 1)//������С��h�����h��С��
	{
		for (int i = h; i < len; i++)
		{
			for (int j = i; j >= h; j -= h)//������ʹa[2h+x],a[h+x]�Ƚϻ�����֮����ʹa[h+x]��a[0+x]�Ƚϻ���
			{
				if (arr[j] < arr[j - h])//�Ƚ�a[h+x]��a[x],����λ��,��֮��ıȽϣ���ʹ��Ԫ���ƶ�h����
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

//����arr1��arr2�Ѿ����򣬹鲢�����res
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
		if (i > mid)//��벿���þ���ȡ�ұߣ�ȡ����ƶ�ָ��
		{
			res[k] = arrt[j++];
		}
		else if (j > len1 + len2 - 1)//�Ұ벿���þ���ȡ��ߣ�ȡ����ƶ�ָ��
		{
			res[k] = arrt[i++];
		}
		else if (arrt[i] > arrt[j])//���ұȽ�ȡС�ߣ�ȡ����ƶ�ָ��
		{
			res[k] = arrt[j++];
		}
		else//ͬ�������ұȽ�ȡС�ߣ�ȡ����ƶ�ָ��
		{
			res[k] = arrt[i++];
		}
	}

}
//���鲢��Ľ�����arr
void mergesort(int arr[], int lo, int mid, int hi)// int left, int mid, int right,
{
	int i = lo,j = mid+1;
	/*int *parr = new int[hi-lo+1];*/
	int parr[50];
	for (int k = lo; k <= hi; k++)
		parr[k] = arr[k];

	for (int k = lo; k <= hi; k++)
	{
		if (i > mid)//��벿���þ���ȡ�ұߣ�ȡ����ƶ�ָ��
			arr[k] = parr[j++];
		else if (j > hi)//�Ұ벿���þ���ȡ��ߣ�ȡ����ƶ�ָ��
			arr[k] = parr[i++];
		else if (parr[i] > parr[j])//���ұȽ�ȡС�ߣ�ȡ����ƶ�ָ��
			arr[k] = parr[j++];
		else//ͬ�������ұȽ�ȡС�ߣ�ȡ����ƶ�ָ��
			arr[k] = parr[i++];
	}
	//delete parr;
}

int minof(int a, int b)
{
	return (a > b) ? b : a;
}

//�Ե����Ϲ鲢��1-1��2-2��4-4��8-8�鲢
void mergesortBU(int arr[],int len )
{
	//int len = sizeof(arr) / sizeof(int); ===1 ###
	for (int sz = 1; sz < len; sz *= 2)
		for (int lo = 0; lo < len - sz; lo += (sz * 2))
			mergesort(arr, lo, lo + sz - 1, minof(lo + sz * 2 - 1, len - 1));
}

//�Զ����£��ݹ黮�����ң������鲢
void mergesortUB(int arr[],int lo,int hi)
{
	if (hi <= lo)
		return;
	int mid = lo + (hi - lo) / 2;
	mergesortUB(arr, lo, mid);
	mergesortUB(arr, mid + 1, hi);
	mergesort(arr, lo, mid, hi);
}