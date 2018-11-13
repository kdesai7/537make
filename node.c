// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "node.h"

/**
 * Print one node
 * Assumes node is non-null, element of node is non-null string
 * Output is NOT terminated by newline
 */
void printStringNode(Node* node) {
	printf("(%s)", (char*)node->element);
}

/**
 * Prints all nodes starting past the header
 * Assumes header is non-null header node
 * Assumes elements of each node are strings
 * Connects elements with "-->" arrows
 * Output terminated by newline
 */
void printStringNodes(Node* header) {
	Node* curr = header;
	while ((curr = curr->next) != NULL) {
		printStringNode(curr);
		if (curr->next != NULL) printf("-->");
	}
	printf("\n");
}

/**
 * Creates a new node with the given element
 */
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
	if (head == NULL) return 1;
	Node* node = newNode(element);
	Node* curr = head;
	while (curr->next != NULL) {
		curr = curr->next;
	}
	// curr->next is now null
	curr->next = node;
	return 0;
}

/**
 * Returns the element at the given 0-based index relative from the head
 * The given node is not included, it is considered a header node
 * Returns NULL on failure
 */
Node* get(Node* header, int index) {
	Node* curr = header;
	if (curr == NULL) return NULL;
	for (int i = -1; i < index; i++) { // -1 because we start at header node
		curr = curr->next;
		if (curr == NULL) return NULL;
	}
	return curr;
}

/**
 * Returns the length of the given linked list
 * Assumes the given node is a header node
 * If given node is NULL, returns -1
 */
int length(Node* header) {
	if (header == NULL) return -1;
	int length = 0;
	Node* curr = header;
	while ((curr = curr->next) != NULL) length++;
	return length;
}

/**
 * Returns index of given element in list
 * Assumes given node is non-null header node
 * Assumes element is a well-formed, non-null string
 */
int indexOf(Node* header, char* element) {
	Node* curr = header;
	int index = 0;
	while ((curr = curr->next) != NULL) {
		char* actual = (char*)curr->element;
		if (!strcmp(actual, element)) {
			return index;
		}
		index++;
	}
	return -1;
}
