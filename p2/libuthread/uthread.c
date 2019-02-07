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
	uthread_t TID;
	uthread_t join_tid;
	bool is_joined;
	State state;
	uthread_ctx_t *context;
	void* arg;
	uthread_func_t func;
	int* stack;// = uthread_ctx_alloc_stack();
}thread;

int main_thread(uthread_func_t func, void *arg);

/* Look for the next available thread */
void uthread_yield(void)
{
	printf("%d LEN in yield ", queue_length(q));
	
	if(queue_length(q) > 0){
		printf("is it >0 ?\n");
		struct thread *curr;
		queue_dequeue(q,(void**)&curr);
//		//struct thread* prev_struct =  running_thread;
		//struct thread* curr_struct =  curr; //make this our running thread.

		uthread_ctx_switch(running_thread->context, curr->context);
		//running_thread->state = Ready;
                //queue_enqueue(q,(void*)running_thread);//add running thread to q!
                //((struct thread*)curr)->state = Running;
                //running_thread = (struct thread*)curr; //now this is running thread.
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
	
		struct thread *thread = malloc(sizeof(struct thread));
		void *top_of_stack = uthread_ctx_alloc_stack();
	
		
		thread->TID = num_threads;
		thread->func = func;
		thread->arg = arg;
		thread->is_joined = false;
		//thread->parent = running_thread;
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

void uthread_exit(int retval)
{
	running_thread->state = Zombie;

}

int uthread_join(uthread_t tid, int *retval)
{
	printf("In join");
	//running_thread....
/*	if(running_thread->TID == tid){
		return -1;
	}//if running thread is the parent...

	if(tid == 0){return -1;}


	struct Node *curr = q->front;
	while(curr != NULL){
		if(((struct thread*)curr->key)->TID == tid){
			if(((struct thread*)curr->key)->is_joined == false){
				((struct thread*)curr->key)->is_joined = true;
				running_thread->state = Blocked; //block the running thead.
				queue_enqueue(blocked_queue, running_thread);
				((struct thread*)curr->key)->join_tid = running_thread->TID;
				uthread_yield();
				printf("GOES IN IF\n");
			}
		}
		curr = curr->next;
		}
	*/

		struct thread *curr;
		printf("%d LEN", queue_length(q));
                queue_dequeue(q,(void**)&curr);
		uthread_ctx_switch(running_thread->context, curr->context);
//	while(queue_length(q)>0){
//		uthread_yield();
//		printf("after");

//	}
		return 0;
}

int main_thread(uthread_func_t func, void *arg)
{
	printf("reached main_thread\n");
	struct thread *thread = malloc(sizeof(struct thread));
	thread->TID = num_threads;
        thread->state = Running; //assign to read?
	thread->arg = arg;
	thread->is_joined = false;
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
	running_thread = thread;
	//queue_enqueue(q, thread); //add the struct to the queue
	return 0; //return TID
}

