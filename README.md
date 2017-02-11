Project #2 - User-level thread library

# Introduction

This library will provide a complete interface for applications to create and run independent threads concurrently.

# Table of Contents

* ** Phase 1: Queue API ** 
  * Functionality
* ** Phase 2: thread API ** 
  * Functinality


# Phase 1: queue API

The queue is implemented via doubly-linked list. We encapsulate each element in the queue as a node that contained a void pointer - `void* data` - that effectively becomes the thread TCB. We chose this data structure because it allows us to expand dynamically on a per thread basis. Our queue supports `O(1)` operations for enqueues and dequeues along with iterating and finding any element in the list for needed operation of `queue_iterate()` and `queue_delete()`

## Functionality 

### queue_pop

This function removes or "pops" and element from the front of the queue.
Forwards the front pointer of the queue and adjusts pointers of the front and back if necessary. After this, the front element is read. We included this function for debugging purposes as well as to integrate it with `queue_destroy` which simplified our implementation and readability. 

### queue_empty

Returns `true` if the size of the queue is 0.

### queue_front

Returns the element in the front of the queue, otherwise returns `NULL`

### queue_create

Allocates memory for a queue and initializes struct property such as the front, back and size of queue.

### queue_destroy

Performs a procedure that continuosly empties the queue until the queue is empty. We `free` the elements along this process. We integrate `queue_pop` in the process which adjusts the front pointer.

### queue_enqueue

This function allocates a new node and initializes the data for it that is past into the function. There are two cases to keep in mind: if the queue is empty, the front and back have to point to the new node and the next and previous point to null, otherwise we fit in the new node in the back of the queue and adjust pointers so that it is properly linked (double linked). Finally, the size is incremented.

### queue_dequeue

This functions removes an element from the queue. Here's how we implement this function.First we check to make sure the queue is not empty. We dereference the double voided data that is passed in to the function to equal to the dequeue element. This is returned back to the calling function that is utilized later. This function behaves just like `queue_pop`, but without the data that is passed.

### queue_delete

Iterates through the double linked list to the passed data and if that data is found, the size is decremented and we return 0 to denote a successful delete.

### queue_iterate 

This iterates through the queue and applies the function pointer to each element in the queue. We also ensure that if the function pointer deletes the data member it still continues to iterate to the end of the queue. *As of now, we have not found any siginifcant use of this function.* After completing the project,we found that our implementation did not need this function,but we still defined its functionality and also tested it thoroughly. 

### queue_length

Returns the length of the queue or returns `NULL` if the queue itself is invalid.

### queue_iterate_db

This is a helper function that was neccessary in our own exhaustive testing of the Queue API. We made sure it is discluded from the queue header file.


# Phase 2: Thread API

This is the main user thread library that manages the scheduling of threads of any context execution. Along with scheduling, it handles the starting of an `idle` thread which acts as the entry point of a process. 


## Functionality

### uthread_start

This function starts off by initializing a single queue that acts as a container for all the threads within a process. We maintain a strut called `uthread_tcb` (which is the Thread Control Block) that contains a pointer to the context of the thread, its own private stack, state of execution and an integer identifier. We included these four attributes to the TCB because it allowed for an intuitive interface of an individual thread; for example, the state of thread is an `enum` typedef that contains the states `RUNNING`, `READY`, `BLOCKED`, `TERMINATED`. The idle thread is identified by the integer 0, and every preceeding thread follows an n+1 id that starts from the idle thread.

We also maintain a global pointer to the current thread in order to easily interface through each function in the Thread API. 

The idle thread bootstraps the initial thread and continues to yield until there are no more threads in the queue. This is handled by using a while loop which checks if the queue is empty and continuosly calls our `yield` function.


### uthread_create

This function handles the creation of threads by enqueueing each new thread.
We begin by allocatin memory and initizlizing the proper atrributes (state, stack, etc) for a thread and its context. Finally the thread gets enqueued. Every thread should have its own context so it can resume if and when it gets yielded.

### uthread_yield

This function yields the currently running thread to the next `READY` thread.
We obtain the currently running thread. If its previous state was `RUNNING` we set it to `READY` since it hasn't finished its execution and dequeue the front of our queue. This return a pointer to the front of the queue; we ensure that every thread in the queue is in `READY` state before it is resumed to `RUNNING` state. If the thread hasn't finished its execution, then its re-enqueued. The most critical part is when we context switch, which changes context to the next thread in the queue. Finally we check if the thread is in a `TERMINATED` state in which case we free its TCB.

### uthread_exit

When a thread exits, its state is set to `TERMINATED` and we free the context and stack, then yield to the next thread.



# Phase 3: semaphore API

We create a semaphore struct with a count and a queue. The count represents the number of threads able to share a common resource at the same time and the queue handles blocked threads that are in the wait list for the next resource.

## Functionality

### sem_create

This constructs a new semaphore by initializing a new queue and passed in count.

### sem_destroy

All elements in the queue are delisted (using the `queue_destroy` function) and the memory for the semaphore is freed.

### uthread_block

The state of the current thread is changed to `BLOCKED` and yields to the next thread.

### uthread_unblock 

We `assert` that the thread hasn't been previosly in the `BLOCKED` state, we changed its state to `READY` and enqueue it to the "ready" queue in `thread.c`

### sem_up

A queue with a semaphore waitlist that has at least one or more threads in it implies that the when "freeing" a resource the front of the queue should be unblocked. Otherwise, a resource is freed by incrementing the count in the semaphore.

### sem_down 

When requesting to obtain resource we check to make sure that there is actually a resource available and if so, the resource is "consumed". Otherwise, its enlisted to the semaphore queue as a blocked thread.


# Phase 4: Preemption

In the preemption phase of our project, we implement the necessary functions to force a thread into yeilding after having been running for some amount of time. Each thread is equally assigned a fixed time frame for execution, so that no thread in particular hogs CPU time. In order to accomplish this, we use syscalls from the `Signal.h` library to perform the needed operations. We also use timer interrupts to keep track of the running time for each thread and interrupt the processor once a thread has exceeded its max allowed time.

## Functionality

### preempt_save

This function maintains the masking of the current thread. This means it saves the signals to be blocked in the provided pointer of type `sigset_t`. After that we disable preemtion explained below. After completing of all the phases, we did not use this function, because our implementation does not require to save the current mask signal, because we only care about one signal.

### preempt_restore

This function restores the given mask signals by copying the mask contents of its pointer argument to the currently running mask signals object. 

### preempt_enable

This functions enables the `SIGVTALRM` signal for the currently running mask. The `SIGVTALRM` is responsible for enabling the timer interrupt signal. We performe this task creating an empty `sigset_t` variable, and adding the `SIGVTALRM` signal to the variable. We then call the syscall `sigprocmask` with the `SIG_UNBLOCK` parameter to enable the `SIGVTALRM` signal, which enables the timer for the interrupt.

### preempt_disable

Disable blocks the `SIGVTALRM` which stops the timer. It does so by creating a new `sigset_t` variable and adding the signal to it. When we call `sigprocmask` with the the `SIG_BLOCK` parameter, it releases that signal from the mask. 

### preempt_disabled

This function checks to see if the signal responsible for the timer alarm `SIGVTALRM` is masked on or off, and returns true or false respectively. Again, as we did in most other preemption functions, we use `sigsetops` syscalls to check for the status of the signal.

### timer_handler

This is the function handler for the timer interrupt. This function gets called whenever the down counter reaches zero. This function is responsible for forcing currently running thread to yield. We call the function `uthread_yield` to force the currently running thread to yield to the next thread.

### preempt_start

This function was provided to us by professor Joel Porquet. Basically, this functions sets up the timer interrupt. In setting up the timer interrupt, a handler function is registered to the timer interrupt. The counter is also initialized to a specific time and is set to run periodically over and over again. We did not modify this function. 


## Design Choices

There were a few section in the code that we considered to be critical and required the usage of the `preemption` functions. 

* Malloc:
	* We recognized that malloc is not an atomic function and was crucial that there are no interrupts while it allocates new space.
* Queue: 
	* Queue operations are not atomic, so it was crucial that we disabled the timer since pointers were being shifted around. If the timer had been enabled, an interrupt would cause a mess while shifting pointers.
* Context Switching:
	* Consider this example: If you diable the timer before a context switch and enter a new thread, this new thread could potentially run an infinite loop and never exit (since the timer is disabled). For this reason, our program ensures that `preempt_enable` is turned on before context switching.



## References
 * https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm
 * http://stackoverflow.com/questions/25261/set-and-oldset-in-sigprocmask 
 * https://support.sas.com/documentation/onlinedoc/sasc/doc700/html/lr1/zlocking.htm
 * http://man7.org/linux/man-pages/man2/sigprocmask.2.html
 * http://pubs.opengroup.org/onlinepubs/009695399/functions/sigaction.html
 * http://www.cs.cornell.edu/courses/cs4410/2014fa/CS4411/projects/project1/project1.pdf

