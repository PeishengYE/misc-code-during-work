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

#define IMAGE_ID_DELIMITER "_"
#define MAX_IMAGE_ID_LENGTH 60
//#define STR_IMAGE_ID "262150_0_1375"
//#define STR_IMAGE_ID "262150_0_175"
#define STR_IMAGE_ID "262150_200_175"
static  int crc_finder(char *image_id, int image_id_size, char *crc, int *pos_x, int *pos_y)
{

	int ret = -1;
	char image_id_m[MAX_IMAGE_ID_LENGTH];
	char * delimiter_first;
	char * delimiter_sec;
	char num[8];
	char num_length;
	int crc_length;
	/* */
	if(image_id_size > MAX_IMAGE_ID_LENGTH -1){
		return ret;
	}

	memset(image_id_m, 0, MAX_IMAGE_ID_LENGTH);
	memcpy(image_id_m, image_id, image_id_size);

	//printf(" crc finder()>> image_id_m = %s\n", image_id_m);

	delimiter_first = strstr(image_id_m, IMAGE_ID_DELIMITER);
	if(delimiter_first == NULL){
		return ret;
	}

	crc_length = delimiter_first - image_id_m;

	memcpy(crc, image_id_m, crc_length);

	delimiter_sec = strstr(delimiter_first + 1, IMAGE_ID_DELIMITER);

	if(delimiter_sec == NULL){
		return ret;
	}

	num_length = ( delimiter_sec - delimiter_first - 1);
	//printf(" crc finder()>> num_length 1 = %d\n", num_length);

	memset(num, 0, 8);
	memcpy(num, delimiter_first + 1, num_length);

	*pos_x = str_to_int(num);
	num_length = image_id_size - crc_length - num_length - 2;
	//printf(" crc finder()>> num_length 2 = %d\n", num_length);

	memset(num, 0, 8);
	memcpy(num, delimiter_sec + 1, num_length);

	*pos_y = str_to_int(num); 
	ret = 0;
	return ret;

}



int main()
{
	//char *action = STR_SHOW; 
	//char *action = STR_COPY; 
	//char *action = STR_HIDE; 
	char *action = "tempasdfasf:1234:2233"; 
	//char *image_id_str = STR_IMAGE_ID;
	char *image_id_str = "123150_120_123";
	int pos_x, pos_y, length;
	char image_id_str_m[MAX_IMAGE_ID_LENGTH];
	int ret;
	char crc[20];


    find_postion(action, &pos_x, &pos_y);
	printf("pos_x = %d ", pos_x);
	printf("pos_y = %d\n", pos_y);

	length = strlen(image_id_str);
	strcpy(image_id_str_m, image_id_str );

    memset(crc, 0, 20);

    ret = crc_finder(image_id_str_m, length, crc, &pos_x, &pos_y);
	if(ret != 0){

		printf("error on crc finder\n");
		exit(0);
	}else{

		printf(" crc: << %s >>\n", crc);
		printf(" pos_x: %d ", pos_x);
		printf(" pos_y: %d\n", pos_y);
	}




}
