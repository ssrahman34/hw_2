#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"



struct queue {
	/* TODO Phase 1 */
	int front;
	int rear;
	int size;
	int capacity;
	int *array;
};

queue_t queue_create(void)
{
	
	struct queue* queue = (struct queue*) malloc(sizeof(struct queue));
	queue->front = 0;
	queue->size = 0;	
	queue->capacity = 100;
	queue->rear = queue->capacity  -1;
	queue->array = (int*) malloc(queue->capacity  * sizeof(int));
	return queue;
}

int queue_destroy(queue_t queue)
{
	// delete(queue->array);
	// delete(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue->size == queue->capacity) 
		return -1; 
    if(queue->rear == queue->capacity -1 ){
		queue->rear = -1;
	} 
	queue->rear = queue->rear +1;
    // queue->array[queue->rear] = *data; //HOW DO WE FIX THIS?
    queue->size += 1; 
    return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
	return 0;
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	return 0;
}

