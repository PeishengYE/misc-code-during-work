#include <stdio.h>

// The list element type and head.

struct node { 
    int data;
    struct node *link;
};
static struct node *first = NULL;

// A reverse function which uses only two extra pointers.

void reverse() {
    // curNode traverses the list, first is reset to empty list.
    struct node *curNode = first, *nxtNode;
    first = NULL;

    // Until no more in list, insert current before first and advance.
    while (curNode != NULL) {
        // Need to save next node since we're changing the current.
        nxtNode = curNode->link;

        // Insert at start of new list.
        curNode->link = first;
        first = curNode;

        // Advance to next.
        curNode = nxtNode;
    }
}

// Code to dump the current list.

static void dumpNodes() {
    struct node *curNode = first;
    printf ("==========\n");
    while (curNode != NULL) {
        printf ("%d\n", curNode->data);
        curNode = curNode->link;
    }
}

// Test harness main program.

int main (void) {
    int i;
    struct node *newnode;

    // Create list (using actually the same insert-before-first
    // that is used in reverse function.

    for (i = 0; i < 5; i++) {
        newnode = malloc (sizeof (struct node));
        newnode->data = i;
        newnode->link = first;
        first = newnode;
    }

    // Dump list, reverse it, then dump again.

    dumpNodes();
    reverse();
    dumpNodes();
    printf ("==========\n");

    return 0;
}
