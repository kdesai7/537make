// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "build_spec_repr.h"
#include "text_parsing.h"

static const char* MSG_MALLOC = "Malloc failed";
static const char* MSG_FILE_NOT_FOUND = "File not found";
static const char* MSG_FILE_NOT_CLOSED = "File failed to close";
static const char* MSG_LINE_TOO_LONG = "Token too long";
static const char* MSG_LINE_STARTS_WITH_SPACE = "Line starts with space";
static const char* MSG_NULL_TERMINATOR = "Line contains null terminator";
static const char* MSG_NO_TERMINATOR = "No null terminator at end of buffer";

/**
 * Adds the given line to the target's structure
 *
 * Assumes the given line is valid
 * 
 * isTargetLine is truthy if it is a target line, falsy if it is a command line
 *
 * Returns 0 on success, nonzero on error.
 */
int processLine(TargetInfoBuilder* tib, char* line, int isTargetLine) {
	if (isTargetLine) {
		addNewTarget(tib);
	} else { // it's a command line
		// TODO add new command
	}

	char* lineType = isTargetLine ? "TARG" : "CMDS";
	printf("%s:%s\n", lineType, line); // for debugging purposes
	return 0;
}

int isWhitespaceOrColon(char c) {
	return c == ':' || c == ' ' || c == '\t';
}

/**
 * Make sure line is valid makefile line and returns line type
 * Line cannot start with space (Error code -1)
 * Line cannot contain null terminators (Error code -1)
 * Line must end with null terminator (Error code -1)
 * if target line : return 1; command line: return 0; invalid line : return -1 
 */
int determineLineType(char* buffer, int length, int firstMeaningfulLine) {
	// printf("Validating >>%s<<, length %d\n", buffer, length);
	int expectingTargetLine = 0;
	int foundColon = 0; // whether we have encountered a colon yet
	int foundWhitespace = 0; // whether we have encountered whitespace yet
	char c = buffer[0];
	if (c == ' ') {
		fprintf(stderr, "%s\n", MSG_LINE_STARTS_WITH_SPACE);
		return -1; // invalid Line
	}

	expectingTargetLine = (c == '\t') ? 0 : 1;

	if (firstMeaningfulLine && !expectingTargetLine) {
		fprintf(stderr, "%s\n", "First meaningful line isn't a target");
		return -1;
	}

	if (!expectingTargetLine && (buffer[1] == '\t' || buffer[1] == ' ')) {
		fprintf(stderr, "%s\n", "Command line has too much whitespace");
		return -1;
	}

	for (int i = 0; i < length; i++) {
		c = buffer[i];
		if (c == '\0') {
			fprintf(stderr, "%s\n", MSG_NULL_TERMINATOR);
			return -1;
		}
		if (expectingTargetLine) {
			if (foundColon && c == ':') { // if we find a second colon
				fprintf(stderr, "%s\n", "Target line has multiple colons");
				return -1;
			}
			// two words before colon makes something invalid
			if (foundWhitespace && !foundColon && !isWhitespaceOrColon(c)) {
				fprintf(stderr, "%s\n", "Multiple words before colon");
				return -1;
			}

			if (c == ':') foundColon = 1;
			if (c == ' ' || c == '\t') foundWhitespace = 1;
		}
    }

	if (buffer[length] != '\0') {
		fprintf(stderr, "%s\n", MSG_NO_TERMINATOR);
		return -1;
	}

	return expectingTargetLine ? 1 : 0;
}

/**
 * Parse the given file, return a linked list of TargetInfo elements
 * Return NULL on failure
 */
Node* parse(char* filename) {
	TargetInfoBuilder* tib = newTargetInfoBuilder();

	FILE* file;
	char* buffer; // stores one line at a time
	int c;
	int validBuffer = 0; // false iff buffer overflow
	int length = 0; 
	int firstMeaningfulLine = 1; // truthy if parsing first meaningful line

	// Open file
	file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "%s\n", MSG_FILE_NOT_FOUND);
		return NULL;
	}

	// Parse the file
	while (1) {
		// allocate the buffer
		buffer = (char*) malloc(BUFFSIZE * sizeof(char));
		if (buffer == NULL) {
			fprintf(stderr, "%s\n", MSG_MALLOC);
			return NULL;
		}

		validBuffer = 0; // assume line invalid until told otherwise

		// populate the buffer for one line
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
				length = i;
				break;
			} // end of 'if end of line'

			buffer[i] = c;
		} // end buffer population

		if (!validBuffer) {
			fprintf(stderr, "%s\n", MSG_LINE_TOO_LONG);
			return NULL;
		}

		// ignore blank lines and comment lines
		if (length > 0 && buffer[0] != '#') { // if meaningful line
			int result = determineLineType(buffer, length, firstMeaningfulLine);
			firstMeaningfulLine = 0;
			if (result == -1) { // line is invalid
				return NULL;
			}
			processLine(tib, buffer, result);
		} else { // ignore this line, free it
			free(buffer);
		}
	} // end infinite while
}
