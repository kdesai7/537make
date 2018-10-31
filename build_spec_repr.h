// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H
#include "node.h"
typedef struct TargetInfo {
	char* name;
	Node* cmds; // each element is an array of strings, char**
	Node* deps; // each element is a string, char*
} TargetInfo;

typedef struct TargetInfoBuilder {
	Node* targets; // each element is a TargetInfo*
} TargetInfoBuilder;

TargetInfo* newTargetInfo();
TargetInfoBuilder* newTargetInfoBuilder();
int addNewTarget(TargetInfoBuilder* tib);
#endif

