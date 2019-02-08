//Tests queue implementation,
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../libuthread/queue.h"
#include "../libuthread/queue.c"

static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}



void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
}




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
	test_iterator();
        return 0;
}
