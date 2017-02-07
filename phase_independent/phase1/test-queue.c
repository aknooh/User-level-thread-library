/*
 * Complete test for queue.h
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"


void test_all()
{	
	// init data
	int * data1 = (int*) malloc(sizeof(int));
	int * data2 = (int*) malloc(sizeof(int));
	int * data3 = (int*) malloc(sizeof(int));
	int * data4 = (int*) malloc(sizeof(int));
	int * data5 = (int*) malloc(sizeof(int));

	*data1 = 10;
	*data2 = 20;
	*data3 = 30;
	*data4 = 40;
	*data5 = 50;

	queue_t queue = queue_create();
	
	// enqueue
	queue_enqueue(queue, data1); queue_iterate_db(queue); // 10
	queue_enqueue(queue, data2); queue_iterate_db(queue); // 10, 20
	queue_enqueue(queue, data3); queue_iterate_db(queue); // ...
	queue_enqueue(queue, data4); queue_iterate_db(queue);
	queue_enqueue(queue, data5); queue_iterate_db(queue);

	// delete
	assert(queue_length(queue) == 5);
	queue_delete(queue,data2);
	assert(queue_length(queue) == 4);

	// dequeue
	queue_pop(queue); 
	queue_iterate_db(queue);
	assert(queue_length(queue) == 3);

	// testing edge cases
	assert(queue_destroy(NULL) == -1);
	assert(queue_enqueue(NULL, NULL) == -1);

	// destroy queue
	queue_destroy(queue);


	// testing edge cases
	int * data11 = (int*) malloc(sizeof(int));
	int * data12 = (int*) malloc(sizeof(int));
	int * data13 = (int*) malloc(sizeof(int));
	*data11 = 11;
	*data12 = 12;
	*data13 = 13;
	queue_t queue2 = queue_create();
	
	queue_enqueue(queue, data11); queue_iterate_db(queue); // 11
	queue_pop(queue); 			  queue_iterate_db(queue); // (empty)
	assert(queue_length(queue) == 0);
	assert(queue_delete(queue,data11) == 0); 			   // empty element not found

	queue_enqueue(queue, data12); queue_iterate_db(queue); // 12
	assert(queue_length(queue) == 1);
	assert(queue_delete(queue,data13) == -1); 			   // element does not exist
}

int main(void)
{
	test_all();
	return 0;
}
