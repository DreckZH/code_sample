
#ifndef __my_algo4_sort__2016_07_23__
#define __my_algo4_sort__2016_07_23__

int minof(int a, int b);

//选择标准值，左边元素小于标准值，右边大于标准值
void quicksort(int arr[],int left,int right);

//选择最小元素，依次排列
void selectsort(int arr[], int left, int right);
void selectsort_recur(int arr[], int left, int right);

//左边排好序，依次从右侧摘取一个元素，在左序列中冒泡插入
void insertsort(int arr[], int left, int right);

//h有序数组，将数组分成n个h数组，对每个h数组排序，大数移动到另一个大的h数组中，最后用插入排序总数组
void shellsort(int arr[], int left, int right);

//归并arr1和arr2到res
void mergesort(int arr1[], int len1, int arr2[], int len2, int *res);
void mergesort(int arr[], int lo, int mid, int hi);// int left, int mid, int right,
void mergesortBU(int arr[],int len);
void mergesortUB(int arr[], int lo, int hi);

#endif