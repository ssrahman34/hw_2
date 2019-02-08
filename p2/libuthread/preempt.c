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
sigset_t *set;
struct itimerval value;//1000ms
/*int main(){
preempt_start();
return 0;
}*/
void preempt_disable(void)
{
        value.it_value.tv_usec = 0; //disable alarm
        value.it_value.tv_sec = 0; //disable alarm
         //sigdelset(set, SIGVTALRM);
        //sigfillset(SIGVTALRM); //signal blocking function
}
void preempt_enable(void)
{
        value.it_value.tv_usec = 10000; //disable alarm
        value.it_value.tv_sec = 10000; //disable alarm
//      sigaddset(set, SIGVTALRM);
        //sigemptyset(SIGVTALRM);
}
void preempt_start(void)
{
        struct sigaction action;
        sigemptyset (&action.sa_mask);
        value.it_value.tv_usec = 10000;
        value.it_value.tv_sec = 0;
        value.it_interval.tv_sec = 0;
        value.it_interval.tv_usec = 10000;
        action.sa_handler = uthread_yield;//uthread_yield; //call yield fn
        sigaction(SIGVTALRM,&action, NULL);

}
//https://stackoverflow.com/questions/5316749/having-trouble-using-sigaction-with-a-timer-signal
