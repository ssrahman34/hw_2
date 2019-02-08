# Target library
lib := libuthread.a
CFLAGS := -Wall -Werror
CC-FLAGS1 := gcc $(CFLAGS)
CC-FLAGS2 = gcc $(CFLAGS)
all: $(lib)

## TODO: Phase 1.1
queue.o: queue.h queue.c
	gcc -Wall -Werror -c -o queue.o queue.c

context.o: context.h context.c
	gcc -Wall -Werror -c -o context.o context.c

uthread.o: uthread.h uthread.c
	gcc -Wall -Werror -c -o uthread.o uthread.c

preempt.o: preempt.h preempt.c
	gcc -Wall -Werror -c -o preempt.o preempt.c

libuthread.a: queue.o context.o uthread.o preempt.o
        ar rcs lib queue.o context.o uthread.o preempt.o

clean: rm -f queue.o context.o uthread.o preempt.o
