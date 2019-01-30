# Target library
lib := libuthread.a

all: $(lib)

## TODO: Phase 1.1
queue.o: queue.h queue.c
	gcc -Wall -Werror -c -o queue.o queue.c
libuthread.a: queue.o
	ar rcs lib queue.o