#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define _UTHREAD_PRIVATE
#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

queue_t Queue;					// main Queue to hold threads 
								// global so all functions can access it

// thread function struct to store a function
// and its argument in the queue.
struct thread_func{
	char uthread_func_name[64];
	void *arg;
	//char *argChar;				// if arg is char
//	int argInt;					// if arg is int
};


struct uthread_tcb {
	/* TODO Phase 2 */
};

void uthread_yield(void)
{
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
}

void uthread_exit(void)
{
	/* TODO Phase 2 */
}

void uthread_block(void)
{
	/* TODO Phase 2 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2 */
}

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
}

void uthread_start(uthread_func_t start, void *arg)
{
	int newStatus;
	thread_func start_func;
	strcpy(start_func.uthread_func_name, (char**) start);
	start_func.arg = arg; 
	Queue = queue_create(void);		// create a a Queue object
	newStatus = queue_enqueue(Queue,start);

// I added some new stuff
while(Queue.length()>-1)
{
	uthread_yield();		// yield to other threads 
}


	/* TODO Phase 2 */
}
