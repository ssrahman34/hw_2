#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"
#include "queue.h"
#include "queue.c"

int num_threads = 0;
struct queue* q;

typedef enum{ Blocked = 0, Running = 1, Ready = 2, Zombie = 3} State;

typedef struct thread_struct{
	uthread_t TID;
	State state;
	uthread_ctx_t registers;
	int *stack;
}thread_struct; 


void uthread_yield(void)
{//look for next available thread!
	struct Node *curr = q->front;
	struct Node *next;
	while(curr->next != NULL){
		struct thread_struct* prev_struct =  (struct thread_struct*)curr->key;	
		struct thread_struct* curr_struct =  (struct thread_struct*)curr->next->key;
		if(curr_struct->state == Ready){
			//prev_struct is what we switch from
			//curr_struct will be the next threat we switch to 
			uthread_ctx_switch(prev_struct, curr_struct);
			curr_struct->state = Running;
			prev_struct->state = Blocked;
		}
		curr=curr->next;
	}//while loops

	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(num_threads == 0){
		main_thread(func, arg);
	}
	else{
		ucontext_t *thread;
		void *top_of_stack = uthread_ctx_alloc_stack();
		int retVal = uthread_ctx_init(thread, top_of_stack, func, arg);
	
		if (retVal== -1){
			return -1;
		}
		num_threads++; //increment thread number
		thread_struct* ts;
		ts->TID = num_threads;
		ts->state = 2; //assign to read?
		ts->stack = top_of_stack; //assign stack
		
		queue_enqueue(q, ts); //add the struct to the queue
		return ts->TID;
	}
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
}

int main_thread(uthread_func_t func, void *arg)
{
	ucontext_t *thread;
	void *top_of_stack = uthread_ctx_alloc_stack();
	int retVal = uthread_ctx_init(thread, top_of_stack, func, arg);
	
	if (retVal== -1){
		return -1;
	}
	num_threads++; //increment thread number
	thread_struct* ts;
	ts->TID = num_threads;
	ts->state = 1; //assign to read?
	ts->stack = top_of_stack; //assign stack
	q = queue_create();
	
	queue_enqueue(q, ts); //add the struct to the queue
	return 0; //return TID
}

