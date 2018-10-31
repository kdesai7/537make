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
