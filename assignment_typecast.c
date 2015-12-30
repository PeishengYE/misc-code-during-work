#include <stdio.h>

int main()
{
	unsigned int a = 0xFFFFFFF7;
	unsigned char i = (unsigned char)a;
	char * b = (char *)&a;
	printf("%08x, %08x\n", i, *b);
}
