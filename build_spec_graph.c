// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdlib.h>

#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "node.h"

/**
 * Adds the given element to the end of the list if it isn't already in the list
 * Assumes list is a non-null header node, element is non-null string
 * Returns 0 on success, nonzero on failure
 */
int addIfNotIn(Node* list, char* element) {
	if (indexOf(list, element) == -1) {
		return append(list, (void*)element);
	}
	return 0;
}

/**
 * Builds a graph based on the given linked list of targets
 * Assumes the given node is a non-null header and each element is a TargetInfo*
 * Returns NULL on failure
 * Returns a reference to the graph on success
 */
void* buildSpecGraph(Node* targetsHeader) {
	Node* names = newNode(NULL); // currently stored names
	char* name; // current name to add
	Node* targetNode = targetsHeader;
	TargetInfo* target;
	Node* dep;

	if (names == NULL) return NULL;

	// Add all the names of targets and their dependencies
	while((targetNode = targetNode->next) != NULL) {
		target = (TargetInfo*)targetNode->element;
		name = target->name;
		// Add target's name
		addIfNotIn(names, name);
		// Add target's dependencies
		dep = target->deps;
		while ((dep = dep->next) != NULL) {
			name = (char*)dep->element;
			addIfNotIn(names, name);
		}
	}

	printStringNodes(names); // for debugging purposes TODO remove

	return NULL;
}
