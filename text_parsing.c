// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "build_spec_repr.h"

static const int TARGET = 0; // the name of the target
static const int DEPENDENCY = 1; // a dependency of some target
static const int COMMAND = 2; // the command to execute the target

/**
 * Adds the given token to the targets structure
 *
 * Returns 0 on success, nonzero on error.
 */
int processToken(char* token, int type) {
	printf("%s\n", token);
}

/**
 * Error codes:
 *     - 1: malloc failed
 *     - 2: file not found
 *     - 3: token too long
 *     - 4: line starts with space
 */
int parse(char* filename) {
	FILE* file;
	char* buffer; // stores one token at a time
	int c;
	int validToken = 0; // false iff buffer overflow
	int firstLine = 1; // firstLine has to start with the target

	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "File not found\n");
		return 2;
	}

	while (1) {
		buffer = (char*) malloc(BUFFSIZE * sizeof(char));
		if (buffer == NULL) {
			fprintf(stderr, "ERROR : Malloc failed\n");
			return 1;
		}
		validToken = 0; // assume invalid until told otherwise
		for (int i = 0; i < BUFFSIZE; i++) {
			c = getc(file);
			if (c == '\n') {
				if (i == 0)  { // we've encountered a blank line
					i = -1; // go back to the first element of the buffer
					printf("Empty line\n");
					continue;
				} else {
					buffer[i] = '\0';
					validToken = 1;
					break;
				}
			} // end 'if newline', no else needed because we continue or break

			if (c == ' ') {
				if (i == 0) {
					fprintf(stderr, "Line starts with space\n");
					return 4;
				}
				buffer[i] = '\0';
				validToken = 1;
				break;
			}

			if (c == EOF) {
				if (i == 0) {
					return 0;
				}
				buffer[i] = '\0';
				validToken = 1;
				return 0;
			} // end if EOF

			buffer[i] = c;
		} // end buffer population

		if (!validToken) {
			fprintf(stderr, "Token too long\n");
			return 3;
		}
		processToken(buffer, -1);
	}

	fclose(file);
	return 0;
}
