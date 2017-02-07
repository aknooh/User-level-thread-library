CFLAGS  = -g -Wall

all: libuthread.a

libuthread.a: queue.o context.o uthread.o
	ar crs libuthread.a queue.o context.o uthread.o

queue.o: queue.c queue.h
	gcc $(CFLAGS) -c queue.c

context.o: context.c context.h
	gcc $(CFLAGS) -c context.c

uthread.o: uthread.c uthread.h context.h queue.h
	gcc $(CFLAGS) -c uthread.c

clean:
	rm -f *.o libuthread.a
