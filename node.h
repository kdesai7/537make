// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315
#ifndef NODE_H
#define NODE_H
typedef struct Node {
	void* element;
	struct Node* next;
} Node;
Node* newNode(void* element);
int append(Node* head, void* element);
Node* get(Node* head, int index);
int length(Node* header);
#endif
