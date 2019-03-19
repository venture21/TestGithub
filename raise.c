#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigHandler(int sig)
{
	printf("raise() : I got signal=%d\n",sig);
	(void)signal(SIGINT,SIG_DFL);
}


int main(void)
{
	int count=0;
	signal(SIGINT, sigHandler);
	while(1)
	{
		printf("Hello World\n");
		count++;
		if(count==3)
		{
			raise(SIGINT);
			count=0;
		}
		sleep(1);
	}
}
