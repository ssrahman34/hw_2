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
struct queue * q;
struct queue * blocked_queue;
struct queue * zombie_queue;

struct thread * running_thread;

typedef enum {
    Blocked,
    Running,
    Ready,
    Zombie
}
State;

typedef struct thread {
    uthread_t TID;
    uthread_t join_tid;
    bool is_joined;
    State state;
    int retVal; //so we can use in jion
    uthread_ctx_t * context;
    void * arg;
    uthread_func_t func;
    int * stack; // = uthread_ctx_alloc_stack();
}
thread;

int main_thread(uthread_func_t func, void * arg);

/* Look for the next available thread */
void uthread_yield(void) {
    if (queue_length(q) > 0) {
        struct thread * running = running_thread;
        struct Node * top = q -> front;
        struct thread * temp = (struct thread * ) top -> key;
        struct thread * curr = NULL; //= malloc(sizeof(thread));
        //prempt disable?
        queue_dequeue(q, (void ** ) & curr);

        if (running -> state == Running) {
            running -> state = Ready;
            queue_enqueue(q, (void * ) running);
        }
        temp -> state = Running;
        running_thread = temp;
        uthread_ctx_switch(running -> context, temp -> context);
    }
    return;
}

uthread_t uthread_self(void) {
    return running_thread -> TID;
}

int uthread_create(uthread_func_t func, void * arg) {
    if (num_threads == 0) {
        main_thread(func, arg);
    }
    struct thread * thr = malloc(sizeof(struct thread));
    void * top_of_stack = uthread_ctx_alloc_stack();

    thr -> TID = num_threads;
    thr -> func = func;
    thr -> arg = arg;
    thr -> is_joined = false;
    //thread->parent = running_thread;
    thr -> state = Ready; //assign to read?
    thr -> stack = top_of_stack; //assign stack
    thr -> context = malloc(sizeof(uthread_ctx_t));
    int retVal = uthread_ctx_init(thr -> context, top_of_stack, func, arg);
    if (retVal != 0) {
        return -1;
    }
    num_threads++; //increment thread number
    queue_enqueue(q, (void * ) thr); //add the struct to the queue
    return thr -> TID;
}

void uthread_exit(int retval) {
    running_thread -> state = Zombie;
    running_thread -> retVal = retval;
    struct thread * running = running_thread;
    uthread_t TID_to_unblock = running_thread -> join_tid;
    queue_enqueue(zombie_queue, running);

    struct Node * curr_blocked = blocked_queue -> front;

    while (curr_blocked != NULL) {
        struct thread * temp_blocked = curr_blocked -> key;
        if (temp_blocked -> TID == TID_to_unblock) {
            temp_blocked -> state = Ready;
            queue_delete(blocked_queue, temp_blocked); //make it unblocked
            queue_enqueue(q, temp_blocked);
            break;
        } //we found which one to unblocn!
        curr_blocked = curr_blocked -> next;
    }
    uthread_yield();
}

int uthread_join(uthread_t tid, int * retval) {
    if (running_thread -> TID == tid) {
        return -1;
    } //if running thread is parent

    if (tid == 0) {
        return -1;
    }

    int found = 0;
    int blocked = 0;
    struct Node * curr_zombie = zombie_queue -> front;
    struct Node * curr_q = q -> front;
    struct Node * curr_blocked = blocked_queue -> front;

    while (curr_q != NULL) {
        struct thread * temp = curr_q -> key; //get the struct
        if (temp -> TID == tid) {
            found = 1;
            if (temp -> is_joined == false) {
                blocked = 1;
                temp -> is_joined = true;
                running_thread -> state = Blocked; //block the running thead.
                queue_enqueue(blocked_queue, running_thread);
                temp -> join_tid = running_thread -> TID; //temp will use join_tid to figure which parent to unblock
                uthread_yield();
                break;
            } else {
                return -1;
            }
        }
        curr_q = curr_q -> next;
    }

    while (curr_blocked != NULL) {
        struct thread * temp_blocked = curr_blocked -> key; //get the struct
        if (temp_blocked -> TID == tid) {
            found = 1;
            if (temp_blocked -> is_joined == false) {
                blocked = 1;
                temp_blocked -> is_joined = true;
                running_thread -> state = Blocked; //block the running thead.
                queue_enqueue(blocked_queue, running_thread);
                temp_blocked -> join_tid = running_thread -> TID; //temp will use join_tid to figure which parent to unblock
                uthread_yield();
                //delete child
                break;
            } //if it hasnt been joined before
            else {
                return -1;
            }
        }
        curr_blocked = curr_blocked -> next;
    }
    while (curr_zombie != NULL) {
        struct thread * temp_zombie = curr_q -> key;
        if (temp_zombie -> TID == tid) {
            found = 1;
            if (temp_zombie -> is_joined == false) {
                temp_zombie -> is_joined = true;
                if (retval != NULL)
                    *
                    retval = temp_zombie -> retVal;
                free(temp_zombie -> context);
                uthread_ctx_destroy_stack(temp_zombie -> stack);
                break;
            } else {
                return -1;
            }
            uthread_yield();
            break;
        }
        curr_zombie = curr_zombie -> next;
    }
    if (found == 0) {
        return -1;
    }
    return 0;
}

int main_thread(uthread_func_t func, void * arg) {
    struct thread * thread = malloc(sizeof(struct thread));
    thread -> TID = num_threads;
    thread -> state = Running; //assign to read?
    thread -> arg = arg;
    thread -> is_joined = false;
    thread -> func = func;
    thread -> context = malloc(sizeof(uthread_ctx_t));
    void * top_of_stack = uthread_ctx_alloc_stack();
    thread -> stack = top_of_stack; //assign stack
    int retVal = uthread_ctx_init(thread -> context, top_of_stack, func, arg);

    if (retVal == -1) {
        return -1;
    }
    num_threads++; //increment thread number
    zombie_queue = queue_create();
    blocked_queue = queue_create();
    q = queue_create();
    running_thread = thread;
    return 0; //return TID
}
