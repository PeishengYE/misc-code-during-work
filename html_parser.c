
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

	        //printf("hit...\n");
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

static char * get_sub_item_name(char *mem, char *mem_end)
{

	char *item_name_starter = NULL;
	char *item_name_ender = NULL;
	char *ret = NULL;
	int size;
	char *current = mem;

	while((*current !=  SUB_HTML_TAG_STARTER)
	&&(current < mem_end)){ 
		current++;}

   if(current < mem_end)
			item_name_starter = current;

   while((*current !=  SUB_HTML_TAG_ENDER )
		&&(current < mem_end)){ current++;}

  if(current < mem_end)
        item_name_ender = current;

  if((item_name_starter != NULL)
		  &&(item_name_ender != NULL)){
	  size = item_name_ender - item_name_starter;

	  ret = malloc(size + 2);
	  memset(ret, 0, size +2);
	  memcpy(ret, item_name_starter, size);

	  printf("%s()>> subname:%s \n", __FUNCTION__, ret );

  }
  return ret;

}

static void get_sub_item_content(char *mem, char *mem_end, char **position, int *size)
{
	char delimiter;
	char *current = mem;
	char *content_starter = NULL;
	char *content_ender = NULL;
	int ret_size = 0;

	char *tmp_mem;

	/* find the delimiter */
	while((*current != '=')
			&&(current < mem_end)) current++;

	/* go to the next char after the symbol '=' */
	current ++;

   if(current < mem_end){

        delimiter = *current ;
        content_starter = current + 1;

	  //printf("%s()>> delimiter:(%c) \n", __FUNCTION__, delimiter );
   }

   current  =  content_starter ;
   while((*current != delimiter)
			&&(current < mem_end)) current++;

   if(current < mem_end){

        content_ender = current ;
	     //printf("%s()>> the next delimiter:(%c) \n", __FUNCTION__, *current );
   }

   if(content_starter != NULL){
	   *position = content_starter;
       ret_size =  content_ender - content_starter ;   


     *size = ret_size;
/**/
   tmp_mem = malloc(ret_size + 10);
	memset (tmp_mem, 0, ret_size + 10);
	memcpy(tmp_mem, content_starter, ret_size);
	printf("%s()>> content:: %s \n", __FUNCTION__, tmp_mem );
	free(tmp_mem);
   /**/
   }

   



}


static void html_sub_items_constructor(char *html_item, int html_item_size)
{
    char *html_end = html_item + html_item_size;
	char *current = html_item;
    html_items *cur_item = html_sub_items_container;

	char *content_position = NULL;
	int content_size;

	int counter = 0;



   do{
		cur_item -> next = (html_items *)malloc(sizeof (html_items));
		cur_item = cur_item->next; 

		memset(cur_item, 0, sizeof(html_items));


	    
		/* assign */
		cur_item->name =  get_sub_item_name(current, html_end);

        get_sub_item_content(current, html_end, &content_position, &content_size);

	    //content_position = NULL;
		if(content_position == NULL) break;
		counter ++;
		//if(counter >= 3) break;

		cur_item->ptr = content_position;
		cur_item->size = content_size;

		current = content_position + content_size;


		/* be careful there is a two byte left, when current reach the end
		 * "">"
		 * */
	}while( current < html_end -3);

}

static void html_sub_items_destructor( )
{
   html_items *cur_item = html_sub_items_container->next;
   char *mem;
   html_items *prev_item;
   while(cur_item != NULL){
	   printf("%s()>> sub name = %s\n", __FUNCTION__,cur_item->name );

	   mem = malloc(cur_item->size + 2);
	   memset(mem, 0, cur_item->size + 2);

	   memcpy(mem, cur_item->ptr, cur_item->size);
	   printf("%s()>> sub content: \n%s\n",__FUNCTION__, mem );


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


static void html_items_destructor( )
{
   html_items *cur_item = html_items_container->next;
   char *mem;
   html_items *prev_item;
   while(cur_item != NULL){
	   printf("name = %s\n", cur_item->name );

	   mem = malloc(cur_item->size + 2);
	   memset(mem, 0, cur_item->size + 2);

	  

	   memcpy(mem, cur_item->ptr, cur_item->size);
	   printf("content: \n%s\n", mem );

      if(strcmp(cur_item->name, "img") == 0){
	      printf("img going to be angalsis\n");
          html_sub_items_constructor(mem, cur_item->size);
          html_sub_items_destructor();
	   }

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

#define HTML_END_FLAG_5 "<dwdata>"
static void  remove_and_save_unfinished_packet(char * mem,  int* size)
{
	char *last_mem;
	int max_size = *size; 
	int current_place = max_size;
	int tag_length;
	char *current;

	char mem_tmp[20];
	tag_length = strlen(HTML_END_FLAG_5);
	last_mem = mem + max_size;
	/* loop from the end of big memory */

	while(current_place >= tag_length){

		current = mem + current_place;
		current -= tag_length;

		memset(mem_tmp, 0, 20);
		memcpy(mem_tmp, current, tag_length);

		if(strcmp(mem_tmp, HTML_END_FLAG_5) == 0){

			printf("found html tag\n");
			break;
		}


		current_place--;
	}

	if(current_place > tag_length){
	   *size = current_place;
	   printf("place on %d\n", current_place);
	}else{
	   *size = 0;
	   printf("no html tag found, the entire packet should be put back to the container\n");
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
	int original_size;
	char buf[BUF_SIZE];

	char last_str[SIZE_TO_SHOW + 10];
	char *big_mem;
	char *remain_mem;
	char *mem_tmp;
	int remain_size;



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

   original_size = file_size;
   remove_and_save_unfinished_packet(big_mem,   &file_size);
   remain_size = original_size - file_size;

   mem_tmp = malloc(remain_size);
   memset(mem_tmp, 0, remain_size);

   remain_mem = big_mem + file_size ;
   memcpy(mem_tmp, remain_mem,remain_size);

   printf("remained bytes:: \n");
   printf("%s\n", mem_tmp);


#if 0
	printf("html constructor  \n");
   html_items_constructor(big_mem,  file_size);

	printf("html destructor  \n");
   html_items_destructor();


	mem -= SIZE_TO_SHOW ;
	memset(last_str, 0, sizeof(last_str));

	memcpy(last_str, mem, SIZE_TO_SHOW );
	

	printf("\nstring: %s\n", last_str);
	printf("\nmem length: %d\n", strlen(big_mem));
#endif
	
}
