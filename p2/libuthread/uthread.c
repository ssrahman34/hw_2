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
struct queue* zombie_queue;

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
	int retVal; //so we can use in jion
	uthread_ctx_t *context;
	void* arg;
	uthread_func_t func;
	int* stack;// = uthread_ctx_alloc_stack();
}thread;

int main_thread(uthread_func_t func, void *arg);

/* Look for the next available thread */
void uthread_yield(void)
{
//	printf("in yield");
	//printf("running thread TID = %d\n", running_thread->TID);
	if(queue_length(q) > 0){
		struct thread *running = running_thread;
		//printf("%s", temp->TID);
		struct Node *top = q->front;
		struct thread* temp = (struct thread*)top->key;
//		printf("%ld\n", temp);
		/*if(temp->TID == 0){
			printf("NULL");
		}else{

			printf("front of queue TID is = %d\n", temp->TID);
		}*/
		struct thread *curr = NULL;//= malloc(sizeof(thread));
                //prempt disable?

		queue_dequeue(q,(void **)&curr);
//		printf("%ld\n", curr);
//		printf("curr  DEQUEUED tid : %d\n", curr->TID);
		if(temp->state == Ready){

                	if(running->state == Running){

			running->state = Ready;
                	queue_enqueue(q, (void*)running);
			}
		temp->state = Running;
		running_thread = temp;
		
		uthread_ctx_switch(running->context, temp->context);
		}
	}
	return;
}

uthread_t uthread_self(void)
{
	return running_thread->TID;	
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(num_threads == 0){
		main_thread(func, arg);
	}
	
		struct thread *thr = malloc(sizeof(struct thread));
		void *top_of_stack = uthread_ctx_alloc_stack();
	
//	printf("Reached uthread_create\n");
		
		thr->TID = num_threads;
		thr->func = func;
		thr->arg = arg;
		thr->is_joined = false;
		//thread->parent = running_thread;
		thr->state = Ready; //assign to read?
		thr->stack = top_of_stack; //assign stack
		thr->context = malloc(sizeof(uthread_ctx_t));
		int retVal = uthread_ctx_init(thr->context, top_of_stack, func, arg);
		if (retVal != 0){
			return -1;
                }	
		num_threads++; //increment thread number
		queue_enqueue(q, (void*)thr); //add the struct to the queue
		//struct Node *top = q->front;
		//struct thread *temp = top->key;
		//printf("temp TID %d", temp->TID);
		return thr->TID;
	
}

void uthread_exit(int retval)
{
//	printf("in exit");
	running_thread->state = Zombie;
	running_thread->retVal = retval;
	struct thread* running = running_thread;
	uthread_t TID_to_unblock = running_thread->join_tid;
	queue_enqueue(zombie_queue, running);

	struct Node *curr_blocked = blocked_queue->front;

	while(curr_blocked != NULL){
		struct thread* temp_blocked = curr_blocked->key;
		if(temp_blocked->TID == TID_to_unblock){
			temp_blocked->state = Ready;
			queue_delete(blocked_queue, temp_blocked); //make it unblocked
			queue_enqueue(q, temp_blocked);
			break;
		}//we found which one to unblocn!
		curr_blocked = curr_blocked->next;
	}

	struct Node *top = q->front;
        struct thread* temp = (struct thread*)top->key;
	if(queue_length(q) > 0){
		struct thread *curr;//= malloc(sizeof(thread));
        	queue_dequeue(q,(void **)&curr);
		printf("exit and switch:");
		uthread_ctx_switch(running->context, temp->context); //what if there is nothing left in queue?
	}
	//UPDATE your parent to be UNBLOCKEd
	//add parent to the READY QUEUE
	// pop the next ready item from the queue
	// context switch to it..
}


int uthread_join(uthread_t tid, int *retval)
{
	//printf("In join");

	if(running_thread->TID == tid){
		return -1;
	}//if running thread is parent

	if(tid == 0){return -1;}

	int found =0;
	int blocked = 0;
	struct Node *curr_zombie = zombie_queue->front;
	struct Node *curr_q = q->front;
	struct Node *curr_blocked = blocked_queue->front;
	
	while(curr_q != NULL){
		struct thread* temp = curr_q->key; //get the struct
		if(temp->TID == tid){
			found = 1;
			if(temp->is_joined == false){
				blocked = 1;
				temp->is_joined = true;
				running_thread->state = Blocked; //block the running thead.
				queue_enqueue(blocked_queue, running_thread);
				temp->join_tid = running_thread->TID;//temp will use join_tid to figure which parent to unblock
				uthread_yield();
				break;
			}
			else{
				return -1; 
			}
		}
		curr_q = curr_q->next;
	}

		while(curr_blocked != NULL){
			struct thread* temp_blocked = curr_blocked->key; //get the struct
			if(temp_blocked->TID == tid){
				found = 1;
				if (temp_blocked->is_joined == false){
			 		blocked = 1;
                                	temp_blocked->is_joined = true;
                                	running_thread->state = Blocked; //block the running thead.
                                	queue_enqueue(blocked_queue, running_thread);
                                	temp_blocked->join_tid = running_thread->TID;//temp will use join_tid to figure which parent to unblock
                                	uthread_yield();
					//delete child
					break;	
				}//if it hasnt been joined before
				else{
					return -1;
				}
			}
			curr_blocked = curr_blocked->next;
		}
		while(curr_zombie != NULL){
			struct thread* temp_zombie = curr_q->key;
			if(temp_zombie->TID == tid){
				found = 1;
				if (temp_zombie->is_joined == false){
					temp_zombie->is_joined = true;
					if(retval != NULL)
                                        	*retval = temp_zombie->retVal;
					//free(curr_zombie);
					//free(curr_q);
					free(temp_zombie->context);
					uthread_ctx_destroy_stack(temp_zombie->stack);
				}
				else{
					return -1;
				}
				//if(blocked == 1){
					/*running_thread->state = Ready;
					struct thread* running = running_thread;

					queue_enqueue(q, running);
					struct Node *top = q->front;
        				struct thread* temp = (struct thread*)top->key;
					//switch from your running thread tp next scheduled thread...
        				if(queue_length(q) > 0){
                				struct thread *curr;//= malloc(sizeof(thread));
                				queue_dequeue(q,(void **)&curr);
                				uthread_ctx_switch(running->context, temp->context); //what if there is nothing left in queue?
        				}*/
					uthread_yield();
				//}
				//removde temp_zombie from enqueue.. Deltete it!
				break;
			}
			curr_zombie = curr_zombie->next;
		}
		/*while(1){
			if(queue_length(q) == 0){
				break;
			}
			uthread_yield();
		}*/
	if(found == 0){
		return -1;
	}
		return 0;
}

int main_thread(uthread_func_t func, void *arg)
{
//	printf("reached main_thread\n");
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
	zombie_queue = queue_create();
	blocked_queue = queue_create();
	q = queue_create();
	//printf("Queue created\n");	
	running_thread = thread;
	//printf("%d", running_thread->TID);
	//queue_enqueue(q, thread); //add the struct to the queue
	return 0; //return TID
}

