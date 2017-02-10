CFLAGS  = -g -Wall -Werror

all: libuthread.a

libuthread.a: queue.o context.o uthread.o semaphore.o
	ar crs libuthread.a queue.o context.o uthread.o semaphore.o

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c queue.c

context.o: context.c context.h
	gcc $(CFLAGS) -c context.c

uthread.o: uthread.c uthread.h context.h queue.h
	gcc $(CFLAGS) -c uthread.c

semaphore.o: semaphore.c preempt.h queue.h uthread.h
	gcc $(CFLAGS) -c semaphore.c

clean:
	rm -f *.o libuthread.a
