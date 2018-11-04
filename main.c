// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <string.h>

#include "build_spec_repr.h"
#include "build_spec_graph.h"
#include "node.h"
#include "text_parsing.h"

const int BUFFSIZE = 256;

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
	char* filename = "makefile";
	int fileSpecified = 0; // assume no file specified
	Node* targets;

	if (testTargetInfoCreation()) return 1;
	if (testNode()) return 3;

	if (argc > 1) {
		filename = argv[1];
		fileSpecified = 1;
	}

	targets = parse(filename);

	if (targets == NULL) {
		if (fileSpecified) {
			fprintf(stderr, "Failed to parse %s\n", filename);
			return -1;
		} else { // we were looking for "makefile"
			filename[0] = 'M'; // look for "Makefile"
			targets = parse(filename);
			if (targets == NULL) {
				fprintf(stderr, "Failed to parse both 'makefile' and 'Makefile'");
				return -1;
			}
		}
	}

	printTargets(targets); // for testing purposes TODO remove

	if (buildSpecGraph(targets) == NULL) {
		fprintf(stderr, "Failed to build spec graph for %s\n", filename);
		return -1;
	}

	return 0;
}
