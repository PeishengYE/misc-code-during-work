
/* this is about a basic programming skill , 
 * it should be finished during 20 seconds */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GetMemory(char *p, int num)
{
	p = (char *)malloc(sizeof(char)*num);
};
int main()
{
	char *str = NULL;
	GetMemory(str, 100);
	strcpy(str, "hello");
	return 0;
}
