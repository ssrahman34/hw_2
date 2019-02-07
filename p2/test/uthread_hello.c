/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

//#include <../libuthread/uthread.h>
#include "../libuthread/uthread.c"
#include "../libuthread/uthread.h"
int hello(void* arg)
{
	printf("Hello sarah and world!\n");
	return 0;
}

int main(void)
{
	uthread_t tid;

	tid = uthread_create(hello, NULL);
	uthread_join(tid, NULL);

	return 0;
}
