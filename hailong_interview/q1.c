#include "stdio.h"
/*
remove repeated elements in an array.
*/
int unique(int array[], int len) // remove repeated elements.
{
	int i = 0, j = 0;
	
	if (len == 0)
		return 0;
	if (len == 1)
		return 1;
	
	for ( ; i < len - 1; i++) //for the first (len - 1) elements.
	{
		while (array[i] == array[i+1])
		{
			i++;
		}
		array[j] = array[i];
		j++;
	}
	if (array[j-1] != array[len - 1]) //for the last element.
	{
		array[j]=array[len - 1];
		j++;		
	}
	for (i = j; i < len; i++) // set remains to -1, not necessary.
		array[i] = -1;
	return j;
}

void printunique(int a[], int len) // print new array;
{
	int m = 0;
	printf("\n-----------------\n");
	printf("new length is %d.\n",len);
	if (len <= 0)
	{
		printf("Array is empty!\n");
		return;
	}
	for ( ; m < len; m++)
	{
		if (m == 0)
		{
			printf("[ %d", a[m]);
		}
		else
		{
			printf(", %d", a[m]);
		}
	}
	printf(" ]\n");
}

void main ()
{
	int A[] = {1,1,3};
	int a1[] = {2,3,4,4,4,5,6};
	int a2[] = {1,1,1,1,3,3,3,4,44,55,55,66,77,88,88};
	int a3[] = {1,2,3,4,5,6};
	int a4[] = {2,2,2,2};
	int a5[] = {1};
	int a6[] = {1,2};
	int a7[] = {1,2,3};
	int a8[] = {1,2,2};
	int a9[] = {7,9,9,31,69,74};
	int * a10 = NULL;
	int a11[] = {1,1,1,3,4,4,4};
	int a12[] = {1,1,2,2,3,3,4,4,5,5};
	int a13[] = {1,2,3,4,5,5,6};
	int ulen = 0;
	int p = 0;
	
	ulen = unique(A, 3);
	printunique(A, ulen);
	
	ulen = unique(a1, 7);
	printunique(a1, ulen);
	
	ulen = unique(a2, 15);
	printunique(a2, ulen);
	
	ulen = unique(a3, 6);
	printunique(a3, ulen);
	
	ulen = unique(a4, 4);
	printunique(a4, ulen);
	
	ulen = unique(a5, 1);
	printunique(a5, ulen);
	
	ulen = unique(a6, 2);
	printunique(a6, ulen);
	
	ulen = unique(a7, 3);
	printunique(a7, ulen);
	
	ulen = unique(a8, 3);
	printunique(a8, ulen);
	
	ulen = unique(a9, 6);
	printunique(a9, ulen);
	
	ulen = unique(a10, 0);
	printunique(a10, ulen);
	
	ulen = unique(a11, 7);
	printunique(a11, ulen);
	
	ulen = unique(a12, 10);
	printunique(a12, ulen);
	
	ulen = unique(a13, 7);
	printunique(a13, ulen);
	
}