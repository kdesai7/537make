// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "build_spec_repr.h"
#include "text_parsing.h"

static const int TARGET = 0; // the name of the target
static const int DEPENDENCY = 1; // a dependency of some target
static const int COMMAND = 2; // beginning of new command to execute the target
static const int ARGUMENT = 3; // argument to a command

static const char* MSG_MALLOC = "Malloc failed";
static const char* MSG_FILE_NOT_FOUND = "File not found";
static const char* MSG_FILE_NOT_CLOSED = "File failed to close";
static const char* MSG_TOKEN_TOO_LONG = "Token too long";
static const char* MSG_LINE_STARTS_WITH_SPACE = "Line starts with space";

static const int ERR_INVALID_TOKEN_TYPE = 1;

/**
 * Adds the given token to the targets structure
 *
 * Returns 0 on success, nonzero on error.
 *
 */
int processToken(char* token, int type) {
	if (type != TARGET
		&& type != DEPENDENCY
		&& type != COMMAND
		&& type != ARGUMENT
	) { // invalid
		fprintf(stderr, "Invalid token type\n");
		return ERR_INVALID_TOKEN_TYPE;
	}
	printf("%s\n", token);
	return 0;
}

TargetInfoBuilder* newTargetInfoBuilder(int capacity) {
	TargetInfoBuilder* t =
		(TargetInfoBuilder*) malloc(sizeof(TargetInfoBuilder));
	t->targets = (TargetInfo**) malloc(capacity * sizeof(TargetInfo*));
	t->MAX_CAPACITY = capacity;
	t->currentIndex = 0;
	return t;
}

TargetInfo** parse(char* filename) {
	TargetInfoBuilder* tib = newTargetInfoBuilder(MAX_TARGETS);

	FILE* file;
	char* buffer; // stores one token at a time
	int c;
	int validToken = 0; // false iff buffer overflow

	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "%s\n", MSG_FILE_NOT_FOUND);
		return NULL;
	}

	while (1) {
		buffer = (char*) malloc(BUFFSIZE * sizeof(char));
		if (buffer == NULL) {
			fprintf(stderr, "%s\n", MSG_MALLOC);
			return NULL;
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
					fprintf(stderr, "%s\n", MSG_LINE_STARTS_WITH_SPACE);
					return NULL;
				}
				buffer[i] = '\0';
				validToken = 1;
				break;
			}

			if (c == EOF) {
				if (i == 0) { // Done with the method
					if (fclose(file)) {
						fprintf(stderr, "%s\n", MSG_FILE_NOT_CLOSED);
						return NULL;
					}
					return tib->targets;
				}
				buffer[i] = '\0';
				validToken = 1;
				return 0;
			} // end if EOF

			buffer[i] = c;
		} // end buffer population

		if (!validToken) {
			fprintf(stderr, "%s\n", MSG_TOKEN_TOO_LONG);
			return NULL;
		}
		processToken(buffer, -1);
	} // end infinite while
}
