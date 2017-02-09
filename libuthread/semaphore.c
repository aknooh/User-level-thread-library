#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define _UTHREAD_PRIVATE
#include "preempt.h"
#include "queue.h"
#include "semaphore.h"
#include "uthread.h"

struct semaphore {
	int sem_count;
	struct *queue wait_list;
};

sem_t sem_create(size_t count)
{
	struct semaphore *new_sem = malloc(sizeof(struct semaphore));
	int sem_count = count;
	wait_list = queue_create();

	if (new_sum == NULL) return NULL;
	else return new_sum;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL) return -1;
	if (queue_destroy(sem->wait_list) == -1) return -1;
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if (sem == NULL) return -1;

	// requesting to get in when resource is full 
	// gets placed to the back of line 
	else if (sem->sem_count == 0) {
		struct uthread_tcb *cur_tcb = uthread_current();
		queue_enqueue(sem->wait_list, cur_tcb);
		uthread_block();
	} 

	// otherwise, another resource is consumed
	else --sem->sem_count;
	
	return 0;
}

int sem_up(sem_t sem)
{
	if (sem == NULL) return -1;

	// if there is a queue waitlist, that implies
	// that the source is already full, and
	// the person must get back into line
	if (queue_length(sem->wait_list) > 0) {
		struct uthread_tcb *front;
		if (queue_dequeue(sem->wait_list, (void**) &front) == -1) {
			fprintf(stderr, "Failure to dequeue from queue.\n");
			return -1;
		}
		uthread_unblock(front);
	}
	else ++sem->sem_count;
}

