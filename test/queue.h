// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#ifndef QUEUE_H
#define QUEUE_H
typedef struct Queue {
	pthread_mutex_t mutex;
	pthread_cond_t empty;
	pthread_cond_t full;
	char **elements;
	int capacity;
	int first;
	int last;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
} Queue;
Queue *createStringQueue(int capacity);
int enqueueString(Queue *queue, char *string);
char* dequeueString(Queue *queue);
int dequeueCount(Queue* q);
int modIncrement(Queue *q, int index);
void printQueueInfo(Queue *q, char* name);
#endif
