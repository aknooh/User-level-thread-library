/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 * thread1
 * thread2
 * thread3
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
