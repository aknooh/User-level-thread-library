/*
 * Simple hello world test with preempt.
 *
 * Tests will help confirm that the other tests work as well.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void hello(void* arg)
{
	while (1) {
		printf("Hello world!\n");
	}
	
}

int main(void)
{
	uthread_start(hello, NULL);
	return 0;
}
