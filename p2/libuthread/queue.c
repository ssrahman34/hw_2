#include <stdint.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include "queue.h"

struct Node {
    void * key;
    struct Node * next; //pointer to next node
};

struct queue {
    struct Node * front, * rear;
    int length;
};

queue_t queue_create(void) {
    struct queue * queue = (struct queue * ) malloc(sizeof(struct queue));
    queue -> front = NULL;
    queue -> rear = NULL;
    queue->length = 0;
    return queue;
}

int queue_destroy(queue_t queue) {
    if (queue == NULL || queue_length(queue) != 0) {
        return -1;
    }
    struct Node * curr = queue -> front;
    struct Node * next;
    while (curr != NULL) {
        next = curr -> next;
        free(curr);
        curr = next;
    } //free the nodes in the quque
    free(queue);
    return 0;
}

int queue_enqueue(queue_t queue, void * data) {
    if (data == NULL || queue == NULL) {
        return -1;
    }
    struct Node * new_node = (struct Node * ) malloc(sizeof(struct Node));
    new_node -> key = data;
    new_node -> next = NULL;

    queue->length++;
    if (queue -> rear == NULL) {
        queue -> front = queue -> rear = new_node;
        return 0;
    }
    queue -> rear -> next = new_node;
    queue -> rear = new_node;
    return 0;

}

int queue_dequeue(queue_t queue, void ** data) {
    if (queue == NULL || data == NULL || queue_length(queue) == 0 || queue -> front == NULL) {
        return -1;
    }

    * data = & (queue -> front -> key);
    queue -> front = queue -> front -> next;
    if (queue -> front == NULL) {
        queue -> rear = NULL;
    }
    queue->length--;
    //free (queue->front);
    return 0;
}

int queue_delete(queue_t queue, void * data) {
    if (queue == NULL || data == NULL) {
        return -1;
    }
    struct Node * curr = queue -> front;
    while (curr -> next != NULL) {
        if (curr -> next -> key == data) {
            struct Node * temp = curr -> next;
            curr -> next = curr -> next -> next;
            queue->length--;
            free(temp); //delete
            return 0;
        }
        curr = curr -> next;
    }
    return -1; //data was not found in queue
}

int queue_iterate(queue_t queue, queue_func_t func, void * arg, void ** data) {
    if (queue == NULL || func == NULL) {
        return -1;
    }
    struct Node * curr = queue -> front;
    //void (*ptr)() = &func; //this is the function to call
    int retVal;
    while (curr != NULL) {
        retVal = func(curr, arg); //the result of the function call???
        // Do we need to iterate through data? is is *data in func
        if (retVal == 1) {
            if (data == NULL) {
                return -1;
            } else {
                break;
            }
        }
        curr = curr -> next;
    }
    return 0;
}

int queue_length(queue_t queue) {
    if (queue == NULL) {
        return -1;
    }
    if (queue -> front == NULL) {
        return 0; //we have an empty queue
    }
    
    return queue->length;
}
