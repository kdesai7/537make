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
static const char* MSG_NULL_TERMINATOR = "Line contains null terminator";
static const char* MSG_NO_TERMINATOR = "No null terminator at end of buffer";

static const int ERR_INVALID_TOKEN_TYPE = 1;

/**
 * Adds the given line to the target's structure
 *
 * Returns 0 on success, nonzero on error.
 *
 */
int processLine(TargetInfoBuilder* tib, char* line, int type) {
	if (type == TARGET) {
		addNewTarget(tib);
	} else if (type != DEPENDENCY
		&& type != COMMAND
		&& type != ARGUMENT
	) { // invalid
		fprintf(stderr, "Invalid line type\n");
		return ERR_INVALID_TOKEN_TYPE;
	}
	printf("%s\n", line);
	return 0;
}

/**
 * Make sure line is valid makefile line
 * Line cannot start with space (Error code 1)
 * Line cannot contain null terminators (Error code 2)
 * Line must end with null terminator (Error code 3)
 */
int validateLine(char* buffer, int length) {
	// printf("Validating >>%s<<, length %d\n", buffer, length);
	char c = buffer[0];
	if (c == ' ') {
		fprintf(stderr, "%s\n", MSG_LINE_STARTS_WITH_SPACE);
		return 1;
	}
	for (int i = 0; i < length; i++) {
		c = buffer[i];
		if (c == '\0') {
			fprintf(stderr, "%s\n", MSG_NULL_TERMINATOR);
			return 2;
		}
	}
	if (buffer[length + 1] != '\0') {
		fprintf(stderr, "%s\n", MSG_NO_TERMINATOR);
		return 3;
	}
	return 0;
}

/**
 * Parse the given file, return a linked list of TargetInfo elements
 * Return NULL on failure
 */
Node* parse(char* filename) {
	TargetInfoBuilder* tib = newTargetInfoBuilder(MAX_TARGETS);

	FILE* file;
	char* buffer; // stores one line at a time
	int c;
	int validBuffer = 0; // false iff buffer overflow
	int length = 0;

	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "%s\n", MSG_FILE_NOT_FOUND);
		return NULL;
	}

	while (1) {
		// allocate the buffer
		buffer = (char*) malloc(BUFFSIZE * sizeof(char));
		if (buffer == NULL) {
			fprintf(stderr, "%s\n", MSG_MALLOC);
			return NULL;
		}

		validBuffer = 0; // assume line invalid until told otherwise

		// populate the buffer
		for (int i = 0; i < BUFFSIZE; i++) {
			c = getc(file);

			if (c == EOF || c == '\n') { // end of line
				if (c == EOF && i == 0) { // end of file
					if (fclose(file)) {
						fprintf(stderr, "%s\n", MSG_FILE_NOT_CLOSED);
						return NULL;
					}
					return tib->targets; // return the targets
				}
				buffer[i] = '\0';
				validBuffer = 1;
				length = i - 1;
				break;
			} // end of 'if end of line'

			buffer[i] = c;
		} // end buffer population

		if (!validBuffer) {
			fprintf(stderr, "%s\n", MSG_TOKEN_TOO_LONG);
			return NULL;
		}

		// ignore blank lines and comment lines
		if (length > 0 && buffer[0] != '#') {
			int result = validateLine(buffer, length);
			if (result != 0 && result != -1) {
				return NULL;
			}
			processLine(tib, buffer, TARGET);
		} else { // ignore this line, free it
			free(buffer);
		}
	} // end infinite while
}
