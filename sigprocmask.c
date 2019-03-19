#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sigHandler(int signo)
{
    printf("signal no : %d\n", signo); 
}

int main(void)
{
	struct sigaction act;
    sigset_t newmask, oldmask, pendmask;
    act.sa_handler = sigHandler;
	act.sa_flags = 0;
    if(signal(SIGQUIT, sigHandler) == SIG_ERR)
    	printf("can't catch SIGQUIT\n");

	if(signal(SIGINT, sigHandler) == SIG_ERR)
		printf("can't catch SIGINT\n");
                
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask)<0)
        printf("SIG_BLOCK error\n");
	printf("SIGQUIT is blocked\n");
	sleep(10);
    
	if(sigpending(&pendmask)<0)
        printf("sigpending error\n");
 
	if(sigismember(&pendmask, SIGQUIT))
		printf("SIGQUIT pending\n");

    if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)
        printf("SIG_SETMASK error");
    printf("SIGQUIT is unblocked\n");
    
    sleep(5);
    exit(0);
 }
