#include <stdio.h>		// printf()
#include <unistd.h>		// fork()
#include <wait.h>		// wait()
#include <signal.h>		// signal(), kill()
#include <stdlib.h>

void pr_exit(int status)
{
	// 정상 종료인 경우
	if (WIFEXITED(status))
	    printf("normal termination, exit status = %d\n", WEXITSTATUS(status));

	// 시그널에 의해서 종료된 경우
	else if (WIFSIGNALED(status))
	    printf("abnormal termination, signal number = %d%s\n",WTERMSIG(status),
				WCOREDUMP(status) ? "(core file generated)" : "");

	// 시그널에 의해 STOP된 경우
	else if (WIFSTOPPED(status))
	    printf("child stopped, signal number = %d\n", WSTOPSIG(status));

}



static void sig_handler(int signum)
{
	printf("I got signal : signum=%d\n",signum);
	exit(0);
}


int main(void)
{

	int status;
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

	wait(&status);
	pr_exit(status);

	return 0;
}


