#include <signal.h>		//signal()
#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <unistd.h> 	//pause()

static void sigHandler(int sig)
{
	static int count = 0;
	
	count++;

	if(sig==SIGINT)
	{
		printf("Caught SIGINT (%d)\n", count);
		return; /* Resume execution at point of interruption */
	}
	else //if(sig==SIGQUIT)
	{
		printf("Caught SIGQUIT (%d)\n", count);
		//return; /* Resume execution at point of interruption */		
	}

	// Must be SIGQUIT - print a message and terminate the process 
	printf("Caught SIGQUIT - that's all folks!\n");
	exit(0);
}


int main(int argc, char *argv[])
{
	/* Establish same handler for SIGINT and SIGQUIT */
	if (signal(SIGINT, sigHandler) == SIG_ERR)
		return -1;
	
	if (signal(SIGQUIT, sigHandler) == SIG_ERR)
		return -1;

	for (;;) /* Loop forever, waiting for signals */
		pause(); /* Block until a signal is caught */
}
