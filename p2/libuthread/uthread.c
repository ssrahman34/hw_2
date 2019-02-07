#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.c"
#include "context.h"
#include "preempt.h"
#include "preempt.c"
#include "queue.h"
#include "uthread.h"
#include "queue.h"
#include "queue.c"

int num_threads = 0;
struct queue* q;
struct queue* blocked_queue;

struct thread* running_thread;

typedef enum{
        Blocked ,
        Running ,
        Ready ,
        Zombie 
}State;

typedef struct thread{
	//thread* parent = malloc(sizeof(thread));
	uthread_t TID;
	State state;
	uthread_ctx_t* context;
	void* arg;
	uthread_func_t func;
	int* stack;// = uthread_ctx_alloc_stack();
} thread;

int main_thread(uthread_func_t func, void *arg);

/* Look for the next available thread */
void uthread_yield(void)
{
	if(queue_length(q) > 0){
		struct Node *curr;
		queue_dequeue(q,&curr);
		struct thread* prev_struct =  running_thread;
		struct thread* curr_struct =  (struct thread*)curr->key; //make this our running thread.

		prev_struct->state = Blocked;
		queue_enqueue(blocked_queue,running_thread);//add it to bloecked		
		uthread_ctx_switch(prev_struct->context, curr_struct->context);
		curr_struct->state = Running;
		running_thread = curr_struct; //now this is running thread.
	}//if we have queue
	return;
}

uthread_t uthread_self(void)
{
	
}

int uthread_create(uthread_func_t func, void *arg)
{
	printf("Reached uthread_create\n");
	if(num_threads == 0){
		main_thread(func, arg);
	}
	else{
		struct thread *thread = malloc(sizeof(struct thread));
		void *top_of_stack = uthread_ctx_alloc_stack();
	
		
		thread->TID = num_threads;
		thread->func = func;
		thread->arg = arg;
		thread->state = Ready; //assign to read?
		thread->stack = top_of_stack; //assign stack
		thread->context = malloc(sizeof(uthread_ctx_t));
		int retVal = uthread_ctx_init(thread->context, top_of_stack, func, arg);
		if (retVal != 0){
			return -1;
                }	
		num_threads++; //increment thread number
		queue_enqueue(q, thread); //add the struct to the queue
		return thread->TID;
	}
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	while(1){
		struct Node *curr = q->front;
		while(curr != NULL){
			if(((struct thread*)curr->key)->state == Ready){
				uthread_yield();
			}
			curr = curr->next;
		}
		break;
	}
	return 0;
}

int main_thread(uthread_func_t func, void *arg)
{
	printf("reached main_thread\n");
	struct thread *thread = malloc(sizeof(struct thread));
	thread->TID = num_threads;
        thread->state = Running; //assign to read?
	thread->arg = arg;
	//thread->parent = NULL;
	thread->func = func;
	thread->context = malloc(sizeof(uthread_ctx_t));
	void *top_of_stack = uthread_ctx_alloc_stack();
        thread->stack = top_of_stack; //assign stack
	int retVal = uthread_ctx_init(thread->context, top_of_stack, func, arg);

	if (retVal == -1){
		return -1;
	}
	num_threads++; //increment thread number
	blocked_queue = queue_create();
	q = queue_create();
printf("Queue created\n");	
	queue_enqueue(q, thread); //add the struct to the queue
	printf("%d",q->front->key);
	return 0; //return TID
}

