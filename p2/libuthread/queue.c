#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"



struct queue {
	int front;
	int rear;
	int size;
	int capacity;
	int **array; //a list of pointers...
};

queue_t queue_create(void)
{
	
	struct queue* queue = (struct queue*) malloc(sizeof(struct queue));
	queue->front = 0;
	queue->size = 0;	
	queue->capacity = 100;
	queue->rear = queue->capacity  -1;
	queue->array = (int**) malloc(queue->capacity  * sizeof(void*));
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
    queue->array[queue->rear] = data; //data will be a pointer to the address
    queue->size += 1; 
    return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL){
		return -1;
	}
	for(int i = 0; i < queue->size; i++){
		if (queue->array[i] == data){
			queue->size = queue->size -1;
			for(int j = 0; j < queue->size;j++){
				queue->array[i]	= queue->array[i+1];
				queue->rear = queue->rear -1;
			}
			return 0;
		}//we found data
	}
	return -1;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	// for(int i = 0; i < queue->size; i++){
	// 	//queue[i]
	// }
	return 0;
}

int queue_length(queue_t queue)
{
	return 0;
}

