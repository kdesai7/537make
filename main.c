// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>

#include "build_spec_repr.h"

const int BUFFSIZE = 256;
const int MAX_ARG_COUNT = 128;
const int MAX_CMD_COUNT = 128;
const int MAX_DEP_COUNT = 128;

int main() {
	TargetInfo* t = newTargetInfo();
	if (t == NULL) {
		return 1;
	}
	return 0;
}
