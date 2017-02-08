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
struct uthread_tcb* curThread;
int thread_id = 0;							


typedef enum
{
	RUNNING,
	READY,
	BLOCKED,
	WAIT,
	NO_STATE,
	ZOMBIE,
	TERMINATED
} uthread_state_t;


struct uthread_tcb {
	uthread_ctx_t   *context;
	void* 			stack;
	uthread_state_t state;
	int 			id;
};


// A thread array to store blocked threads
struct uthread_tcb* blocked;
int index = -1;


struct uthread_tcb *uthread_current(void)
{
	return curThread;
}



void uthread_yield(void)
{
	// save current state and block it
	struct uthread_tcb* cur_save = uthread_current();
	curThread->state = BLOCKED;

	// save oldest element in the queue
	void **front = malloc(sizeof(void**));
	if (queue_dequeue(queue, front) == -1) {
		fprintf(stderr, "Failure to dequeue from queue.\n");
		return;
	}
	// the oldest element in the queue is now ready 
	// to be next thread in context execution
	struct uthread_tcb *next = *front;
	next->state = READY;

	// current thread becomes the new thread from queue
	curThread = next;

	// switch context from the previous one 
	// to the new one from the dequeue
	uthread_ctx_switch(cur_save->context, next->context);

	// enqueue the old one back into the queue
	queue_enqueue(queue, cur_save);
}


int uthread_create(uthread_func_t func, void *arg)
{
	// initialize thread
	struct uthread_tcb* thread = 
				(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if (thread == NULL) {
		fprintf(stderr, "Failure to allocate memory to thread tcb.\n");
		return -1;
	}

	// initialize thread properties
	thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	if (thread->context == NULL) {
		fprintf(stderr, "Failure to allocate memory to context.\n");
		return -1;
	}
	thread->state   = READY;
	thread->id 		= thread_id++;
	thread->stack   = uthread_ctx_alloc_stack();

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
	struct uthread_tcb *cur = uthread_current();
	cur->state = TERMINATED;
	free(cur->stack);
	free(cur->context);

	// if ()
	// free(cur);
	uthread_yield();
}

void uthread_block(void)
{
	/* TODO Phase 2 */
	// First, save current thread 
	// Second, block current thread 
	struct uthread_tcb* newBthread = 
				(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if(newBthread == NULL){
		fprintf(stderr, "Failure to allocate new Bthread.\n");
		return -1;
	}

	newBthread->context = curThread->context;
	newBthread->state = BLOCKED;
	newBthread->id = curThread->id;
	newBthread->stack = curThread->stack;
	if(index == -1){
		blocked = (struct uthread_tcb*)malloc(10* (sizeof(struct uthread_tcb) / (struct uthread_tcb) );
	}
	index++;
	if(index >= sizeof(struct uthread_tcb)){
		blocked = realloc(blocked, (sizeof(struct uthread_tcb) / (struct uthread_tcb) * 2);		// double size of the array
	}
	else{
		blocked[index] = newBthread;
	}
	curThread->state = BLOCKED;
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
		fprintf(stderr, "Failure to allocate memory to queue.\n");
		return;
	}

	// initialize main idle thread and current thread
	struct uthread_tcb* idle_thread = 
				(struct uthread_tcb*)malloc(sizeof(struct uthread_tcb));
	if (idle_thread == NULL) {
		fprintf(stderr, "Failure to allocate memory to thread tcb.\n");
		return;
	}

	// initialize thread properties
	idle_thread->context = (uthread_ctx_t *)malloc(sizeof(uthread_ctx_t));
	if (idle_thread->context == NULL) {
		fprintf(stderr, "Failure to allocate memory to context.\n");
		return;
	}
	idle_thread->state   = NO_STATE;
	idle_thread->id      = thread_id++;

	// set current thread to
	curThread = idle_thread;

	if(uthread_create(start, arg) == -1) {
		fprintf(stderr, "Error: fail to create idle_thread.\n");
		return ;
	}

	// set idle 
	while(queue_length(queue) != 0) 
		uthread_yield();

	uthread_exit();
}
