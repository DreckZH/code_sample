
#ifndef __my_commonfunction_utility__2016_07_23__
#define __my_commonfunction_utility__2016_07_23__

#define  M   template<typename T>
#define  PR(X, ...) printf("Message "#X" : "  __VA_ARGS__)//PR(1, "x=%g。\n", x);Message 1： x=48


/************************************/
//M FOR template
M bool less(T value1, T value2)
{
	return value1 < value2;
}

M void swap(T arr[], int index1, int index2)
{
	T t = arr[index1];
	arr[index1] = arr[index2];
	arr[index2] = t;
}

M void show(T arr[])
{
	for (int i = 0; i < sizeof(arr) / sizeof(T); i++)
		printf("%d ",arr[i]);
	printf("\n");
}

//positive order
M bool isSorted(T arr[])
{
	for (int i = 1; i < sizeof(arr) / sizeof(T); i++)
		if (less(arr[i], arr[i - 1]))
			return false;
	return true;
}

/************************************/
//for 32 bits trans, max memory : 4 bytes
typedef union
{
	bool			bBool;
	unsigned char	byBuf[4];//4 bytes
	short			snInt;
	unsigned short	usnInt;
	long			snL; 
	unsigned long	usnL;
	float			f;
}UnionDataType;

//float Bytes2Float(const unsigned char* bytes)
//{
//	UnionDataType  unData;
//	unData.snL = 0; //内存清零
//
//	unData.byBuf[0] = bytes[0];  //低位
//	unData.byBuf[1] = bytes[1];  //高位
//
//	float fVal;
//	if (unData.snInt < 0) //负数：补码
//	{
//		unData.byBuf[1] = unData.byBuf[1] & 0x7F;//补码转换，取反加一（除符号位外）
//		unData.usnInt ^= 0xffff;
//		unData.usnInt += 1;
//		fVal = (float)unData.snInt; 
//	}
//	else //正数：原码
//		fVal = (float)unData.snInt; 
//
//	return fVal;
//}
//IEEE754=｛Fraction:1-23,Exponent:23-31,Sign:32｝:R32.23
float Bytes2Float_IEEE754(const unsigned char *bytes)
{
	UnionDataType  unData;
	unData.snL = 0; //内存清零

	unData.byBuf[0] = bytes[0];  //低位
	unData.byBuf[1] = bytes[1];
	unData.byBuf[2] = bytes[2];
	unData.byBuf[3] = bytes[3];  //高位

	return unData.f;//直接返回float类型即为IEEE格式
}

//检查大小端模式
void Init()
{
	int indian = 1;
	bool gBigIndian;
	char* p = (char*)&indian;
	if (p[0] == 0)	{
		gBigIndian = true;
	}
	else	{
		gBigIndian = false;
	}
}

//
void* lsearch(void* key, void *base,
	int n, int elemSize,
	int(*cmpFun)(void*, void*))
{
	for (int i = 0; i < n; i++){
		void* elemAddr = (char*)base + i*elemSize;
		if (comFun(key,elemAddr) == 0)
			return elemAddr;
	}
	return NULL;
}

#endif