// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>

#include "build_spec_repr.h"
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

int testNodeCreation() {
	int a = 1;
	int b = 2;
	int c = 3;
	Node* head = newNode(NULL); // header node
	append(head, &a); // index 0
	append(head, &b); // 1
	append(head, &a); // 2
	append(head, &c); // 3
	int expected = c;
	Node* end = get(head, 3);
	int actual = *((int*) end->element);
	if (actual != expected) {
		fprintf(stderr, "Node creation not as expected, got %d instead of %d\n", actual, expected);
		return 3;
	}

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
	if (testNodeCreation()) return 3;

	// Parse good file
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

	printTargets(targets);

	return 0;
}
