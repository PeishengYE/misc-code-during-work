#include <stdio.h>
#include <stdlib.h>

struct node{
	struct node * prev;
	struct node * next;
	int data;
};

struct node * head;

void insert_data(int data)
{
	struct node *new_node;
	struct node *iter, *iter_prev;

	/* making new node */
	new_node = (struct node *) malloc(sizeof(struct node));
	new_node -> next = NULL;
	new_node -> prev = NULL;
	new_node -> data = data;


	if( head == NULL){
		head = new_node;

	}else if (data <= head -> data){

		new_node -> next = head;
		head -> prev = new_node; 
		head = new_node;

	}else{

		for(iter = head -> next, iter_prev = head;
				iter -> next != NULL;
				iter = iter -> next, 
				iter_prev = iter_prev -> next){

			if(data <=  iter->data){

				new_node -> next = iter;
				new_node -> prev = iter_prev;

				iter -> prev = new_node;
				iter_prev -> next = new_node;

				return;

			}


		}

		/* not found,  put the new node at the end of list */

		/*
		iter_prev -> next = new_node;
		new_node -> prev = iter_prev;
		*/
		iter -> next = new_node;
		new_node -> prev = iter;

	}

}

void display_list()
{
	struct node * iter, *iter_prev;
	for(iter = head -> next, iter_prev = head; 
			iter != NULL; 
			iter = iter->next, iter_prev = iter_prev->next){
		printf("%d ", iter->data);

	}
		printf("\n===================\n ");

		for(;iter_prev!=NULL;iter_prev = iter_prev->prev){
			printf("%d ", iter_prev->data);
		}

}

int main()
{
	int i, data;
	for (i = 0; i < 1000; i++)
	{
		data = random()%1000 + 1;
		insert_data(data);

	}
	display_list();

}
