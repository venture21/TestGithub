#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int sig;
	int i, ret;
	pid_t myPid, childPid;
	myPid=getpid();
	printf("myPid=%d\n",myPid);
	
	childPid=fork();

	// 자식 프로세스가 생성되지 않은 경우
	if(childPid<0)
		return -1;
	// 자식 프로세스인 경우
	else if(childPid==0)
	{
		execl("./cntSig","",NULL);

		//execl실행에 실패했을 경우
		fprintf(stderr, "execl error: %s\n",strerror(errno));
		printf("execl:%d\n",ret);
	}
	else
	{
		// 10번의 SIGINT를 전송한다.
		for(i=0;i<10;i++)
		{
			sleep(1);
			kill(childPid, SIGINT);
		}
		// 자식 프로세스를 강제로 하도록 SIGKILL을 전송한다.
		kill(childPid, SIGKILL);
	}
}
