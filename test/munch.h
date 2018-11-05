// 1,Mark Wiemer,mwiemer2,mww,mwiemer2@wisc.edu,9074356420
// 2,Jenny Ye,hye35,haengjung,hye35@wisc.edu,9075878315

#ifndef MUNCH_H
#define MUNCH_H

#include <pthread.h>

#include "queue.h"

typedef struct MunchArgs {
	Queue* in;
	Queue* out;
} MunchArgs;

void* munch1(void* queues);
void* munch2(void* queues);
#endif
