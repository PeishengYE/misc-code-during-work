#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define REMOVE_CHARS_NUMBER 166
#define LINE_LENGTH 300
static char * remove_chars(char *line)
{

	char *buf;
	int length;


	length = strlen(line);

	buf = malloc(length + 10);
	memset(buf, 0,  length + 10);

    //printf("line  lengthe = %d", length);

	if( length > REMOVE_CHARS_NUMBER){
	   memcpy(buf, line + REMOVE_CHARS_NUMBER, length - REMOVE_CHARS_NUMBER );
	}else{

     printf("length = %d ", length);
	}

	return buf;

}

//int main(char *argv[], int argc)
int main(int argc, char *argv[] )
{
	FILE *fp_src, *fp_dst;
	char *line;
	char *new_line;
	int i = 0;

	line = malloc(LINE_LENGTH);
	fp_src = fopen(argv[1], "r");
	if( fp_src == NULL){
          printf("open source file failed,");
		  return -1;
	}
	fp_dst = fopen(argv[2], "w");
	
	while (fgets(line, sizeof(line), fp_src)){

        printf("line :: %s ", line);
		//i++;
        //printf("line %d reading ", i);
        new_line =  remove_chars(line);
		fputs(new_line, fp_dst);
		free(new_line);
	}

}
