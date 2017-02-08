/*
 * Exhaustive test for queue.h
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

// global access
queue_t queue;

// test queue iterate
void test(void *data)
{
	int *my_int = (int*)data;
	if(*my_int == 35)
		queue_delete(queue, data);
}

void test_all()
{	
	// init data
	int * data1 = (int*) malloc(sizeof(int));
	int * data2 = (int*) malloc(sizeof(int));
	int * data3 = (int*) malloc(sizeof(int));
	int * data4 = (int*) malloc(sizeof(int));
	int * data5 = (int*) malloc(sizeof(int));
	int * data6 = (int*) malloc(sizeof(int));

	*data1 = 10;
	*data2 = 20;
	*data3 = 35;
	*data4 = 40;
	*data5 = 50;
	*data6 = 60;

	queue = queue_create();
	
	// enqueue
	queue_enqueue(queue, data1); queue_iterate_db(queue); // 10
	queue_enqueue(queue, data2); queue_iterate_db(queue); // 10, 20
	queue_enqueue(queue, data3); queue_iterate_db(queue); // ...
	queue_enqueue(queue, data4); queue_iterate_db(queue);
	queue_enqueue(queue, data5); queue_iterate_db(queue);
	queue_enqueue(queue, data6); queue_iterate_db(queue);

	// length
	assert(queue_length(queue) == 6);

	// queue iterate
	queue_iterate(queue, test); queue_iterate_db(queue);
	assert(queue_length(queue) == 5);

	// delete
	assert(queue_length(queue) == 5);
	queue_delete(queue,data2);
	assert(queue_length(queue) == 4);

	// dequeue
	assert(queue_pop(queue) == 0);
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
	
	queue_enqueue(queue2, data11); queue_iterate_db(queue2); // 11
	assert(queue_pop(queue2) == 0);	   
	queue_iterate_db(queue2); 								 // (empty)
	
	assert(queue_length(queue2) == 0);
	assert(queue_delete(queue2, data12) == 0); 			     // empty element not found

	queue_enqueue(queue2, data12); queue_iterate_db(queue2); // 12
	assert(queue_length(queue2) == 1);
	assert(queue_delete(queue2, data13) == -1); 			 // element does not exist
	queue_enqueue(queue2, data13); queue_iterate_db(queue2); // 12, 13
	
	assert(queue_pop(queue2) == 0);	
	assert(queue_length(queue2) == 0);
	assert(queue_pop(queue2) == -1);	

	queue_destroy(queue2);
}

int main(void)
{
	test_all();
	return 0;
}
