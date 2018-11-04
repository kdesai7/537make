// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>

#include "build_spec_repr.h"
#include "main.h"
#include "node.h"
#include "text_parsing.h"

/**
 * Pretty-prints the contents of one node for debugging
 * Assumes targetNode is non-null, element of type TargetInfo*
 * Output is terminated by newline
 */
void printTarget(Node* targetNode) {
	TargetInfo* target = (TargetInfo*)(targetNode->element);
	Node* depNode = target->deps;
	Node* cmdNode = target->cmds;
	char* dep; // one printable dependency
	char** cmd;

	// Print name and dependencies
	printf("%s:", target->name);
	while (depNode->next != NULL) {
		depNode = depNode->next;
		dep = (char*)depNode->element;
		printf(" %s", dep);
	}
	printf("\n");

	// Print commands
	while (cmdNode->next != NULL) {
		cmdNode = cmdNode->next;
		cmd = (char**)cmdNode->element;
		printf("\t");
		for (int i = 0; cmd[i] != NULL; i++) {
			if (i != 0) printf(" ");
			printf("%s", cmd[i]);
		}
		printf("\n");
	}
}

/**
 * Print the parsed targets for debugging purposes
 * Assumes the node's elements are of type TargetInfo*
 * Assumes given node is non-null header node
 */
void printTargets(Node* targetHeader) {
	Node* curr = targetHeader;
	while (curr->next != NULL) {
		curr = curr->next;
		printTarget(curr);
		printf("\n"); // extra blank line for readability
	}
}

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
	t->tail = t->targets;
	return t;
}

/**
 * Tries to add a new target to the given tib
 * tokens are the tokens of the target line, assumed non-null, length >= 1
 * Assumes tokens ends with NULL pointer
 * Returns 0 on success, nonzero on failure
 * Returns 1 if newTargetInfo fails
 * Returns 2 if newNode fails
 * Returns 3 if append fails
 */
int addNewTarget(TargetInfoBuilder* tib, char** tokens) {
	int error = 0;

	TargetInfo* newTarget = newTargetInfo(); // will print if fails
	if (newTarget == NULL) return 1; // so don't print here

	newTarget->name = tokens[0];
	for (int i = 1; tokens[i] != NULL; i++) {
		error = append(newTarget->deps, (void*)tokens[i]);
		if (error) return 3;
	}

	error = append(tib->targets, newTarget);
	if (error) return 3;

	tib->tail = tib->tail->next; // Update tail reference

	return 0;
}

/**
 * Adds the given tokens as a command to the last target
 * Returns 0 on success
 * Returns 1 if node creation fails
 * Returns received error code if append fails
 */
int addNewCommand(TargetInfoBuilder* tib, char** tokens) {
	int error = 0;

	TargetInfo* lastTarget = (TargetInfo*)tib->tail->element;
	error = append(lastTarget->cmds, (void*)tokens);
	if (error) return error;

	return 0;
}
