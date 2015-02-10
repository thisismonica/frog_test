#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
	int value;
	struct Node *next;
	struct Node *prev;
}Node_t;

void remove_from_list(Node_t** head, Node_t* node){

	if(node->prev == node)
		//check if only one node
		*head = NULL;
	else{
		//delete node
		(node->prev)->next = node->next;
		(node->next)->prev = node->prev;
		//change head if node is head
		if(node==*head)
			*head = node->next;
	}
}

#include "klee.h"
#include "ansi_prefix.PPCEABI.bare.h"

int main() {
    
	Node_t** a0;
	a0 = (Node_t**)malloc( sizeof(Node_t*) );

	klee_make_symbolic(a0,sizeof(*a0),"a0");

	Node_t* a1;
	a1 = (Node_t*)malloc( sizeof(Node_t) );
	klee_make_symbolic(a1,sizeof(*a1),"a1");

	remove_from_list(a0,a1);

	return 0;


}
