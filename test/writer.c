// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "main.h"
#include "queue.h"

void* write(void* v) {
	Queue* q = (Queue*) v;

	while (1) {
		char *dequeued = dequeueString(q);
		if (dequeued != NULL) { // not end of thread
			printf("%s\n", dequeued);
			free(dequeued);
		} else {
			printf("Strings processed: %d\n", dequeueCount(q) + 1);
			pthread_exit(NULL);
		}
	}
}
