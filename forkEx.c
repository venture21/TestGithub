#include <stdio.h>		 // printf()
#include <unistd.h>		 // fork()
#include <sys/types.h>   // pid_t

int main(void)
{
	pid_t pid;
	int x=0;

	pid=fork();

	x=1;

	if(pid==0)
	{
		printf("pid=%ld, I am process %ld, getppid=%ld \n",(long)pid, (long)getpid(), (long)getppid());
		sleep(1);
		printf("Done\n");
	}
	else
		printf("pid=%ld, I am process %ld\n",(long)pid, (long)getpid());

	return 0;
}

