// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#ifndef BUILD_SPEC_REPR_H
#define BUILD_SPEC_REPR_H
typedef struct TargetInfo {
	char* name;
	char*** cmds;
	char** deps;
} TargetInfo;

typedef struct TargetInfoBuilder {
	TargetInfo** targets; // array of pointers to targets
	int MAX_CAPACITY; // length of targets array
	int targetIndex; // index of the current target
} TargetInfoBuilder;

TargetInfo* newTargetInfo();
TargetInfoBuilder* newTargetInfoBuilder(int capacity);
int addNewTarget(TargetInfoBuilder* tib);
#endif

