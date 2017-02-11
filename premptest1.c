/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads with no yields.
 * All threads are in a while loop, so none of the threads should terminate
 * but this test should demonstrate context switch between threads dispite 
 * no explicit yields.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void thread4(void* arg)
{
    getchar();
    while(1)
        printf("thread4\n");
}

void thread3(void* arg)
{
    getchar();
    while(1)
        printf("thread3\n");
}

void thread2(void* arg)
{
    getchar();
    while(1)
        printf("thread2\n");
}

void thread1(void* arg)
{
	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);
	uthread_create(thread4, NULL);
    
	getchar();
    while(1)
        printf("thread1\n");
    
}

int main(void)
{
	uthread_start(thread1, NULL);
	return 0;
}
