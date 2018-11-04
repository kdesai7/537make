// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdlib.h>
#include <stdio.h>

#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "main.h"
#include "node.h"

void printGraph(Graph* graph) {
	for (int r = 0; r < graph->size; r++) {
		for (int c = 0; c < graph->size; c++) {
			if (c != 0) printf(", ");
			printf("%d", graph->matrix[r][c]);
		}
		printf("\n");
	}
}

Graph* newGraph(int size) {
	Graph* g = (Graph*)malloc(sizeof(Graph));
	if (g == NULL) {
		printerr("Malloc failed");
		return NULL;
	}

	g->matrix = (int**)malloc(size * sizeof(int*));
	if (g->matrix == NULL) {
		printerr("Malloc failed");
		return NULL;
	}

	for (int r = 0; r < size; r++) {
		g->matrix[r] = (int*)malloc(size * sizeof(int));
		if (g->matrix[r] == NULL) {
			printerr("Malloc failed");
			return NULL;
		}
		for (int c = 0; c < size; c++) {
			g->matrix[r][c] = 0; // no dependencies by default
		}
	}

	g->size = size;

	return g;
}

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
	Node* targetNode = targetsHeader; // used to iterate through targets
	TargetInfo* target; // actual element
	Node* dep; // iterates through dependencies
	Graph* graph; // the actual graph
	int row; // row within graph matrix
	int col; // col within graph matrix

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

	// Build and populate the graph
	graph = newGraph(length(names));
	targetNode = targetsHeader; // reset this node
	while ((targetNode = targetNode->next) != NULL) {
		target = (TargetInfo*)targetNode->element;
		name = target->name;
		row = indexOf(names, name);
		dep = target->deps;
		while ((dep = dep->next) != NULL) {
			name = (char*)dep->element;
			col = indexOf(names, name);
			graph->matrix[row][col] = 1;
		}
	}

	printGraph(graph);

	return NULL;
}
