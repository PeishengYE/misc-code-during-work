#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXIMUM_BASE 5

#define STR_SHOW "show:12:444"
#define STR_COPY "copy:0:893"
#define STR_HIDE "hide"
#define DIGIT_DELIMTER ":"

static int str_to_int(char *str)
{
	int i, ret = 0;
	int length ;
	char letter;
	int digit_start;
	char digit_sum = 0;

	int base[MAXIMUM_BASE] = { 1, 10, 100, 1000, 10000 };

	length = strlen(str);
	//ubi_debug("[UBI_YEP] length =  %d\n", length);
	/* find the number of continuous digit */
	for( i = 0; i< length; i++){
		letter = str[i];

		if((letter >= '0') && (letter <= '9')){
			digit_start = i;
			break;

		}

	}
	//ubi_debug("[UBI_YEP] start from   %d\n", digit_start);

	for( i = digit_start; i < length; i++){

		letter = str[i];
		if((letter >= '0') && (letter <= '9')){
			digit_sum ++;
		}else{
			break;
		}
	}

	//ubi_debug("[UBI_YEP] "total digit:   %d\n", digit_sum);

	if(digit_sum >= MAXIMUM_BASE){
		digit_sum = MAXIMUM_BASE;
	}

	/* calculate the valus, and return */
	for(i = 0 ; i <  digit_sum; i++){
		letter = str[digit_start + i];

		ret += base[ digit_sum - 1 - i ] *(letter - '0');

	}

	return ret;

}

static int find_postion(char *action, int *x, int *y)
{

	char *delimiter_first;
	char *delimiter_sec;
	char number[10];
	char *start;
	char copy_length;

	delimiter_first = strstr(action, DIGIT_DELIMTER);
	if(delimiter_first != NULL){


	    delimiter_sec = strstr(delimiter_first + 1, DIGIT_DELIMTER);

		if(delimiter_sec != NULL){
			memset(number, 0, 10);
			start = delimiter_first + 1;
			copy_length = delimiter_sec - start; 
			memcpy(number,start , copy_length);

			*x = str_to_int(number);

			memset(number, 0, 10);
			start = delimiter_sec + 1;
			strcpy(number, start);

			*y = str_to_int(number);

		}

	}


}


int main()
{
	//char *action = STR_SHOW; 
	//char *action = STR_COPY; 
	//char *action = STR_HIDE; 
	char *action = "tempasdfasf:1234:2233"; 
	int pos_x, pos_y;


    find_postion(action, &pos_x, &pos_y);
	printf("pos_x = %d ", pos_x);
	printf("pos_y = %d\n", pos_y);

}
