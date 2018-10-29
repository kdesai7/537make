// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#ifndef TEXT_PARSING_C
#define TEXT_PARSING_C
typedef struct TargetInfoBuilder {
	TargetInfo** targets; // array of pointers to targets
	int MAX_CAPACITY; // length of targets array
	int currentIndex; // index of the current target
} TargetInfoBuilder;
TargetInfo** parse(char* filename);
#endif
