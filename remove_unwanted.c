#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HTML_TAG_STARTER '<'
#define ERROR_PACKET "50950=<input type='hidden' val"
#define GOOD_PACKET "<input type='hidden' val"
#define ABNORMAL_PACKET "asdfasfdasfdinput type='hidden' val"




static void check_if_html_tag_starter_at_first(char **mem, int *size)
{

	char *html_tag_star = *mem;
	int size_m = *size;
	int i = 0;

	while((*html_tag_star != HTML_TAG_STARTER) 
			&&( i < size_m)) {

	   html_tag_star++;
	   i++;
	}

	if(i < size_m){

	    *mem = html_tag_star;
		*size = size_m -i;
	}

}

int main()
{
	int length;
	char *packet = GOOD_PACKET;
	//char *packet = ERROR_PACKET;
	//char *packet = ABNORMAL_PACKET;
	char new_packet[100];
	length = strlen(packet);
	printf("oring: \t%s\n", packet);

	memset(new_packet, 0, 100);
	check_if_html_tag_starter_at_first(&packet, &length);
	strncpy(new_packet, packet, length);
	printf("new: \t%s\n", new_packet);

}

