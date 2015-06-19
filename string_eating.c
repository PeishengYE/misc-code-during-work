#include <stdio.h>
#include <stdlib.h>
int eat_space(char **ptr)
{
	//char *tmp;
	char *tmp2 = *ptr;

	//while(*tmp2 != 0 && strchr(tmp2, " ") != NULL ){
	while(*tmp2 != 0 && strchr(tmp2, ' ') != NULL ){
			tmp2++;
	}

	*ptr = tmp2;
	return 0;
}

int main()
{
	//char *str = "    12345678";
	char str[] = "    12345678";

	printf("\n%s\n", str);
	eat_space(&str);

	printf("%s\n", str);
	return 0;
}
