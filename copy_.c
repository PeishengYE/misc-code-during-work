#include <stdio.h>
#include <stdlib.h>
#define BUF_LENGTH 1024
int main(int argc, char *argv[])
{
	FILE *f_read, *f_write;

	char buf[BUF_LENGTH];

	f_read = fopen(argv[1], "r");
	f_write = fopen(argv[1], "w");

	n_read = fread( buf, f_read, 1, BUF_LENGTH, f_read );

	while (n_read == BUF_LENGTH){
		fwrite( buf,1, BUF_LENGTH,f_write );
		n_read = fread(buf, 1, BUF_LENGTH, f_read);
	}

	fwrite(  buf,  1, n_read,f_read);

}
