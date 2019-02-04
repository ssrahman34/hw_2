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

int num_threads = 0;

typedef struct thread_struct{
	uthread_t TID;
	State state;
	uthread_ctx_t registers;
	int *stack = uthread_ctx_alloc_stack();
}thread_struct; 

typedef enum{
	Blocked = 0,
	Running = 1,
	Ready = 2,
	Zombie = 3
}State;


void uthread_yield(void)
{
	/* TODO Phase 2 */
}

uthread_t uthread_self(void)
{
	
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(num_threads == 0){
		main_thread();
	}
	else{
		ucontext_t *thread;
		void *top_of_stack = uthread_ctx_alloc_stack();
		int retVal = uthread_ctx_init(thread, top_of_stack, func, arg);
	
		if (retVal== -1){
			return -1;
		}
		num_threads++; //increment thread number
		thread_struct ts;
		ts.TID = num_threads;
		ts.state = 2; //assign to read?
		ts.stack = top_of_stack; //assign stack

		return uthread_self();
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
	thread_struct ts;
	ts.TID = num_threads;
	ts.state = 2; //assign to read?
	ts.stack = top_of_stack; //assign stack

	return uthread_self(); //return TID
}

