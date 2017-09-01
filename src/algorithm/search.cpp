
#include "search.h"

int binarysearch(int key,int arr[], int left, int right)
{
	int index = -1,mid = -1;
	int lo = left, hi = right;

	while (lo < hi)
	{
		mid = lo + (hi - lo) / 2;
		if (key < arr[mid])
		{
			hi = mid - 1;
		}

		else if (key > arr[mid])
		{
			lo = mid + 1;
		}

		else
		{
			index = mid;
			break;
		}

	}
	return index;
}

int binary_search(int arr[], int n, int key)
{
	int lo = 0, hi = n - 1, mid;
	while (lo <= hi)
	{
		mid = (lo + hi) / 2; //= lo + (hi-lo)/2

		if (arr[mid] == key)
			return mid;

		if (key > arr[mid])
			lo = mid + 1;
		else
			hi = mid - 1;
	}
	return -1;
}
