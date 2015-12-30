#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SRC_STRING "/ubisurfer/UbiAgent?action=CURL&view=mobile&url=http://www.jaguar.ca/en/all-models/2016-xf/index.html?utm_source=Yahoo &utm_medium=Native Ad&utm_campaign=XF Launch Mobile&width=512&height=1920 HTTP/1.1"

static void copy_and_remove_space(char *src, char* dest, 
		int dest_length,
		int src_length)
{
	int i = 0, j=0; 

	while((i<src_length)
			&&(j< dest_length - 60)){

		if(src[i] == 0x20){

			dest[j]= '%';
			dest[j+1]= '2';
			dest[j+2]= '0';

			i++;
			j += 3;


		}else{
			dest[j] = src[i];
		    i++;
		    j++;
		}


	}


}

int main()
{
	char * src = SRC_STRING;
	char * dest;
	int length_src, length_dest;
	
	length_src = strlen(src);
	length_dest = length_src + 20;
	dest = malloc(length_dest);

	memset(dest, 0, length_dest);


    copy_and_remove_space(src, dest, 
		length_dest,
		length_src);

	printf("old: \n" );
	printf("%s\n\n", src );

	printf("new: \n" );
	printf("%s\n", dest );


}

