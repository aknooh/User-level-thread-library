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

// global access array (all threads)
queue_t queue;		
struct *uthread_tcb curThread;
int thread_id = 0;							

typedef enum
{
	RUNNING,
	READY,
	BLOCKED,
	UNBLOCKED,
	IDLE
} uthread_state ;

struct thread_func{

};

struct uthread_tcb {
	uthread_ctx_t *context;
	void* stack;
	enum STATE state;
	int id;
};

<<<<<<< Updated upstream
// not needed im using a global
// We actually need this for later
// struct uthread_tcb *uthread_current(void)
// {
// 	/* TODO Phase 2 */
// }
=======
not needed im using a global
struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2 */
}
>>>>>>> Stashed changes


void uthread_yield(void)
{
	// block current state
	curThread->state = BLOCKED;

	// store the element that is dequeued
	void **front = malloc(sizeof(void**));
	if (queue_dequeue(threadList, front) == -1) 
		fprintf(stderr, "Failure to dequeue from queue.\n");

	struct uthread_tcb *next = *front;
	curThread = next;

	// switch with the dequeue one
	uthread_ctx_switch(curThread->context, next->context);
	queue_enqueue(queue, curThread);
}

int uthread_create(uthread_func_t func, void *arg)
{
	// initialize thread and current thread
	curThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	struct uthread_tcb* thread = 
				(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if (thread == NULL) {
		fprintf(stderr, "Failure to allocate memory to thread tcb.\n");
		return;
	}

	// initialize thread properties
	thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	thread->state = RUNNING;
	thread->id = thread_id++;
	thread->stack = uthread_ctx_alloc_stack();

	// initialize thread execution context
	if (uthread_ctx_init(thread->context, thread->stack, func, arg) == -1) {
		fprintf(stderr, "Failure to initialize execution context");
		return -1;
	}
	
	// enqueue thread
	queue_enqueue(queue, thread);
	return 0;
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


void uthread_start(uthread_func_t start, void *arg)
{
	// initialize queue
	queue = queue_create();
	if (queue == NULL) {
		fprintf(stderr, "Failure to allocate memory to queue. \n");
		return;
	}

	// initialize main idle thread and current thread
	curThread = (struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	struct uthread_tcb* idle_thread = 
				(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if (idle_thread == NULL) {
		fprintf(stderr, "Failure to allocate memory to thread tcb.\n");
		return;
	}

	// initialize thread properties
	idle_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	idle_thread->state   = IDLE;
	idle_thread->id      = thread_id++;
	idle_thread->stack   = uthread_ctx_alloc_stack();			// we don't need to initialize the stack for idle

	// set current thread to
	curThread = idle_thread;

	if(uthread_create(start, arg) == -1) {
		fprintf(stderr, "Error: fail to create idle_thread.\n");
		return ;
	}

	// set idle 
	while(queue_length(queue) != 0) 
		uthread_yield();
	
	free(curThread);
}
