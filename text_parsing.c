// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "build_spec_repr.h"

int parse(char* filename) {
	char* buffer;
	int c;
	int validLine = 0;
	int firstLine = 1;	// firstLine has to start with the target

	while (1) {
		buffer = (char*) malloc(BUFFSIZE * sizeof(char));
		if (buffer == NULL) {
			fprintf(stderr, "ERROR : Malloc failed\n");
			return 1;
		}
		for (int i=0; i < BUFFSIZE; i++) {
			c = getchar();
			if (c == '\n') {
				if (i == 0)  {	 // blank line
					i = -1;   // go back to the first element of the buffer
					continue;	
				} else {
					buffer[i] = '\0';
					validLine = 1;
					firstLine = 0;
					break;
				}
			}
		}
	}
	return 0;
}
