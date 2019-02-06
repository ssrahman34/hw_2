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

typedef enum{
        Blocked = 0,
        Running = 1,
        Ready = 2,
        Zombie = 3
}State;

struct uthread_ctx_t{
	uthread_t TID;
	State state;
	uthread_ctx_t registers;
	int* stack;// = uthread_ctx_alloc_stack();
};

int main_thread(uthread_func_t func, void *arg);
void uthread_yield(void)
{//look for next available thread!
	struct Node *curr = q->front;
	struct Node *next;
	while(curr->next != NULL){
		struct uthread_ctx_t* prev_struct =  (struct uthread_ctx_t*)curr->key;	
		struct uthread_ctx_t* curr_struct =  (struct uthread_ctx_t*)curr->next->key;
		if(curr_struct->state == Ready){
			//prev_struct is what we switch from
			//curr_struct will be the next threat we switch to 
			uthread_ctx_switch((uthread_ctx_t*)prev_struct, (uthread_ctx_t*)curr_struct);
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
	printf("Reached uthread_create\n");
	if(num_threads == 0){
		main_thread(func, arg);
	}
	else{
		uthread_ctx_t *thread;
		void *top_of_stack = uthread_ctx_alloc_stack();
		int retVal = uthread_ctx_init(thread, top_of_stack, func, arg);
	
		if (retVal== -1){
			return -1;
		}
		num_threads++; //increment thread number
		struct uthread_ctx_t* ts;
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
	printf("reached main_thread\n");
	uthread_ctx_t *thread;
	void *top_of_stack = uthread_ctx_alloc_stack();
//printf("%ls\n",top_of_stack);
	int retVal = uthread_ctx_init(thread, top_of_stack, func, arg);
	printf(" %d retVAL", retVal);

       	printf("reached after uthread init\n");	
	printf(" %d NOW", retVal);
	if (retVal == -1){
		return -1;
	}
	printf("Did it return -1?");
	num_threads++; //increment thread number
	struct uthread_ctx_t* ts;
	ts->TID = num_threads;
	ts->state = 1; //assign to read?
	ts->stack = top_of_stack; //assign stack
	q = queue_create();
printf("Queue created\n");	
	queue_enqueue(q, ts); //add the struct to the queue
	return 0; //return TID
}

