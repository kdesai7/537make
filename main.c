// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>

#include "build_spec_repr.h"
#include "text_parsing.h"

const int BUFFSIZE = 256;
const int MAX_ARG_COUNT = 128;
const int MAX_CMD_COUNT = 128;
const int MAX_DEP_COUNT = 128;

const int NUM_FILES = 3;

int main() {
	int error = 0;
	char* files[NUM_FILES];
	files[0] = "in.txt";
	files[1] = "spaces.txt";
	files[2] = "blank.txt";

	TargetInfo* t = newTargetInfo();
	if (t == NULL) {
		fprintf(stderr, "newTargetInfo failed\n");
		return 1;
	}

	for (int i = 0; i < NUM_FILES; i++) {
		printf("Parsing %s\n", files[i]);
		if (i == 1) {
			printf("This should fail...\n");
		}
		if (parse(files[i])) {
			fprintf(stderr, "Parsing of %s failed\n", files[i]);
			if (i != 1) { // i == 1 should fail, parsing spaces should fail
				error = 1;
			}
		}
		printf("Done parsing %s\n", files[i]);
	}
	if (error) return 2;

	return 0;
}
