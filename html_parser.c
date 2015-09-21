
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct html_item_{
	char *name;
	char *ptr;
	int size;
	struct html_item_ *next;
}html_items;

#define DW_HTML_TAG  "<dwdata>"
#define SUB_HTML_TAG_STARTER  ' '
#define SUB_HTML_TAG_ENDER  '='

#define TAG_BEGIN '<'
#define TAG_ENDER ' '
static char *get_html_tag(char * current) 
{
	int size;
	char *mem_starter, *ret;
	char *mem_ender;
	char *cur = current;

	for(;*cur != TAG_BEGIN; cur++){ }
	mem_starter = cur ;

	for(cur = mem_starter; *cur != TAG_ENDER; cur++){ }
	mem_ender = cur;

	size = mem_ender - mem_starter;
	
	ret = (char *)malloc(size +2);
	memset(ret, 0, size+2);

	memcpy(ret, mem_starter+1 , size -1);
	return ret;
}


html_items html_item ;
html_items *html_items_container = &html_item;
/* seperate html in different typs */
static void html_items_constructor(char *html, int html_size)
{
	char *html_end = html + html_size;
	char *current = html;
    html_items *cur_item = html_items_container;
	int name_size, item_size;
	char str_tmp[20];


	do{
	        //printf("hit... 1 " );
	/* memory allocation for the html item */
		cur_item -> next = (html_items *)malloc(sizeof (html_items));
		cur_item = cur_item->next; 

		memset(cur_item, 0, sizeof(html_items));

	/* assign the start pointer of html, html tag to the first item ptr */
		cur_item->ptr = current;
		cur_item->name = get_html_tag(current); 

	/* find the first datawind tag, assign the size of first html item */
	    while(current < html_end){

		memset(str_tmp, 0, sizeof(str_tmp));
		memcpy(str_tmp,  current, strlen(DW_HTML_TAG));


	    if(strcmp(str_tmp, DW_HTML_TAG) == 0){

	        printf("hit...\n");
		   item_size = current - cur_item->ptr;
           cur_item->size = item_size; 

		   current += strlen(DW_HTML_TAG);
		   break;
	       }

		current++;
	   }

	        //printf("hit...2 " );

	/* if current loop ptr is NOT the end of html stream, continue */

	}while( current < html_end);

}

html_items html_sub_item ;
html_items *html_sub_items_container = &html_sub_item;

static void get_sub_item_name(char *mem, char *mem_end)
{

	char *item_name_starter, *item_name_ender;

	while(current !=  SUB_HTML_TAG_STARTER)
	&&(current < mem_end){ 
		current++;}

        if(current < mem_end)
			item_name_starter = current;

	    while(current !=  SUB_HTML_TAG_ENDER )
		&&(current < mem_end){ current++;}

        if(current < mem_end)
        item_name_ender = current;



}


static void html_sub_items_constructor(char *html_item, int html_item_size)
{
   char *html_end = html + html_size;
	char *current = html;
    html_items *cur_item = html_sub_items_container;

	char *item_name_starter, *item_name_ender;
	char *item_content_starter, *item_content_ender;






   do{
	/* memory allocation for the html item */
		cur_item -> next = (html_items *)malloc(sizeof (html_items));
		cur_item = cur_item->next; 

		memset(cur_item, 0, sizeof(html_items));


	    
		/* assign */
		cur_item->name = get_sub_html_tag(current); 

		cur_item->ptr = current;



	        printf("hit...\n");
		   item_size = current - cur_item->ptr;
           cur_item->size = item_size; 

		   current += strlen(DW_HTML_TAG);


	        //printf("hit...2 " );

	/* if current loop ptr is NOT the end of html stream, continue */

	}while( current < html_end);

}

static void html_sub_items_destructor()
{

}

static void html_items_destructor()
{
   html_items *cur_item = html_items_container->next;
   char *mem;
   html_items *prev_item;
   while(cur_item != NULL){
	   printf("name = %s\n", cur_item->name );

	   mem = malloc(cur_item->size + 2);
	   memset(mem, 0, cur_item->size + 2);

	   memcpy(mem, cur_item->ptr, cur_item->size);
	   printf("string: \n%s\n", mem );

	   cur_item = cur_item->next;
	   free(mem);
   }


   cur_item = html_items_container->next;
   while(cur_item != NULL){
	   //free(cur_item->ptr);
	   free(cur_item->name);
       prev_item = cur_item;
	   cur_item = cur_item->next;
	   free(prev_item);
   }

}





#define BUF_SIZE 100
#define SIZE_TO_SHOW 150
int main(int argc, char * argv[])
{
	FILE *fp;
	struct stat file_stat;
	char *mem;
	int read_size = BUF_SIZE;
	int  nread, file_size;
	char buf[BUF_SIZE];

	char last_str[SIZE_TO_SHOW + 10];
	char *big_mem;



	/* open files */

	if((fp = fopen(argv[1],"r+")) == NULL)
		perror("fopen error");

		/* get file size */
	if(stat(argv[1],&file_stat) != 0)
		perror("stat error");

	//printf("file_size   = %d\n",file_stat.st_size);
	file_size = file_stat.st_size;

	mem = (char *)malloc(file_size + 20);
	big_mem = mem;

	memset(mem, 0, file_size + 20);

	//nread = fread(buf,read_size,1,fp);
	nread = fread(buf,1,read_size,fp);

	while(nread > 0){

	   //printf("read_size   = %d\n",nread);
		memcpy(mem, buf, nread);
		mem += nread;
	    nread = fread(buf,1,read_size,fp);

	}


   html_items_constructor(big_mem,  file_size);

   html_items_destructor();


#if 0
	mem -= SIZE_TO_SHOW ;
	memset(last_str, 0, sizeof(last_str));

	memcpy(last_str, mem, SIZE_TO_SHOW );
	

	printf("\nstring: %s\n", last_str);
	printf("\nmem length: %d\n", strlen(big_mem));
#endif
	
}
