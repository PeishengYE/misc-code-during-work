/*
using binary search, find the starting and ending position of a given target value in a given sorted array of integers.
*/

#include "stdio.h"

typedef struct posstruct { //for saving the starting and ending position of the target.
	int starting;
	int ending;
} posstru;

int findstart(int * arr, int begin, int end, int target) 
/*
arr[end] == target. now search for the starting position.
*/
{	
	int fsbegin = begin;
	int fsend = end;
	int fsnow = (begin + end)/2;
	if (arr[fsbegin] == target)
		return fsbegin;
		
	while(fsend - fsbegin != 1)
	{
		if (arr[fsnow] == target)
		{
			if (arr[fsnow-1] != target)
				return fsnow;
			else
			{
				fsend = fsnow;
				fsnow = (fsbegin + fsend)/2;
			}
		}
		else
		{
			if (arr[fsnow + 1] == target)
				return fsnow + 1;
			else
			{
				fsbegin = fsnow;
				fsnow = (fsbegin + fsend)/2;
			}
		}
	}
	if (arr[fsbegin] != target)
		return fsend;
	else
		return fsbegin;
}
int findend(int * arr, int begin, int end, int target)
/*
arr[begin] == target, now search for the ending position.
*/
{
	int febegin = begin;
	int feend = end;
	int fenow = (febegin + feend)/2;
	if (arr[feend] == target)
		return feend;
		
	while ((feend - febegin) != 1)
	{
		if (arr[fenow] == target)
		{
			if (arr[fenow + 1] != target)
				return fenow;
			else
			{
				febegin = fenow;
				fenow = (febegin + feend)/2;
			}
		}
		else
		{
			if(arr[fenow-1] == target)
				return fenow-1;
			else
			{
				feend = fenow;
				fenow = (febegin + feend)/2;
			}
		}
	}
	if (arr[feend] == target)
		return feend;
	else
		return febegin;
}
posstru findit (int * arr, int arrlen, int target)
/*
arr is a sorted array of integers. arrlen is arr's length.
target is the integer to be searched for in arr.
this function return a posstru structure, which hold the starting and ending position of target in arr.
if target is not found, [-1, -1] will be returned.
*/
{
	posstru pos;
	pos.starting = -1;
	pos.ending = -1;
	int first = 0;
	int second = arrlen - 1;
	int now = second/2;
	int found = 0;
	if (arr == NULL)
		return pos;
	if (arrlen <= 0)
		return pos;
	if (arrlen == 1)
	{
		if (arr[0] == target)
		{
			pos.starting = 0;
			pos.ending = 0;			
		}
		return pos;
	}
	if (arrlen == 2)
	{
		if (arr[0] == target)
		{
			pos.starting = 0;
			pos.ending = 0;			
		}
		if (arr[1] == target)
		{
			pos.ending = 1;
			if (pos.starting == -1)
			{
				pos.starting = 1;
			}
		}
		return pos;
	}
	if (arr[0] == target)
	{
		pos.starting = 0;
		if (arr[1] != target)
		{
			pos.ending = 0;
			return pos;
		}
	}
	if (arr[second] == target)
	{
		pos.ending = second;
		if (arr[second-1] != target)
		{
			pos.starting = second;
			return pos;
		}
	}
	while (arr[now] != target)
	{
		if((second - first) == 1)//search is done, target not found.
		{
			return pos;
		}
		if (arr[now] > target)
		{
			second = now;
			now = (first + second)/2;
		}
		else
		{
			first = now;
			now = (first + second)/2;
		}
	}
	//target is found, now search for the starting position.
	pos.starting = findstart(arr, first, now, target);
	//search for the ending position.
	pos.ending = findend(arr, now, second, target);
	return pos;
}

void printpos(posstru pos) //print the starting and ending position.
{
	printf("   [%d, %d]\n", pos.starting, pos.ending);
}

main () //for test.
{
	int a1[6] = {5,7,7,8,8,10};
	int a2[0];
	int a3[1] = {2};
	int a4[2] = {6,8};
	int a5[2] = {8,8};
	int a6[3] = {99, 200, 300};
	int a7[4] = {2,5,6,8};
	int a8[5] = {2,3,3,3,9};
	int a9[6] = {3,4,5,77,77,77};
	int a10[10] = {1,8,8,8,8,8,8,8,8,10};
 	posstru apos;
	apos = findit(a1, 6, 8);
	printf("(a1, 6, 8)");
	printpos(apos);
	apos = findit(a1, 6, 7);
	printf("(a1, 6, 7)");
	printpos(apos);
	apos = findit(a1, 6, 5);
	printf("(a1, 6, 5)");
	printpos(apos);
	apos = findit(a1, 6, 10);
	printf("(a1, 6, 10)");
	printpos(apos);
	apos = findit(a1, 6, 11);
	printf("(a1, 6, 11)");
	printpos(apos);
	apos = findit(a2, 0, 300);
	printf("(a2, 0, 300)");
	printpos(apos);
	apos = findit(a3, 1, 1);
	printf("(a3, 1, 1)");
	printpos(apos);
	apos = findit(a3, 1, 2);
	printf("(a3, 1, 2)");
	printpos(apos);
	apos = findit(a4, 2, 6);
	printf("(a4, 2, 6)");
	printpos(apos);
	apos = findit(a4, 2, 8);
	printf("(a4, 2, 8)");
	printpos(apos);
	apos = findit(a4, 2, 10);
	printf("(a4, 2, 10)");
	printpos(apos);
	apos = findit(a5, 2, 8);
	printf("(a5, 2, 8)");
	printpos(apos);
	apos = findit(a5, 2, 33);
	printf("(a5, 2, 33)");
	printpos(apos);
	apos = findit(a6, 3, 201);
	printf("(a6, 3, 201)");
	printpos(apos);
	apos = findit(a6, 3, 1);
	printf("(a6, 3, 1)");
	printpos(apos);
	apos = findit(a6, 3, 200);
	printf("(a6, 3, 200)");
	printpos(apos);
	apos = findit(a6, 3, 99);
	printf("(a6, 3, 99)");
	printpos(apos);
	apos = findit(a6, 3, 300);
	printf("(a6, 3, 300)");
	printpos(apos);
	apos = findit(a7, 4, 7);
	printf("(a7, 4, 7)");
	printpos(apos);
	apos = findit(a7, 4, 6);
	printf("(a7, 4, 6)");
	printpos(apos);
	apos = findit(a8, 5, 3);
	printf("(a8, 5, 3)");
	printpos(apos);
	apos = findit(a8, 5, 7);
	printf("(a8, 5, 7)");
	printpos(apos);
	apos = findit(a9, 6, 0);
	printf("(a9, 6, 0)");
	printpos(apos);
	apos = findit(a9, 6, 100);
	printf("(a9, 6, 100)");
	printpos(apos);
	apos = findit(a9, 6, 77);
	printf("(a9, 6, 77)");
	printpos(apos);
	apos = findit(a9, 6, 8);
	printf("(a9, 6, 8)");
	printpos(apos);
	apos = findit(a10, 10, 8);
	printf("(a10, 10, 8)");
	printpos(apos);
	apos = findit(a10, 10, 10);
	printf("(a10, 10, 10)");
	printpos(apos);
	apos = findit(a10, 10, 1);
	printf("(a10, 10, 1)");
	printpos(apos);
	apos = findit(a10, 10, 2);
	printf("(a10, 10, 2)");
	printpos(apos);
	apos = findit(a10, 10, 0);
	printf("(a10, 10, 0)");
	printpos(apos);
	apos = findit(a10, 10, 9);
	printf("(a10, 10, 9)");
	printpos(apos);
	apos = findit(a10, 10, 11);
	printf("(a10, 10, 11)");
	printpos(apos);
	apos = findit(a10, 10, 444);
	printf("(a10, 10, 444)");
	printpos(apos);
}