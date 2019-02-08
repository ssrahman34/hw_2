#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
//sigset_t block;

void preempt_disable(void)
{
	//sigfillset(SIGVTALRM); //signal blocking function
}

void preempt_enable(void)
{
	//sigemptyset(SIGVTALRM);
}

void preempt_start(void)
{
	struct sigaction action;
	struct itimerval value;//1000ms
	value.it_value.tv_usec = 10000;
	//SIGVTalarm
	//sigemptyset(&action.sa_mask);
	action.sa_flags = SA_NODEFER;//SA_NOMASK
	action.sa_handler = &uthread_yield; //call yield fn
	//setitimer (value, &new, &old); //do we check <0?
	sigaction(SIGVTALRM,&action, NULL);
	//signal (SIGALRM, catch_alarm)

}
//https://stackoverflow.com/questions/5316749/having-trouble-using-sigaction-with-a-timer-signal
