#include <stdio.h>		// printf()
#include <unistd.h>		// fork()
#include <wait.h>		// wait()
#include <signal.h>		// signal(), kill()
#include <stdlib.h>
#include <string.h>

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



static void sig_killhandler(int signum)
{
	printf("sig_killhandler : signum=%d\n",signum);
	exit(0);
}

static void sig_inthandler(int signum)
{
	printf("sig_inthandler : signum=%d\n",signum);
	exit(0);
}

int main(void)
{

	int status;
	char *temp;
	pid_t pid;

	temp = strsignal(SIGINT);
	printf("%s\n",temp);	

	temp = strsignal(SIGKILL);
	printf("%s\n",temp);

	pid=fork();

	//자식 프로세스일 경우

	if(pid==-1)
	{
		printf("Error:fork pid=-1");
		return -1;
	}
	else if(pid==0)
	{
		// handler에 등록이 불가능한 시그널
		if(signal(SIGKILL,sig_killhandler)==SIG_ERR)
			printf("\ncan't catch SIGKILL\n");

		if(signal(SIGINT,sig_inthandler)==SIG_ERR)
			printf("\ncan't catch SIGINT\n");

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


