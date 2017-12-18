#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct node {
	item item;
	struct node *next;
	struct node *back;
};

typedef struct node node;

struct list {
	struct node *current;
};

// Main function
int listMain() {
	
	return 0;
	
}

// Create a new empty list.
list *newList() {
	list *l = malloc(sizeof(list));
	
	node *sentinelstart = malloc(sizeof(node));
	node *sentinelend = malloc(sizeof(node));
	
	sentinelstart->next = sentinelend;
	sentinelstart->back = NULL;
	
	sentinelend->next = NULL;
	sentinelend->back = sentinelstart;
	
	l->current = sentinelend;
	return l;
}

// Set the current position before the first item or after the last item, e.g.
// to begin a forward or backward traversal.
void start(list *l) {
	while(l->current->back->back != NULL) {
		l->current = l->current->back;
	}
}

void end(list *l) {
	while(l->current->next != NULL) {
		l->current = l->current->next;
	}
}

// Check whether the current position is at the start or end, e.g. to test
// whether a traversal has finished.
bool atStart(list *l) {
	return (l->current->back->back == NULL);
}
bool atEnd(list *l) {
	return (l->current->next == NULL);
}

// Move the current position one place forwards or backwards.  It is an error
// to call forward when at the end of the list, or backward when at the start.
void forward(list *l) {	
	if(l->current->next == NULL)
		fprintf(stderr, "Error: No node forward!");
	else
		l->current = l->current->next;	
}
void backward(list *l) {
	if(l->current->back->back == NULL)
		fprintf(stderr, "Error: No node backwards!");
	else
		l->current = l->current->back;
}

// Insert an item before or after the current position (i.e. at the current
// position, but with the current position ending up after or before the new
// item).
void insertBefore(list *l, item x) {
	node *right = l->current;	
	node *left = right->back;		
	node *middle = malloc(sizeof(node));
	
	middle->item = x;
	middle->back = left;
	middle->next = right;
	
	right->back = middle;
	left->next = middle;
	
}
void insertAfter(list *l, item x) {
	node *right = l->current;	
	node *left = right->back;		
	node *middle = malloc(sizeof(node));
	
	middle->item = x;
	middle->back = left;
	middle->next = right;
	
	right->back = middle;
	left->next = middle;
	
	l->current = middle;
	
}

// Get the item before or after the current position. It is an error to call
// getBefore when at the start, or getAfter when at the end.
item getBefore(list *l) {
	node *prevNode = l->current->back->back;
	assert(prevNode != NULL);
	return l->current->back->item;
}
item getAfter(list *l) {
	node *nextNode = l->current->next;
	assert(nextNode != NULL);
	return l->current->item;
}

// Set the item before or after the current position. It is an error to call
// setBefore when at the start, or setAfter when at the end.
void setBefore(list *l, item x) {
	node *prevNode = l->current->back->back;
	assert(prevNode != NULL);
	l->current->back->item = x;
}
void setAfter(list *l, item x) {
	node *nextNode = l->current->next;
	assert(nextNode != NULL);
	l->current->item = x;
}

// Delete the item before or after the current position. It is an error to call
// deleteBefore when at the start, or deleteAfter when at the end.
void deleteBefore(list *l) {
	node *prevNode = l->current->back->back;
	assert(prevNode != NULL);
	
	node *right = l->current;
	node *middle = right->back;
	node *left = middle->back;
	
	right->back = left;
	left->next = right;
	free(middle);
}
void deleteAfter(list *l) {
	node *nextNode = l->current->next;
	assert(nextNode != NULL);
	
	node *right = l->current->next;
	node *middle = right->back;
	node *left = middle->back;
	
	right->back = left;
	left->next = right;
	l->current = right;
	free(middle);
}

int size(list *l) {
	int count = 0;
	node *current = l->current;
	
	start(l);
	while(l->current->next != NULL) {
		l->current = l->current->next;
		count++;
	}
	
	l->current = current;
	
	return count;
}



















