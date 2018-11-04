// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>

#include "build_spec_repr.h"
#include "node.h"
#include "text_parsing.h"

const int BUFFSIZE = 256;

const int NUM_BAD_FILES = 5;
char* GOOD_FILE_NAME = "in.txt";

/**
 * Prints the error message to stderr
 * e.g. printerr("Oh no!") --> prints "ERROR: Oh no!" terminated by newline
 * to stderr
 */
void printerr(const char* msg) {
	fprintf(stderr, "ERROR: %s\n", msg);
}

int main() {
	int error = 0;
	char* badFiles[NUM_BAD_FILES];
	badFiles[0] = "spaces.txt";
	badFiles[1] = "multWords.txt";
	badFiles[2] = "twoColons.txt";
	badFiles[3] = "cmdWhitespace.txt";
	badFiles[4] = "meaningful.txt";

	TargetInfo* t = newTargetInfo();
	if (t == NULL) {
		fprintf(stderr, "newTargetInfo failed\n");
		return 1;
	}

	// Parse good file
	if (parse(GOOD_FILE_NAME) == NULL) {
		fprintf(stderr, "Failed to parse %s\n", GOOD_FILE_NAME);
		return -1;
	}

	// Parse bad files
	for (int i = 0; i < NUM_BAD_FILES; i++) {
		printf("Parsing %s\n", badFiles[i]);
		if (parse(badFiles[i]) != NULL) {
			fprintf(stderr, "Parsing of %s did not fail when it was supposed to fail\n", badFiles[i]);
			error = 1;
		}
		printf("Done parsing %s\n", badFiles[i]);
	}
	if (error) return 2;

	// Test node creation
	int a = 1;
	int b = 2;
	int c = 3;
	Node* head = newNode((void*) &a);
	append(head, &b);
	append(head, &a);
	append(head, &c);
	Node* end = get(head, 3);
	int actual = *((int*) end->element);
	if (actual != c) { // if failure
		fprintf(stderr, "Node creation not as expected\n");
		return 3;
	} else { // if success
		printf("Node creation successful\n");
	}

	return 0;
}
