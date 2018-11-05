// student,full name (as in Canvas),Net ID,CS ID,wisc email (not CS email),campus ID number
// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

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

	if (child_pid == 0) {
		// if child process
		printf ("Child : executing, %s\n", command[0]);

		execvp(command[0], command);

		//if exec fails
		perror("execve failed");
	}else if (child_pid > 0) {
		// if parent process
		// if library call fails
		if ( (pid = wait(&status)) < 0) {
		       perror("wait");
	       	       exit(EXIT_FAILURE);
		}
		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status)) {
				printf("command failed\n");
			} else {
				printf("command succeeded\n");
			}
		}
 		printf("Parent finished executing\n");
	} else {
		perror ("fork failed");
	       	exit(EXIT_FAILURE);
		return -1;
	}
	return 0;
}
