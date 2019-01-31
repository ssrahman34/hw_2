//Tests queue implementation,
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libuthread/queue.h"
#include "../libuthread/queue.c"

int main(){

        struct queue* q = queue_create();
	int ret = queue_enqueue(q, 1);
	printf("%d", ret);
	printf("%d", q->array[0]);
	queue_enqueue(q, 2);
	queue_enqueue(q, 9);
	queue_delete(q, 2);
	printf("%d %d %d", q->array[0], q->array[1], q->array[2]);
	printf("%d", q->size);
	queue_enqueue(q, 6);
	printf("%d %d %d", q->array[0], q->array[1], q->array[2]);
	int** d;
	queue_dequeue(q,&d);
	printf("NOW: %d %d", q->array[q->front], q->array[q->front+1]);
	//printf("%d", d);

        return 0;
}
