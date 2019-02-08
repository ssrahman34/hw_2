##Data Structures
We used a linked list implementation for our queue. We chose linked lists 
over arrays because since we don’t know how big our queue will be, we are 
able allocate space for our queue as we go. We use a struct called thread 
which is essentially our TCB. We store TID, stack,s state,  the context and 
information like whether the thread has been joined or not. We used 3 
different queues: a ready queue, blocked queue, and zombie queue. We also 
had a struct to store the running struct at any given point in time. 

##Phase 1
queue.c using a Linked List implementation 
A linked list implementation allows for the list to grow easily 
without having to copy over the old content. 
Our queue_create function initializes the queue. We do this by setting
the count of the queue equal to 0 and pointing rear and front to Null.
The enqueue operation adds a new node after rear and moves rear to the 
next node. To do this, we had to create a new node, and point the rear
of the queue to the new node and make the new node rear.
The dequeue operation removes the front node and moves front to the 
next node. 
The delete operation goes through the queue and sets each node to 
be the next node

##Phase 2 and 3
In this phase 2, we implemented the uthread_create, uthread_yield, and 
uthread_join functions. 

The flow of the program is as such:
The first time that a thread is every created,(i.e. In a function we call 
create for the first time)  we make sure to create a “main thread” that is 
running. Then we create the thread that is in the parameter of the create 
function onto a queue of threads. All threads that are first created are 
assigned the state ‘Ready’ and enqueued onto the queue so that we maintain 
the order of which queues are ready. Then as the program proceeds, eventually 
after the creation of threads, the uthread_join function is called. Once 
uthread_join is called with the parameter of the TID of the T2 “child”thread, 
we seach for where this child thread is. There are 3 possible states this child
thread could be in :ready, blocked, and dead. If the T2 child thread is actually
in the state running, we return an error (this would signal we’re trying to 
join on the already running thread).

To figure out where the T2 child is, we’ll look into the zombie queue, 
blocked queue and ready queue. Once found , if T2 is running, we block 
the running thread, and wait for T2 to complete. If T2 is dead, we simply 
store T2’s return value and we are done.

We define a thread struct with attributes such as a TID, context, and stack.
This allows us to create a struct for the new thread and set attributes for 
the thread. If there are no threads that have been created, we create a main
thread in main_thread(). Our main_thread function has the same functionality 
as that of 
create_thread, the only difference being that the default status of the main 
thread is set to running; whereas in uthread_create, we default assign the 
status of the thread to ready and assign the thread to the top of the stack. 
The context attribute is the context object of the specific thread that is 
assigned with the uthread_ctx_init function to initialize the thread. We then 
enqueue the struct onto our ready queue and return the TID of the new thread.

The overview of the yield function is to set aside our current running thread and
instead take the next available thread in the queue. Our yield function goes 
through our ready queue to find the next thread that can be executed. Upon 
finding this thread, we dequeue it off the ready queue. We then change the state
of the thread from Running to Ready
and perform a context switch. 

The uthread exit function occurs when a thread exits itself. Here, we change the
status of the running thread to its zombie state and collect the return value of 
the thread produced in the join function. We go through the blocked queue and
unblock the parent thread and place it onto the ready queue. We then call the
yield function on this ready thread.

##Phase 4
Phase 4 is discussed in our readme file.

