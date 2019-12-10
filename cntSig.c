#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigHandler(int sig)
{
	static int cntsig=0;

	if(sig==SIGINT)
	{
		cntsig++;
	}
	else if(sig==SIGQUIT)
	{
		cntsig--;
	}
	printf("cntsig:%d\n",cntsig);
}

int main(void)
{
	// SIGINT의 signal핸들러를 등록
	if(signal(SIGINT, sigHandler)==SIG_ERR)
		printf("SIGERR!!\n");

	if(signal(SIGQUIT, sigHandler)==SIG_ERR)
		printf("SIGERR!!\n");


	// 반복 구문
	while(1)
	{
		printf("Hello World\n");
		pause(); //sleep(1);
	
	}
}
