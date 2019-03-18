#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int s, sig;
	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		printf("%s pid sig-num\n", argv[0]);
	
	sig = atoi(argv[2]);

	// 해당 pid로 signal을 전송
	s = kill(atol(argv[1]), sig);

	// signal > 0
	if (sig != 0)
	{
		// kill()함수를 호출했는데 에러가 발생
		if (s == -1)
		{
			printf("kill_1\n");
		}	
	 	/* Null signal: process existence check */
		if (s == 0)
		{
			printf("Process exists & we can send it a signal\n");
		}
		else
		{
			// 권한이 없는 경우
			if (errno == EPERM)
				printf("Process exists, but we don't have "
					"permission to send it a signal\n");

			// 해당 프로세스 ID값의 프로세스가 없는 경우
			else if (errno == ESRCH)
				printf("Process does not exist\n");
			// 기타 에러
			else
				printf("kill_2\n");
		}
	}
	exit(EXIT_SUCCESS);
}
