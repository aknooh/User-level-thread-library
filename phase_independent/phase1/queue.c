#include <stdint.h>
#include <stdlib.h>

#include "queue.h"

typedef int bool;
#define true 1
#define false 0


struct node {
	node *prev, *next;
	void *data;
} typedef node;

struct queue {
	node *tail, *front;
	int size;
};

void *queue_top(queue_t queue)
{
	if (queue_empty(queue)) return NULL;
	else return queue->front->data;
}

bool queue_empty(queue_t queue)
{
	return queue_length(queue) == 0;
}

queue_t queue_create(void)
{
	queue_t q = malloc(sizeof(q));
	q->size = 0;
	q->tail = q->front = NULL;
}

int queue_destroy(queue_t queue)
{
	while(!queue_empty(queue)) {
		free(queue_top(queue));
		queue_dequeue(queue);
	}
	return 1;
}

int queue_enqueue(queue_t queue, void *data)
{
	// invalid data or queue
	if ( data == NULL 
	     || queue == NULL) return 0;

	// create new node, and assign data
	node *newNode = malloc(sizeof(node));
	newNode->data = data;

	// if empty: assign head and tail to single element
	if (queue_empty(queue)) {
		queue->tail = queue->front = newNode;
		newNode->next = newNode->prev = NULL;
	}

	// otherwise empty: assign to the back
	else {
		queue->tail->prev = newNode;
		newNode->prev = queue->tail;
		tail = newNode;
		tail->next = NULL;
	} 

	// success
	queue->size++;
	return 1;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue_empty(queue)) return 0;

	node *temp_rm;
	temp_rm = queue->head;
	queue->head = queue->head->next;
	
	if (queue->head) 
		queue->prev = NULL;
	// otherwise empty
	else queue->tail = NULL;

	free(temp_rm);
	queue->size--;
}

int queue_delete(queue_t queue, void *data)
{
	if (queue_empty(queue)) return 0;

	node *iter = queue->front;
	bool found = false;

	// queue as single element
	if (iter->next == NULL 
	    && iter->data == data)
	{
		queue->tail = queue->head = NULL;
		free(iter);
		return 1;
	}

	// iterate to find element
	while (iter != NULL) 
	{
		if (iter->data == data) {
			found = true;

			node *temp_rm = iter;
			iter->prev->next = temp_rm->next;
			node->next->prev = temp_rm->prev;

			free(found);
			break;
		}
		iter = iter->next;
	}

	found ? return 1 : return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}


int queue_length(queue_t queue)
{
	return queue->size;
}



