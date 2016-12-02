/*
Deep copy a list.
*/
#include "stdio.h"
#include "stdlib.h"

typedef struct node{ 
	int value;
	struct node * next;
	struct node * saved;
} nod;

void printnode(nod * head);

void copy_notes(nod * head) //generate the new list, merge two list into one, every new note is right after the corresponding original one.
{
	nod * orgit = head; //iterator for original list;
	while (orgit != NULL)
	{
		nod * cloneit = (nod *) malloc(sizeof(nod));
		if (cloneit == NULL)
		{
			printf("malloc failed!");
			return ;
		}
		cloneit->value = orgit->value;
		cloneit->next = orgit->next;
		orgit->next = cloneit;
		cloneit->saved = NULL;
		orgit = cloneit->next;
	}
}

void copy_saved(nod * head) //deep copy saved pointer.
{
	nod * orgit = head; //iterator for original list;
	nod * cloneit = NULL; //iterator for copied list;
	while (orgit != NULL)
	{
		cloneit = orgit->next;
		if (orgit->saved != NULL)
		{
			cloneit->saved=orgit->saved->next;
		}
		orgit = cloneit->next;
	}
}

nod * separate_notes(nod * head) //separate the merged list into two lists, return the cloned list head;
{
	nod * orgit = head;
	nod * clonehead = NULL;
	nod * cloneit = NULL;
	
	if (orgit != NULL)
	{
		clonehead = orgit->next;
		cloneit = orgit->next;
		
		orgit->next = cloneit->next;
		orgit = orgit->next;
	}
		
	while(orgit != NULL)
	{
		cloneit->next = orgit->next;
		cloneit = cloneit->next;
		orgit->next = cloneit->next;		
		orgit = orgit->next;
	}
	
	return clonehead;
}

nod * copy_list(nod * head) //deep copy a linked list.
{
	if (head == NULL)
		return NULL;
	copy_notes(head);
	copy_saved(head);
	return separate_notes(head);
}

void printnode(nod * head)
{
	nod * tmp = head;
	while (tmp != NULL)
	{
		printf("this addr is %d, this val is %d.\n",tmp, tmp->value);
		if (tmp->next != NULL)
		{
			printf("next addr is %d, next val is %d.\n",tmp->next, tmp->next->value);
		}
		else
		{
			printf("next addr is NULL\n");
		}
		if (tmp->saved != NULL)
		{
			printf("saved addr is %d, saved val is %d.\n",tmp->saved, tmp->saved->value);
		}
		else
		{
			printf("saved addr is NULL.\n");
		}
		printf("--------------\n");
		tmp = tmp->next;
	}
}

void deletenode(nod * head) //free allocated memory;
{
	nod * tmp = head;
	nod * tmpnext;
	while (tmp != NULL)
	{
		tmpnext = tmp->next;
		free(tmp);
		tmp = tmpnext;
	}
}

main ()
{
	nod n1,n2,n3,n4,n5,n6,n7,n8,n9,n10;
	n1.value = 1;
	n1.next = &n2;
	n1.saved = NULL;
	n2.value = 2;
	n2.next = &n3;
	n2.saved = &n7;
	n3.value = 3;
	n3.next = &n4;
	n3.saved = &n5;
	n4.value = 4;
	n4.next = &n5;
	n4.saved = &n3;
	n5.value = 5;
	n5.next = &n6;
	n5.saved = NULL;
	n6.value = 6;
	n6.next = &n7;
	n6.saved = &n7;
	n7.value = 7;
	n7.next = &n8;
	n7.saved = &n9;
	n8.value = 8;
	n8.next = &n9;
	n8.saved = &n1;
	n9.value = 9;
	n9.next = &n10;
	n9.saved = &n6;
	n10.value = 10;
	n10.next = NULL;
	n10.saved = &n1;
	printnode(&n1);
	nod * clonehead = copy_list(&n1);
	printf("\n\n****************\nBelow is cloned list.\n");
	printnode(clonehead);
	
	deletenode(clonehead);
}