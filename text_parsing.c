// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "build_spec_repr.h"

/**
 *
 * Error codes:
 *     - 1: malloc failed
 *     - 2: file not found
 *     - 3: token too long
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
					firstLine = 0;
					break;
				}
			} // end 'if newline', no else needed because we continue or break

			if (c == EOF) {
				buffer[i] = '\0';
				validToken = 1;
				firstLine = 0;
				return 0;
			} // end if EOF

			buffer[i] = c;
		} // end buffer population

		if (!validToken) {
			fprintf(stderr, "Token too long\n");
			return 3;
		}
		printf("%s\n", buffer);
	}

	fclose(file);
	return 0;
}
