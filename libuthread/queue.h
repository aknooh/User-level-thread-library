#ifndef _QUEUE_H
#define _QUEUE_H

/*
 * queue_t - Queue type
 *
 * A queue is a FIFO data structure. Items are enqueued one after the other.
 * When dequeueing, the queue must returned the oldest enqueued item first and
 * so on.
 *
 * Apart from delete and iterate operations, all operations should be O(1).
 */
typedef struct queue* queue_t;

/*
 * queue_create - Allocate an empty queue
 *
 * Return: Pointer to empty queue, or NULL in case of failure
 */
queue_t queue_create(void);

/*
 * queue_destroy - Deallocate a queue
 * @queue: Queue to deallocate
 *
 * Return: 0 if @queue was successfully destroyed, or -1 in case of failure
 */
int queue_destroy(queue_t queue);

/*
 * queue_enqueue - Enqueue data
 * @queue: Queue in which to enqueue data
 * @data: Data to enqueue
 *
 * Return: 0 if @data was successfully enqueued in @queue, or -1 in case of
 * failure
 */
int queue_enqueue(queue_t queue, void *data);

/*
 * queue_dequeue - Dequeue data
 * @queue: Queue in which to dequeue data
 * @data: Address of data pointer where data is received
 *
 * Return: 0 if @data was set with oldest item in @queue, or -1 in case of
 * failure
 */
int queue_dequeue(queue_t queue, void **data);

/*
 * queue_delete - Delete data
 * @queue: Queue in which to delete data
 * @data: Data to delete
 *
 * Return: 0 if @data was found and deleted from @queue, or -1 in case of
 * failure
 */
int queue_delete(queue_t queue, void *data);

/*
 * queue_func_t - Queue callback function type
 * @data: Data item
 */
typedef void (*queue_func_t)(void *data);

/*
 * queue_iterate - Iterate on all items of a queue
 * @queue: Queue to iterate on
 * @func: Function to call on each queue item
 *
 * Return: 0 if @queue was iterated successfully, or -1 in case of failure
 *
 * Note that this function should be resistant to data items being deleted
 * as part of the iteration (ie in @func).
 */
int queue_iterate(queue_t queue, queue_func_t func);

/*
 * queue_length - Queue length
 * @queue: Queue to get the length of
 *
 * Return: Length of @queue, or -1 in case of failure
 */
int queue_length(queue_t queue);


/* DEBUGGING FUNCTIONS COMMENT OUT LATER
void queue_iterate_db(queue_t queue);
int queue_pop(queue_t queue);
*/
#endif /* _QUEUE_H */
