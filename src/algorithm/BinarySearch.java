import java.util.Arrays;
//二分查找算法只适用于有序序列查找
public class BinarySearch
{
//rank
	public static int rank(int key,int[] a)
	{
		int lo=0;
		int hi=a.length-1;
		while(lo<=hi)
		{
			int mid=lo+(hi-lo)/2;
			if(key<a[mid])
				hi=mid-1;
			else if(key>a[mid])
				lo=mid+1;
			else//key==a[mid]
				return mid;//return index of key in a[]
		}
		return -1;
	}
//rank end

	//main
	public static void main(String[] args)
	{
		int[] whitelist = In.readInts(args[0]);
		Arrays.sort(whitelist);

		while(!StdIn.isEmpty())
		{
			int key=StdIn.readInt();
			if(rank(key,whitelist)<0)
				StdOut.println(key);
		}
	}
	//main end

}

//java BinarySearch largeW.txt < largeT.txt