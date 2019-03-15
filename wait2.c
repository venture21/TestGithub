#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
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


int main(void)
{
	pid_t pid;
	char *message;
	int n;
	int exit_code;
	// 프로그램 시작시 문자열 출력
	printf("fork program starting\n");

	// fork()함수를 호출하여 자식 프로세스 생성
	pid = fork();

	switch(pid) 
	{
		// 자식 프로세스 생성이 안된 경우
		case -1:
			perror("fork failed");
			exit(1);

		// 자식 프로세스인 경우 case 0 실행
		case 0:
			message = "This is the child";
			n = 5;
			exit_code = 37;
			break;
		// 부모 프로세스인 경우 default 실행
		default:
			message = "This is the parent";
			n = 3;
			exit_code = 0;
			break;
	}

	for(; n > 0; n--) 
	{
		puts(message);
		sleep(1);
	}

	// 부모 프로세스인 경우 if문 실행
	if (pid != 0) 
	{
		int status;
		pid_t child_pid;

		// wait()함수를 통해 자식 프로세스가 끝나기를 기다림
		child_pid = wait(&status);
		printf("Child has finished: PID = %d\n", child_pid);

		pr_exit(status);
	}
		exit(exit_code);
}
