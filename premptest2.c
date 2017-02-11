/*
 * Thread creation, yielding, and termination
 *
 * A few threads will be in a while loop so, termination for some will 
 * not end. However, other threads will eventually terminate so its 
 * context should eventually end, and the final result should iterate
 * through two different infinite loops.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>


void thread5(void* arg)
{
    for (int i = 0; i < 99999; i++) {
        printf("thread5\n");
    }   
}

void thread4(void* arg)
{
    while(1)
        printf("thread4\n");
}

void thread3(void* arg)
{
    for (int i = 0; i < 99999; i++) {
        printf("thread3\n");
    }        
}

void thread2(void* arg)
{
    while(1)
        printf("thread2\n");
}

void thread1(void* arg)
{
	uthread_create(thread2, NULL);
	uthread_create(thread3, NULL);
	uthread_create(thread4, NULL);
    uthread_create(thread5, NULL);

    while(1)
        printf("thread1\n");
    
}

int main(void)
{
	uthread_start(thread1, NULL);
	return 0;
}
