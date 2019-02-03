#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct Node{
	int key;
	struct Node *next; //pointer to next node
};

struct queue {
	struct Node *front, *rear;
};

queue_t queue_create(void)
{
	
	struct queue* queue = (struct queue*) malloc(sizeof(struct queue));
	queue->front = NULL;
	queue->rear = NULL;  
	return queue;
}

int queue_destroy(queue_t queue)
{
	if(queue == NULL || queue_length(queue) != 0){
		return -1;
	}
	struct Node* curr= queue->front;
	struct Node* next;
	while(curr != NULL){
		next = curr->next;
		free(curr);
		curr = curr->next;
	}//free the nodes in the quque
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue->rear == NULL) 
		return -1; 
    struct Node *temp = (struct Node*) malloc(sizeof(struct Node));
	temp = data; //assign data node to temp....
	queue->rear->next = temp;
	queue->rear = temp;
    return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || data == NULL || queue_length(queue) == 0 || queue->front == NULL){
		return -1;
	}
	
	queue->front = queue->front->next;
	if(queue->front == NULL){
		queue->rear=NULL;
	}
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL){
		return -1;
	}
	struct Node *curr = queue->front;
	while(curr->next != NULL){
		if(curr->next == data){
			struct Node *temp = curr->next;
			curr->next = curr->next->next;
			delete(temp);
			return 0;
		}
		curr = curr->next;
	}
	return -1; //data was not found in queue
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if(queue==NULL || func == NULL){
		return -1;
	}
	struct Node *curr = queue->front;
	//void (*ptr)() = &func; //this is the function to call
	int retVal;
	while(curr!= NULL){
		data = func(curr, arg); //the result of the function call???
		// Do we need to iterate through data? is is *data in func
		curr=curr->next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	}
	int len = 0;
	struct Node* curr = queue->front;
	while(curr != NULL){
		len++;
		curr = curr->next;
	}
	return len;
}

