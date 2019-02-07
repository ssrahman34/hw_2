#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

struct Node{
	void* key;
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
		curr = next;
	}//free the nodes in the quque
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(data == NULL || queue== NULL){
		return -1;
	}
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->key = data;
	new_node->next = NULL;
	//struct Node *temp = (struct Node*) malloc(sizeof(struct Node));
	//temp = data;//this might be a problem //might need to make key a *
	if (queue->rear == NULL){
		queue->front = queue->rear = new_node;		
		return 0;
	} 
	queue->rear->next = new_node;
	queue->rear= new_node;
    return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL || data == NULL || queue_length(queue) == 0 || queue->front == NULL){
		return -1;
	}
	
	*data = &(queue->front->key);
	queue->front = queue->front->next;
	if(queue->front == NULL){
		queue->rear=NULL;
	}
	//free (queue->front);
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL){
		return -1;
	}
	struct Node *curr = queue->front;
	while(curr->next != NULL){
		if(curr->next->key == data){
			struct Node *temp = curr->next;
			curr->next = curr->next->next;
			free(temp); //delete
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
		retVal = func(curr, arg); //the result of the function call???
		// Do we need to iterate through data? is is *data in func
		if(retVal == 1){
			if(data == NULL){
				return -1;
			}
			else{
			//	data = curr;//data received the data item where iteration stoped
				break;
			}
		}
		curr=curr->next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	}
	if(queue->front == NULL){
		return 0; //we have an empty queue
	}
	int length = 1;
	struct Node* curr = queue->front;
	while(curr != queue->rear){
		length++;
		curr = curr->next;
	}
	//length = count(queue->front);
	return length;
}

/*int count(struct Node* head){
 if (head == NULL) 
        return 0; 
  
    // count is 1 + count of remaining list 
    return 1 + count(head->next); 
}*/

