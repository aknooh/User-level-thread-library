/*
 * Complete test for queue.h
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void test_enqueue()
{

}

void test_dequeue()
{
}

void test_enqueue_and_dequeue()
{

}

void test_edge_cases()
{
	assert(queue_destroy(NULL) == -1);
	assert(queue_enqueue(NULL, NULL) == -1);
}

int main(void)
{

	return 0;
}
