#ifndef _UTHREAD_H
#define _UTHREAD_H

/*
 * Public uthread API
 *
 * To be used by applications
 */

/*
 * uthread_func_t - Thread function type
 * @arg: Argument to be passed to the thread
 */
typedef void (*uthread_func_t)(void *arg);

/*
 * uthread_start - Start the thread system
 * @start: Function of the first thread to start
 * @arg: Argument to be passed to the first thread
 *
 * This function should only be called by the main() function of the application
 * and never returns.
 *
 * This function starts the thread system and becomes the "idle" thread. It
 * exits from the application when all the threads have finished and the thread
 * system is done running.
 */
void uthread_start(uthread_func_t start, void *arg);

/*
 * uthread_create - Create a new thread
 * @func: Function to be executed by the thread
 * @arg: Argument to be passed to the thread
 *
 * Return: 0 in case of success, or -1 in case of failure
 */
int uthread_create(uthread_func_t func, void *arg);

/*
 * uthread_exit - Exit from currently running thread
 *
 * This function is to be called from the currently active and running thread in
 * order to finish its execution.
 */
void uthread_exit(void);

/*
 * uthread_yield - Yield execution
 *
 * This function is to be called from the currently active and running thread in
 * order to yield for other threads to execute.
 */
void uthread_yield(void);

#ifdef _UTHREAD_PRIVATE

/*
 * Private uthread API
 *
 * Only accessible internally by the uthread library
 */

/*
 * uthread_tcb - Internal representation of threads called TCB (Thread Control
 * Block)
 */
struct uthread_tcb;

/*
 * uthread_current - Get currently running thread
 *
 * Return: Pointer to current thread's TCB
 */
struct uthread_tcb *uthread_current(void);

/*
 * uthread_block - Block currently running thread
 */
void uthread_block(void);

/*
 * uthread_unblock - Unblock thread
 * @uthread: TCB of thread to unblock
 */
void uthread_unblock(struct uthread_tcb *uthread);

#endif /* UTHREAD_PRIVATE */

#endif /* _THREAD_H */
