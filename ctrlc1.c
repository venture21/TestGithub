#include <signal.h>
#include <stdio.h>
#include <unistd.h>

// SIGINT에 대응하는 핸들러
void sigHandler(int sig)
{
	static int count=0;
	printf("OUCH! - I got signal %d\n", sig);
	if(count==4)
		(void)signal(SIGINT, SIG_DFL);
	count++;
}

// main함수
int main(void)
{
	// SIGINT 신호가 수신되었을 때,
	// sigHandler()함수가 호출되도록 등록
	signal(SIGINT, sigHandler);
	
	while(1) 
	{
		printf("Hello World!\n");
		sleep(1);
	}
}
