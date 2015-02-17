#include <stdio.h>
#include <stdlib.h>

void display(char  input)
{

	switch (input){
		case 'Y':  
			printf("Y is coming\n");
			break;
		case 'M':
			printf("M is coming\n");
			break;
		case 'L':
			printf("L is coming\n");
			break;
		default:
			printf("Default is coming\n");

	}


}

int main(int argc, char *argv[])
{
	int i;
	for(i=1; argv[i] != NULL; i++){

		display(argv[i][0]);

	}

	

}
