# Target library
lib := libuthread.a
all: $(lib)
targets := queue.o uthread.o preempt.o context.o
CFLAGS += -Wall -Werror

## TODO: Phase 1.1

%.o: %.c context.h 
	gcc -c $(CFLAGS) $a $<

%.o: %.c uthread.h 
	gcc -c $(CFLAGS) $a $<

%.o: %.c preempt.h 
	gcc -c $(CFLAGS) $a $<

%.o: %.c queue.h
	gcc -c $(CFLAGS) $a $<

libuthread.a: $(targets)
	ar rcs $(lib) $^

clean: 
	rm -f $(targets)
