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
// note: semaphore_queue always contains blocked ppl
queue_t queue, semaphore_queue;		
struct uthread_tcb* curThread, *cur_sem_thread;
int thread_id = 0;							


typedef enum
{
	RUNNING,
	READY,
	BLOCKED,
	TERMINATED
} uthread_state_t;


struct uthread_tcb {
	uthread_ctx_t   *context;
	void* 			stack;
	uthread_state_t state;
	int 			id;
};


struct uthread_tcb *uthread_current(void)
{
	return curThread;
}



void uthread_yield(void)
{
	// save current state
	struct uthread_tcb* cur_save = uthread_current();

	// if the current state is running, then it hasnt
	// finished its execution, so its we reset its 
	// state to ready.
	if (cur_save->state == RUNNING)
		cur_save->state = READY;

	// save oldest element in the queue
	struct uthread_tcb *front;

	// Disable Preemption
	if(!(preempt_disabled())){
		preempt_disable();
	}	
	if (queue_dequeue(queue, (void**) &front) == -1) {
		fprintf(stderr, "Failure to dequeue from queue.\n");
		return;
	} //printf("id: %d\n", front->id);

	preempt_enable();
	// the oldest element in the queue is now ready 
	// to be next thread in context execution but it
	// had to have come from a ready state since it was queued
	assert(front->state == READY);

	// and is now the next context execution to run 
	front->state = RUNNING;

	// current thread becomes the new thread from queue
	curThread = front;

	// enqueue the old one to the back of the queue
	if (cur_save->state == READY)
	{		
		// Disable Preemption
		if(!(preempt_disabled())){
			preempt_disable();
		}	
		queue_enqueue(queue, cur_save);
		preempt_enable();
	}
	
	// switch context from the previous one 
	// to the new one from the dequeue
	uthread_ctx_switch(cur_save->context, front->context);

	if (cur_save->state == TERMINATED) {
		free(cur_save->context);
		free(cur_save->stack);
		free(cur_save);
		cur_save = NULL;
	}
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
		
	// Disable Preemption
	if(!(preempt_disabled())){
		preempt_disable();
		}	
	queue_enqueue(queue, thread);
	preempt_enable();
	return 0;
}


void uthread_exit(void)
{
	struct uthread_tcb *cur_running = uthread_current();

	free(cur_running->context);
	free(cur_running->stack);
	cur_running->state = TERMINATED;

	// goto next thread in queue
	uthread_yield();
}


void uthread_block(void)
{
	curThread->state = BLOCKED;
	uthread_yield();
}


void uthread_unblock(struct uthread_tcb *uthread)
{
	// must come from a blocked stated
	assert(uthread->state == BLOCKED);

	// find the thread in the queue and change state
	uthread->state = READY;

	
	// Disable Preemption
	if(!(preempt_disabled())){
		preempt_disable();
		}	
	// enqueue back into the queue 
	queue_enqueue(queue, uthread);
	preempt_enable();
}


void uthread_start(uthread_func_t start, void *arg)
{
	// Start Preemption
	preempt_start();
	// initialize queue
		
	// Disable Preemption
	if(!(preempt_disabled())){
		preempt_disable();
		}	
	queue = queue_create();
	preempt_enable();
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
	idle_thread->state   = RUNNING;
	idle_thread->id      = thread_id++;

	// set current thread to
	curThread = idle_thread;
	//preempt_start();

	if(uthread_create(start, arg) == -1) {
		fprintf(stderr, "Error: fail to create idle_thread.\n");
		return ;
	}

	// set idle 
	while(queue_length(queue) != 0) 
		uthread_yield();
}
