// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdlib.h>
#include <stdio.h>

#include "node.h"

Node* newNode(void* element) {
	Node* node = (Node*) malloc(sizeof(Node));
	if (node == NULL) {
		fprintf(stderr, "Malloc failed\n");
		exit(EXIT_FAILURE);
	}
	node->element = element;
	node->next = NULL;
	return node;
}

/**
 * Appends element to end of given linked list
 * Zero on success, nonzero on failure
 */
int append(Node* head, void* element) {
	Node* node = newNode(element);
	Node* curr = head;
	if (curr == NULL) return 1;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	// curr->next is now null
	curr->next = node;
	return 0;
}

/**
 * Returns the element at the given 0-based index relative from the head
 * Returns NULL on failure
 */
Node* get(Node* head, int index) {
	Node* curr = head;
	for (int i = 0; i < index; i++) {
		if (curr == NULL) return NULL;
		curr = curr->next;
	}
	return curr;
}
