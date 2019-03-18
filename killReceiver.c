#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void sigHandler(int sig)
{
	printf("killTest : I got signal %d\n",sig);
	(void)signal(SIGINT,SIG_DFL);
}

int main(void)
{
	signal(SIGINT, sigHandler);
	while(1)
	{
		printf("Hello World\n");
		sleep(1);
	}
	return 0;
}
