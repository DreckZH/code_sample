
#ifndef __my_algo4_sort__2016_07_23__
#define __my_algo4_sort__2016_07_23__

int minof(int a, int b);

//ѡ���׼ֵ�����Ԫ��С�ڱ�׼ֵ���ұߴ��ڱ�׼ֵ
void quicksort(int arr[],int left,int right);

//ѡ����СԪ�أ���������
void selectsort(int arr[], int left, int right);
void selectsort_recur(int arr[], int left, int right);

//����ź������δ��Ҳ�ժȡһ��Ԫ�أ�����������ð�ݲ���
void insertsort(int arr[], int left, int right);

//h�������飬������ֳ�n��h���飬��ÿ��h�������򣬴����ƶ�����һ�����h�����У�����ò�������������
void shellsort(int arr[], int left, int right);

//�鲢arr1��arr2��res
void mergesort(int arr1[], int len1, int arr2[], int len2, int *res);
void mergesort(int arr[], int lo, int mid, int hi);// int left, int mid, int right,
void mergesortBU(int arr[],int len);
void mergesortUB(int arr[], int lo, int hi);

#endif