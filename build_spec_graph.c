// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "main.h"
#include "node.h"

void printNames(Graph* graph) {
	printf("[");
	for (int i = 0; i < graph->size; i++) {
		if (i != 0) printf(", ");
		printf("\"%s\"", graph->names[i]);
	}
	printf("]");
}

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

	g->names = (char**)malloc(size * sizeof(char*));
	if (g->names == NULL) {
		printerr("Malloc failed");
		return NULL;
	}

	g->size = size;

	return g;
}

int graphIndexOf(Graph* graph, char* name) {
	for (int i = 0; i < graph->size; i++) {
		if (!strcmp(graph->names[i], name)) return i;
	}
	return -1;
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
 * Helper for detectCycles(Graph*)
 * Traverses graph and finds cycles
 * Returns truthy iff vertex at index i is part of a cycle
 */
int cycleAtVertex(Graph* g, int index) {
	int visited[g->size];
	int stack[g->size]; // array used as stack
	int stackIndex = -1; // to use array as stack
	int vertexIndex = index;

	// Clear the visited array
	for (int i = 0; i < g->size; i++) {
		visited[i] = 0;
	}

	stackIndex++;
	stack[stackIndex] = vertexIndex;

	// While stack is not empty, traverse
	while (stackIndex >= 0) {
		vertexIndex = stack[stackIndex];
		stackIndex--;
		if (visited[vertexIndex]) {
			if (vertexIndex == index) return 1; // back where we started
			continue; // revisiting old node, don't re-add elements
		}
		visited[vertexIndex] = 1; // mark current vertex as visited
		// Add the children of the current vertex
		for (int i = 0; i < g->size; i++) {
			if (g->matrix[vertexIndex][i]) {
				stackIndex++;
				stack[stackIndex] = i;
			}
		}
	}

	return 0;
}

/**
 * Returns lowest index of some vertex in a cycle if the graph has cycles
 * Returns -1 if graph has no cycles
 */
int detectCycles(Graph* g) {
	for (int i = 0; i < g->size; i++) {
		if (cycleAtVertex(g, i)) return i;
	}
	return -1;
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
	Node* nameNode;
	int namesIndex = 0;

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

	// Populate g->names array with elements of names linked list
	nameNode = names;
	while ((nameNode = nameNode->next) != NULL) {
		name = (char*)nameNode->element;
		graph->names[namesIndex++] = name;
	}

	// Detect cycles
	int cycleIndex = detectCycles(graph);
	if (cycleIndex != -1) {
		fprintf(stderr, "ERROR: Cycle detected at vertex %d\n", cycleIndex);
		return NULL;
	}

	return (void*)graph;
}

/**
 * Makes the given target if it is out of date
 * Does post-order traversal, making all dependencies first, if necessary
 * Assumes targets is a the header node, elements are TargetInfo*
 * Assumes all arguments are non-null, graph is acyclic
 * Returns 0 on success, nonzero on failure
 */
int makeTarget(Node* targets, Graph* graph, char* target) {
	int shouldMakeThis = 0; // truthy if out of date
	int targetIndex = graphIndexOf(graph, target);

	// Call makeTarget on all children
	for (int i = 0; i < graph->size; i++) {
		if (graph->matrix[targetIndex][i]) { // if we depend on the i-th target
			makeTarget(targets, graph, graph->names[i]);
		}
	}

	return 0;
}
