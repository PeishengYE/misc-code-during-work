/*
Given a binary tree and a target value, find a path that the sum of all the values along the path equals to the target value. 
*/
#include "stdio.h"

#define MAX_PATH 5 //maximum length of the array which is used for keeping the path.

typedef struct node {
	int val;
	struct node * parent;
	struct node * left;
	struct node * right;
} nod;

void findpath(nod * root, int value, nod ** stack)
{
	int sum = 0;
	int stack_top = -1; //position of stack top.
	nod * nodit = root; // iterator for loop;
		
	while (1) {
		for ( ; nodit != NULL; )
		{
			sum = sum + nodit->val;
			if (sum == value && nodit->left == NULL && nodit->right == NULL) //found the correct path.
			{
				stack[stack_top + 1] = nodit;
				stack[stack_top + 2] = NULL; //path is ended with NULL.
				return;
			}
			stack_top++;
			stack[stack_top] = nodit;
			
			if (nodit->left != NULL) // left child is of high priority, if there's only right child, try it. if no child any more, set nodit to NULL and go to next loop.
				nodit = nodit->left;
			else
				nodit = nodit->right;
		}

		while (1)
		{
			if (stack_top < 0)
			{
				return; // search finished, path not found;
			}
			nod * stack_parrent = stack[stack_top - 1];
			nodit = stack[stack_top];
			if (nodit != stack_parrent->right) //if this is not right child, then try right child.
			{
				nodit = stack_parrent->right;
				sum = sum - stack[stack_top]->val;
				stack[stack_top] == NULL;
				stack_top--;
				break;
			}
			sum = sum - nodit->val; //all children have been tried, go back to upper level.
			nodit = stack_parrent;
			stack[stack_top] = NULL;
			stack_top --;
		}
	}
}

void printpath(nod ** path) //print the path.
{
	int i = 0;
	if (path == NULL || path[0] == NULL)
	{
		printf("Can not find the path!\n\n\n");
		return;
	}
	
	printf("The path is : \n");
	while (path[i] != NULL)
	{
		if (i != 0)
		{
			printf("-->");
		}
		printf("%d",path[i]->val);
		i++;
	}
	printf("\n\n\n");
}

void clean_stack(nod ** path)
{
	int i = 0;
	for ( ; i < MAX_PATH; i++)
	{
		path[i] = NULL;
	}
}

main()
{
	nod * stack[MAX_PATH] = {NULL, NULL, NULL, NULL, NULL}; //stack for saving temp path. should be big enough to contain the maximum path of the binary tree and plus one, path is ended with NULL.
	//now construct a binary tree.
	nod n1,n2,n3,n4,n5,n6,n7,n8,n9;
	n1.val = 5;
	n1.parent = NULL;
	n1.left = &n2;
	n1.right = &n3;
	n2.val = 4;
	n2.parent = &n1;
	n2.left = &n4;
	n2.right = NULL;
	n3.val = 8;
	n3.parent = &n1;
	n3.left = &n5;
	n3.right = &n6;
	n4.val = 11;
	n4.parent = &n2;
	n4.left = &n7;
	n4.right = &n8;
	n5.val = 13;
	n5.parent = &n3;
	n5.left = NULL;
	n5.right = NULL;
	n6.val = 4;
	n6.parent = &n3;
	n6.left = NULL;
	n6.right = &n9;
	n7.val = 7;
	n7.parent = &n4;
	n7.left = NULL;
	n7.right = NULL;
	n8.val = 2;
	n8.parent = &n4;
	n8.left = NULL;
	n8.right = NULL;
	n9.val = 1;
	n9.parent = &n6;
	n9.left = NULL;
	n9.right = NULL;
	
	findpath(&n1, 22, stack);
	printpath(stack);
	
	clean_stack(stack);
	findpath(&n1, 26, stack);
	printpath(stack);
	
	clean_stack(stack);
	findpath(&n1, 27, stack);
	printpath(stack);
	
	clean_stack(stack);
	findpath(&n1, 18, stack);
	printpath(stack);
	
	clean_stack(stack);
	findpath(&n1, 88, stack);
	printpath(stack);
	
	clean_stack(stack);
	findpath(&n1, 0, stack);
	printpath(stack);
	
}