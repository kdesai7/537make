// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>


#include "build_spec_repr.h"
#include "main.h"
#include "text_parsing.h"

TargetInfo* newTargetInfo() {
	TargetInfo* t;

	t = (TargetInfo*) malloc(sizeof(TargetInfo));
	if (t == NULL) {
		fprintf(stderr, "malloc for new target info failed\n");
		return NULL;
	}
	t->name = (char*) malloc(BUFFSIZE * sizeof(char));
	if (t->name == NULL) {
		fprintf(stderr, "malloc for name failed\n");
		return NULL;
	}
	t->cmds = (Node*) malloc(sizeof(Node));
	if (t->cmds == NULL) {
		fprintf(stderr, "malloc for cmds failed\n");
		return NULL;
	}
	t->deps = (Node*) malloc(sizeof(Node));
	if (t->deps == NULL) {
		fprintf(stderr, "malloc for deps failed\n");
		return NULL;
	}

	return t;
}

TargetInfoBuilder* newTargetInfoBuilder() {
	TargetInfoBuilder* t =
		(TargetInfoBuilder*) malloc(sizeof(TargetInfoBuilder));
	t->targets = (Node*) malloc(sizeof(Node));
	return t;
}

int addNewTarget(TargetInfoBuilder* tib) {
	return 0;
}
