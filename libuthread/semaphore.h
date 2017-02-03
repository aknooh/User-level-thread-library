#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <stdint.h>

/*
 * sem_t - Semaphore type
 *
 * A semaphore is a way to control access to a common resource by multiple
 * threads. Such resource has an internal count, meaning that it can only be
 * shared a certain number of times. When a thread successfully takes the
 * resource, the count is decreased. When the resource is not available,
 * following threads are blocked until the resource becomes available again.
 */
typedef struct semaphore *sem_t;

/*
 * sem_create - Allocate and initialize a semaphore
 * @count: Semaphore count
 *
 * Return: Pointer to initialized semaphore, or NULL in case of failure
 */
sem_t sem_create(size_t count);

/*
 * sem_destroy - Deallocate a semaphore
 * @sem: Semaphore to deallocate
 *
 * Return: 0 is @sem was successfully destroyed, or -1 in case of failure
 */
int sem_destroy(sem_t sem);

/*
 * sem_down - Take a semaphore
 * @sem: Semaphore to take
 *
 * Return: 0 in case of success, or -1 in case of failure
 *
 * Note that taking a unavailable semaphore will cause the caller thread to be
 * blocked until the semaphore becomes available.
 */
int sem_down(sem_t sem);

/*
 * sem_up - Release a semaphore
 * @sem: Semaphore to release
 *
 * Return: 0 in case of success, or -1 in case of failure
 */
int sem_up(sem_t sem);

#endif /* _SEMAPHORE_H */
