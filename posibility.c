#include <stdio.h>
#include <stdlib.h>
#define MAXIMUM_LOOP 1000
#define MAXIMUM 100
int main()
{
	int i=0, x,y, count = 0;
	//double pi = 0;

	for (i = 0; i < MAXIMUM_LOOP; i++){
	   x = random()%MAXIMUM;
	   y = random()%MAXIMUM;

	   if (x*x + y*y < MAXIMUM*MAXIMUM){
		 count ++;
	   }

	}

	printf("%d\n", count);
	//pi = count*4.0/MAXIMUM_LOOP;
	//printf("%f\n", pi);
}
