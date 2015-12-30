#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SCI_FREE free
typedef struct html_image_info_{

	char *mem;
	char * image_id;
	/* TODO 
	 * be carefull to deallocate these two mem
	 * when you destroy this object
	 * deep copying and deep destroy
	 *
	 * */
	int size;
	int pos_x;
	int pos_y;
	int image_type;
	int image_style; 
}html_image_info;

typedef struct image_node_{
   html_image_info *item;
   struct image_node_ *next;

}image_node;

void print_node(html_image_info *each_object);

static image_node *images_head = NULL;

static void free_image_node(image_node * current)
{
    print_node(current->item);
	SCI_FREE(current->item->mem);
	SCI_FREE(current->item->image_id);
	SCI_FREE(current->item);
	SCI_FREE(current);
}

static void ubi_html_image_container_init(void )
{
	image_node * current = images_head;
	image_node * next ;

	if(current != NULL ){

		next = current->next;
		/* free current node */
		free_image_node(current);

		/* free the image objects */
		while(next != NULL){
		   //current = current->next;
		   current = next;
		   next = current->next;
		   free_image_node(current);
		}

	}

	images_head = NULL;

}

static void ubi_html_image_container_destroy(void )
{
    ubi_html_image_container_init( );
}


 void adding_image_in_container(image_node *image)
{
    image_node * current = images_head;

	if(current != NULL ){

		/* find a node with next pointer == NULL 
		 * */
		while(current->next != NULL){
		  current = current->next;
		}
		current->next = image;

	}else{

		images_head = image;
	}

}

void print_node(html_image_info *each_object)
{
	printf("-------------------------\n");
	printf("image_size = << %d >>\n", each_object->size);
	printf("mem = << %s >>\n", each_object->mem);
	printf("image_id = << %s >>\n", each_object->image_id);
	printf("-------------------------\n");
	printf("\n");
	printf("\n");

}


void use_images()
{

    image_node * current = images_head;

	if(current != NULL ){

		/*
		 * */

       //print_node(current->item);


		/* find a node with next pointer == NULL 
		 * */
		while(current->next != NULL){
          print_node(current->item);

		  current = current->next;
		}

          print_node(current->item);

	}else{
     printf("\nimages is empty\n");
	}

}

#define MAX_OBJECT_NUM 10
#define TEST_STRING_1 "hello world!!!"
#define TEST_STRING_2 "image id 123 !!!"
int main(int argc, char *argv[])
{
	int i=0;
    html_image_info *image_object;
    image_node *image_node_m;

	/* loop to add image objects */
	for(i = 0; i < MAX_OBJECT_NUM; i++){

		image_object = malloc(sizeof (html_image_info));
		memset(image_object, 0, sizeof (html_image_info));

		image_object->mem = malloc(strlen(TEST_STRING_1) + 2); 
		strcpy( image_object->mem ,  TEST_STRING_1 );

		image_object->image_id = malloc(strlen(TEST_STRING_2) + 2); 
		strcpy( image_object->image_id ,  TEST_STRING_2 );

		image_object->size = i;

        image_node_m = malloc(sizeof(image_node));
		memset(image_node_m, 0, sizeof(image_node));
		image_node_m->item = image_object;

        adding_image_in_container(image_node_m);
	}

    use_images();

	printf("\ntrying to free images\n");
    ubi_html_image_container_init();
    use_images();
}

