#include <stdio.h>
#include <stdlib.h>

//int main(int argc, char *argv[])
int main( char *argv[], int argc)
{
	int i = 0;
	while (argv[i++] != NULL){
	   printf("argv[%d] : <%s>\n", i,  argv[i]);
	}

}
