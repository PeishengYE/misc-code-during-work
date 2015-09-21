#include <stdio.h>
#include <stdlib.h>
#include <string.h>


const char *string_array[] = {
	"IEBAQCBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6",
	"AAAAAAAAECAwQFBgcICQoL/8QAtREAAgE",
  "FBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUv",
   "2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm",
   NULL,
   NULL
};


const char *string_array_2[] = {
	"AAAAAAAAECAwQFBgcICQoL/8QAtREAAgE",
   //NULL,
   NULL,
   NULL
};


static char **html_packets = NULL;

static int ubi_min(int num1, int num2)
{
	return (num1< num2)?num1:num2;
}

static void copy_the_last_string(char *output_mem, int length)
{
	int i, cur_packet_remain_length;
	int packet_size, real_length, remain_size, copy_size, src_size ;
	char * packet_start, *packet_end ;
    char *packet_copy_start  ;

	char *array_mem[10];
	int array_size[10];
	char *mem, *src;


	memset(output_mem, 0, length);
	memset(array_mem, 0, sizeof(array_mem));
	memset(array_size, 0, sizeof(array_size));


	real_length = length - 1;
	remain_size = real_length;

	i = 0;

	if( html_packets[i] == NULL){
		return ;
	}

 /* find the last packet recved */
	while(html_packets[i] != NULL) i++;

	printf("we have %d packets to handle\n", i);

	/* make i to point to the last packet */
	i--;

	while(remain_size > 0
			&& i >= 0){

		/*  */
			packet_start = html_packets[i] ;
			packet_size = strlen(html_packets[i]) ;

			copy_size = ubi_min(packet_size, remain_size);

		    packet_end = packet_start + packet_size;
            packet_copy_start = packet_end - copy_size ;

			mem = malloc(copy_size + 2);
			memset(mem, 0, copy_size + 2);
			memcpy(mem, packet_copy_start, copy_size);

            array_mem[i] = mem;
			array_size[i] = copy_size;
			remain_size -= copy_size;
			i--;

	}

	mem = output_mem;
	for(i = 0; i< 10; i++){

            if(array_mem[i] != NULL){

				src = array_mem[i];
				src_size = array_size[i];

				memcpy(mem, src, src_size );
				mem += src_size;

				free(src);

			}

	}



}

#define LENGTH_MEM 200

int main(int argc, char **argv)
{

	char *mem;
	int length;

    //html_packets = string_array_2;
    html_packets = string_array;


	mem = malloc(LENGTH_MEM);
	length = LENGTH_MEM;

   copy_the_last_string(mem,  length);
   printf("string : %s\n", mem);
   printf("string length : %d\n", strlen(mem));
   free(mem);

	return 0;
}

