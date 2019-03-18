#include <stdio.h>		// printf()
#include <unistd.h>		// fork()
#include <wait.h>		// wait()
#include <signal.h>		// signal(), kill()
#include <stdlib.h>


static void sig_handler(int signum)
{
	printf("I got signal : signum=%d\n",signum);
	exit(0);
}


int main(void)
{

	pid_t pid;

	pid=fork();

	//자식 프로세스일 경우

	if(pid==-1)
	{
		printf("Error:fork pid=-1");
		return -1;
	}
	else if(pid==0)
	{
		signal(SIGKILL,sig_handler);
		while(1)
		{
			printf("Hello world\n");
			sleep(1);
		}
	}
	else
	{
		sleep(10);
		kill(pid,SIGKILL);
	}

	wait(NULL);

	return 0;
}


