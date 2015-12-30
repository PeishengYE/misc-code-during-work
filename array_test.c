#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_array_1( int (*p)[10])
{
	int i;
	for(i=0; i<10; i++){

		printf("%d\n", (*p)[i]);
	}

}

void print_array_2( int (*p)[11])
{
	int i;
	for(i=0; i<10; i++){

		printf("%d\n", (*p)[i]);
	}

}


int main()
{

	int array[10];
	int i;

	for(i=0; i<10; i++){

		array[i] = i;
	}

   print_array_1( &array);
   print_array_2( &array);

}
