// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "queue.h"

static const int MSG_LENGTH = 256;

/** capacity is the max number of elements */
Queue * createStringQueue(int capacity) {
	Queue* q;
	int error = 0;

	q = (Queue*) malloc(sizeof(Queue));
	if (q == NULL) {
		printf("malloc failed\n");
		return NULL;
	}
	if ((error = pthread_mutex_init(&(q->mutex), NULL))) { // initialize with default values
		printf("pthread_mutex_init failed with error code %d. Exiting...\n", error);
		return NULL;
	}
	if ((error = pthread_cond_init(&(q->empty), NULL))) {
		printf("pthread_cond_init 1 failed with error code %d. Exiting...\n", error);
		return NULL;
	}
	if ((error = pthread_cond_init(&(q->full), NULL))) {
		printf("pthread_cond_init 2 failed with error code %d. Exiting...\n", error);
		return NULL;
	}
	q->elements = (char**) malloc(capacity * sizeof(char*));
	if (q->elements == NULL) {
		printf("malloc 2 failed\n");
		return NULL;
	}

	q->capacity = capacity;
	q->first = 0;
	q->last = 0;

	q->enqueueCount = 0;
	q->dequeueCount = 0;
	q->enqueueBlockCount = 0;
	q->dequeueBlockCount = 0;

	return q;
}

int enqueueString(Queue *q, char *string) {
	int error = 0;
	if ((error = pthread_mutex_lock(&(q->mutex)))) {
		fprintf(stderr, "Error locking in enqueueString, returning error code...\n");
		return error;
	}

	while (modIncrement(q, q->last) == q->first) { // while full
		q->enqueueBlockCount++;
		if ((error = pthread_cond_wait(&(q->full), &(q->mutex)))) {
			fprintf(stderr, "Error waiting in enqueueString, returning error code...\n");
			return error;
		}
	}

	q->elements[q->last] = string;

	q->last = modIncrement(q, q->last);
	if ((error = pthread_cond_signal(&(q->empty)))) {
		fprintf(stderr, "Error signaling in enqueueString, returning error code...\n");
		return error;
	}

	if (string != NULL) q->enqueueCount++;

	if ((error = pthread_mutex_unlock(&(q->mutex)))) {
		fprintf(stderr, "Error unlocking in enqueueString, returning error code...\n");
		return error;
	}
	return 0;
}

char * dequeueString(Queue *q) {
	char *string;
	int error = 0;
	if ((error = pthread_mutex_lock(&(q->mutex)))) {
		fprintf(stderr, "Error %d locking in dequeueString, returning...\n", error);
		return NULL;
	}

	while (q->first == q->last) { // while empty
		q->dequeueBlockCount++;
		if ((error = pthread_cond_wait(&(q->empty), &(q->mutex)))) {
			fprintf(stderr, "Error %d waiting in dequeueString, returning...\n", error);
			return NULL;
		}
	}

	if (q->elements[q->first] != NULL) {
		string = q->elements[q->first];
	} else {
		string = NULL;
	}

	q->first = modIncrement(q, q->first);
	if ((error = pthread_cond_signal(&(q->full)))) {
		fprintf(stderr, "Error %d signaling in dequeueString, returning...\n", error);
		return NULL;
	}

	if (string != NULL) q->dequeueCount++;

	if ((error = pthread_mutex_unlock(&(q->mutex)))) {
		fprintf(stderr, "Error %d unlocking in dequeueString, returning...\n", error);
		return NULL;
	}
	return string;
}

int dequeueCount(Queue* q) {
	return q->dequeueCount;
}

int modIncrement(Queue *q, int index) {
	return (index + 1) % (q->capacity);
}

void printQueueInfo(Queue* q, char* name) {
	char msg[MSG_LENGTH];
	strcpy(msg, "%s:\n");
	strcat(msg, "\tenqueueCount: %d\n");
	strcat(msg, "\tdequeueCount: %d\n");
	strcat(msg, "\tenqueueBlockCount: %d\n");
	strcat(msg, "\tdequeueBlockCount: %d\n");
	printf(msg,
		name,
		q->enqueueCount,
		q->dequeueCount,
		q->enqueueBlockCount,
		q->dequeueBlockCount);
}

