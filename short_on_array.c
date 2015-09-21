#include <stdio.h>
#include <stdlib.h>
int main()
{
	//char array[10] = {1,2,3,4,5,252,0 };
	char array[10] = {1,2,3,4,5,0,1 };
	int size, i;
	short cast_short;
	short *p_cast_short;
	size = sizeof(array);

	printf("\n");
	for(i=0; i< size; i++){
		printf(" %d", array[i]);
	}
	printf("\n");

		p_cast_short =(short *) &(array[5]);
		cast_short = * p_cast_short;

		printf("%d", cast_short);
		printf("\n");

		cast_short = (short) array[5];
		printf("%d", cast_short);
		printf("\n");

}
