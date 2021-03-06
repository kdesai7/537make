// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <string.h>

#include "proc_creation_prog_exe.h"
#include "build_spec_repr.h"
#include "build_spec_graph.h"
#include "node.h"
#include "text_parsing.h"

const int BUFFSIZE = 1024;

/**
 * Prints the error message to stderr
 * e.g. printerr("Oh no!") --> prints "ERROR: Oh no!" terminated by newline
 * to stderr
 */
void printerr(const char* msg) {
	fprintf(stderr, "ERROR: %s\n", msg);
}

/**
 * Tests the get function
 * Returns 0 on success, nonzero on failure
 */
int testNodeGet() {
	int a = 1;
	int b = 2;
	int c = 3;
	Node* head = newNode(NULL); // header node
	append(head, &a); // index 0
	append(head, &b); // 1
	append(head, &a); // 2
	append(head, &c); // 3

	Node* end = get(head, 3);

	int actual = *((int*)end->element);
	int expected = c;
	if (actual != expected) {
		fprintf(stderr, "node.get(...) not as expected, got %d instead of %d\n", actual, expected);
		return 1;
	}
	return 0;
}

/**
 * Tests the length function
 * Returns 0 on success, nonzero on failure
 */
int testNodeLength() {
	int a = 1;
	int b = 2;
	int c = 3;
	Node* head = newNode(NULL); // header node
	append(head, &a); // index 0
	append(head, &b); // 1
	append(head, &a); // 2
	append(head, &c); // 3

	int actual = length(head);

	int expected = 4;
	if (actual != expected) {
		fprintf(stderr, "node.length(...) not as expected, got %d instead of %d\n", actual, expected);
		return 1;
	}
	return 0;
}

/**
 * Tests indexOf function
 * Returns 0 on success, nonzero on failure
 */
int testNodeIndexOf() {
	char* a = "apple";
	char* b = "banana";
	char* c = "cantaloupe";
	Node* head = newNode(NULL); // header node
	append(head, a); // index 0
	append(head, b); // 1
	append(head, a); // 2
	append(head, c); // 3

	int actual = indexOf(head, b);

	int expected = 1;
	if (actual != expected) {
		fprintf(stderr, "node.indexOf(...) not as expected, got %d instead of %d\n", actual, expected);
		return 1;
	}
	return 0;
}

/**
 * Tests functions of the node module
 * Returns 0 on success, returns nonzero on failure
 */
int testNode() {
	if (testNodeGet()) return 1;
	if (testNodeLength()) return 2;
	if (testNodeIndexOf()) return 3;
	return 0;
}

int testTargetInfoCreation() {
	TargetInfo* t = newTargetInfo();
	if (t == NULL) {
		fprintf(stderr, "newTargetInfo failed\n");
		return 1;
	}
	return 0;
}

int main(int argc, char** argv) {
	char filename[BUFFSIZE];
	char* target = NULL;
	int fileSpecified = 0;
	Node* targets;
	int fileNotFound[1];
	fileNotFound[0] = 0;

	if (argc > 2) {
		fprintf(stderr, "Usage: %s <target> [target is optional]\n", argv[0]);
		return -1;
	} else if (argc == 2) {
		target = argv[1];
	}

	strcpy(filename, "makefile");
	targets = parse(filename, fileNotFound);

	if (targets == NULL) {
		if (fileSpecified) { // TODO always false for now, fix later
			// TODO print fileNotFound error if necessary
			fprintf(stderr, "Failed to parse %s\n", filename);
			return -1;
		} else { // we were looking for "makefile"
			if (fileNotFound[0]) {
				strcpy(filename, "Makefile"); // look for "Makefile"
				fileNotFound[0] = 0;
				targets = parse(filename, fileNotFound);
			}
		}
	}

	if (targets == NULL) {
		if (fileNotFound[0]) {
			fprintf(stderr, "File not found\n");
		}
		fprintf(stderr, "Failed to parse %s\n", filename);
		return -1;
	}

	if (targets->next == NULL) {
		printerr("No targets in given file");
		return -1;
	}

	Graph* graph = buildSpecGraph(targets);

	if (graph  == NULL) {
		fprintf(stderr, "Failed to build spec graph for %s\n", filename);
		return -1;
	}

	if (target == NULL) { // no target specified, get first target
		target = (char*)((TargetInfo*)targets->next->element)->name;
	}

	if (makeTarget(targets, graph, target) < 0) {
		printerr("Make target failed");
		return 1;
	}

	return 0;
}
