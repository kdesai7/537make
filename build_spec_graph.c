// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "build_spec_graph.h"
#include "build_spec_repr.h"
#include "main.h"
#include "node.h"
#include "proc_creation_prog_exe.h"

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
 * Return a pointer to the node whose element name is the given name
 * Assumes header is a non-null header node, elements are of type TargetInfo*
 * Returns NULL if no node exists with the given name
 */
Node* find(Node* header, char* name) {
	Node* curr = header;
	TargetInfo* t;
	while ((curr = curr->next) != NULL) {
		t = (TargetInfo*)curr->element;
		if (!strcmp(t->name, name)) return curr;
	}
	return NULL;
}

/**
 * Returns the modified time of the given file as a time_t
 * Returns 0 on failure
 */
time_t lastModifiedTime(char* target) {
	struct stat sb;
	int error = stat(target, &sb);
	if (error) return 0;
	if ((sb.st_mode & S_IFMT) != S_IFREG) return 0;
	return sb.st_mtime;
}

/**
 * Makes the given target if it is out of date
 * Does post-order traversal, making all dependencies first, if necessary
 * Assumes targets is a the header node, elements are TargetInfo*
 * Assumes all arguments are non-null, graph is acyclic
 * Returns 1 if target was out of date, 0 if target was up to date, negative on failure
 */
int makeTarget(Node* targets, Graph* graph, char* target) {
	int shouldMakeThis = 0; // truthy if out of date
	int targetIndex = graphIndexOf(graph, target);
	struct stat fileStat; // info on file corresponding to this target
	Node* targetNode; // element of type TargetInfo*
	int error;
	time_t thisMtime; // modified time of target
	time_t depMtime; // modified time of dependency

	// Check if we have a valid target
	if (targetIndex < 0) {
		fprintf(stderr, "ERROR: No rule to make target %s, stop.\n", target);
		return -1;
	}

	// Call makeTarget on all children
	for (int i = 0; i < graph->size; i++) {
		if (graph->matrix[targetIndex][i]) { // if we depend on the i-th target
			shouldMakeThis |= makeTarget(targets, graph, graph->names[i]);
			if (shouldMakeThis < 0) { // assumes 2's-complement implementation
				return shouldMakeThis; // don't continue processing if make failed
			}
		}
	}

	// If no children were out of date, we look for a file named <target>
	// If we don't find that file, we need to make this target
	// If we find that file, we check deps of target to see if we are up to date
	if (!shouldMakeThis) { // no children were out of date
		// Look for a file
		error = stat(target, &fileStat);
		if (error) { // file not opened
			if (errno == ENOENT) { // file not found
				shouldMakeThis = 1; // we have to make it
			} else { // other error
				fprintf(stderr, "ERROR in %s: %s\n", target, strerror(errno));
				return -1; // unacceptable, quit now
			}
		} else { // file successfully opened
			if ((fileStat.st_mode & S_IFMT) != S_IFREG) { // not regular file
				fprintf(stderr, "ERROR: Opened irregular file %s\n", target);
				return -1;
			}
			// Check last modified time against last modified times of each child
			thisMtime = lastModifiedTime(target);
			if (thisMtime == 0) {
				fprintf(stderr, "ERROR: could not get mtime of %s\n", target);
				return -1;
			}
			for (int i = 0; i < graph->size; i++) {
				if (graph->matrix[targetIndex][i]) {
					depMtime = lastModifiedTime(graph->names[i]);
					if (difftime(depMtime, thisMtime) > 0) { // dep is newer
						shouldMakeThis = 1;
						break;
					}
				}
			}
		}
	}

	// Time to execute commands!
	if (shouldMakeThis) {
		targetNode = find(targets, target);
		if (targetNode == NULL) {
			fprintf(stderr, "No rule to make target \"%s\"\n", target);
			return -1;
		}
		printf("Commands for %s\n", target);
		printCmds(targetNode);
		TargetInfo* targetInfo = (TargetInfo*)targetNode->element;
		executeCommands(targetInfo->cmds);
	}

	return shouldMakeThis;
}
