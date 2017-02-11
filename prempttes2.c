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


void thread2(void* arg)
{
	printf("thread2\n");
    getchar();
}

void thread1(void* arg)
{
	uthread_create(thread2, NULL);
    while(1)
    {
        printf("loop\n");
    }
	//uthread_yield();
	printf("thread1\n");
}

int main(void)
{
	uthread_start(thread1, NULL);
	return 0;
}
