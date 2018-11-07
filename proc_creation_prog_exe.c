// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "main.h"
#include "node.h"

/**
 * Run each build command in a new process,
 * Waiting for its completion,
 * Get the return code
 *
 * Return 0 on success ; nonzero on failure
 */
int executeCommand(char** command) {
	pid_t child_pid, pid;
	int status;

	child_pid = fork();

	if (child_pid == 0) { // child process
		// Print command
		for (int i = 0; command[i] != NULL; i++) {
			if (i != 0) printf(" ");
			printf("%s", command[i]);
		}
		printf("\n");

		execvp(command[0], command);
		// if exec fails
		perror("execvp");
	} else if (child_pid > 0) { // parent process
		// if library call fails
		if ( (pid = wait(&status)) < 0) {
				perror("wait");
				exit(EXIT_FAILURE);
		}
		if (WIFEXITED(status)) { // child is done
			if (WEXITSTATUS(status)) { // command failed
				printerr("Command failed");
				exit(EXIT_FAILURE);
			} else { // command succeeded
				return 0;
			}
		}
	} else { // fork failed
		perror("fork failed");
		exit(EXIT_FAILURE);
		return -1;
	}
	return 0;
}

/**
 * Executes all commands in the linked list
 * Assumes cmdHeader is a non-null header node
 * Assumes elements are of type char**
 * Returns 0 on success, nonzero on failure
 */
int executeCommands(Node* cmdHeader) {
	Node* cmdNode = cmdHeader;
	char** cmd;

	while ((cmdNode = cmdNode->next) != NULL) {
		cmd = (char**)cmdNode->element;
		if (executeCommand(cmd)) {
			return 1;
		}
	}

	return 0;
}
