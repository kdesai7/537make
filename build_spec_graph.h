// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315
#ifndef BUILD_SPEC_GRAPH_H
#define BUILD_SPEC_GRAPH_H
#include "node.h"
typedef struct Graph {
	int** matrix; // 2D array of "bools"
	int size; // number of nodes in graph
} Graph;
void* buildSpecGraph(Node* targetsHeader);
#endif
