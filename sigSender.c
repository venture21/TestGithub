#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int sig;
	pid_t pid;
	pid = atoi(argv[1]);
			
	//sig=atoi(argv[2]);

	kill(pid, SIGINT);

	//sleep(1);
	return 0;
}

