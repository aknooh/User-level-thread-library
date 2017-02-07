#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>
#include "queue.h"

typedef int bool;
#define true 1
#define false 0


struct node {
	struct node *prev, *next;
	void *data;
} typedef node;

struct queue {
	node *back, *front;
	int size;
};

bool queue_empty(queue_t queue)
{
	return queue_length(queue) == 0;
}

void queue_pop(queue_t queue)
{
	if (queue_empty(queue)
		|| queue == NULL) return;

	node *temp_rm;
	temp_rm = queue->front;
	queue->front = queue->front->next;
	
	if (queue->front) 
		queue->front->prev = NULL;
	// otherwise empty
	else queue->back = NULL;

	queue->size--;
	free(temp_rm);
}

void *queue_front(queue_t queue)
{
	if (queue_empty(queue)
		|| queue == NULL) return NULL;
	else return queue->front->data;
}

queue_t queue_create(void)
{
	queue_t q = malloc(sizeof(struct queue));
	q->back = q->front = NULL;
	q->size = 0;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL) return -1;
	while(!queue_empty(queue)) {
		free(queue_front(queue));
		queue_pop(queue);
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	// invalid data or queue
	if ( data == NULL 
	     || queue == NULL) return -1;

	// create new node, and assign data
	node *newNode = malloc(sizeof(node));
	newNode->data = data;

	// if empty: assign front and back to single element
	if (queue_empty(queue)) {
		queue->back = queue->front = newNode;
		newNode->next = newNode->prev = NULL;
	}

	// otherwise empty: assign to the back
	else {
		queue->back->next = newNode;
		newNode->prev = queue->back;
		queue->back = newNode;
		newNode->next = NULL;
	} 

	// success
	queue->size++;
	return 0;
}


int queue_dequeue(queue_t queue, void **data)
{
	if (queue_empty(queue)
		|| queue == NULL) return -1;

	node *temp_rm;
	temp_rm = queue->front;
	queue->front = queue->front->next;
	
	if (queue->front) 
		queue->front->prev = NULL;
	// otherwise empty
	else queue->back = NULL;

	queue->size--;
	free(temp_rm);

	*data = queue->front->data;

	return 0;
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
		queue->back = queue->front = NULL;
		queue->size--;
		free(iter);
		return 0;
	}

	// iterate to find element
	while (iter != NULL) 
	{
		if (iter->data == data) {
			found = true;

			node *temp_rm = iter;
			iter->prev->next = temp_rm->next;
			iter->next->prev = temp_rm->prev;

			free(temp_rm);
			break;
		}
		iter = iter->next;
	}
	queue->size--;
	return found ? 0 : -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue == NULL 
	  || func == NULL) return -1;
	
	node *iter = queue->front;
	node *iter_cp = NULL;
	while (iter != NULL) 
	{
		// save position, so that if even if it deletes,
		// we would still be able to continue iterating
		// through the elements.
		iter_cp = iter->next;

		func(iter->data);
		iter = NULL;

		iter = iter_cp;
	}
	
	return 0;
}


int queue_length(queue_t queue)
{
	if (queue == NULL) return -1;
	return queue->size;
}

void queue_iterate_db(queue_t queue)
{
	if(queue == NULL) return;
	
	node *iter = queue->front;
	while (iter != NULL) 
	{
		printf("%d, ", *((int*)iter->data));
		iter = iter->next;
	}
	printf("\n");
}

