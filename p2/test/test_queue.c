//Tests queue implementation,
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libuthread/queue.h"
#include "../libuthread/queue.c"

int main(){

        struct queue* q = queue_create();
	int ret = queue_enqueue(q, 23);
	int sz = queue_length(q);
	printf("sIZE: %d\n", sz);
	printf("%d", ret);
	printf("%d\n", q->front->key);
	queue_enqueue(q, 45);
	queue_enqueue(q, 8);
	int sz2 = queue_length(q);
        printf("SIZE2: %d\n", sz2);
	printf("REAR: %d", q->rear->key);
	queue_delete(q, 45);
	printf("REAR: %d", q->rear->key);
	printf("test1: %d %d\n", q->front->key, q->front->next->key);
	queue_enqueue(q,7);
	printf("%d %d %d\n", q->front->key, q->front->next->key, q->front->next->next->key);
 printf("REAR: %d", q->rear->key);
 int sz3 = queue_length(q);
        printf("sIZE3: %d\n", sz3);
	int** d;
	queue_dequeue(q,&d);
	printf("SIZE should decrement: %d\n", queue_length(q));
	printf("%d %d %d %d\n", q->front->key, q->front->next->key, q->front->next->next->key ,q->front->next->next->next->key);//note apparently our queue is circular....
	printf("%d", q->rear->key);

        return 0;
}
