#all: libuthread.a

queue: test-queue.o queue.o
	gcc test-queue.o queue.o -o queue

test-queue.o: test-queue.c queue.h
	gcc -c -g test-queue.c

queue.o: queue.h queue.c
	gcc -c -g queue.c

clean:
	rm *.o queue
