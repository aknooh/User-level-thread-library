CFLAGS  = -g -Wall -Werror

all: libuthread.a

libuthread.a: queue.o context.o uthread.o semaphore.o preempt.o
	ar crs libuthread.a queue.o context.o uthread.o semaphore.o preempt.o

queue.o: queue.c queue.h uthread.h preempt.h
	gcc $(CFLAGS) -c queue.c

context.o: context.c context.h
	gcc $(CFLAGS) -c context.c

uthread.o: uthread.c uthread.h
	gcc $(CFLAGS) -c uthread.c

semaphore.o: semaphore.c semaphore.h 
	gcc $(CFLAGS) -c semaphore.c
	
preempt.o: preempt.c preempt.h
	gcc $(CFLAGS) -c preempt.c 

clean:
	rm -f *.o libuthread.a
