#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int main(void)
{
	int ret;

	//====================================================
	// int execlp(const char *file, const char *arg, ...)
	// 파일이름을 path와 arg로 입력해 준다.
	//====================================================
	ret = execlp("ps", "ps -ax", NULL);
	fprintf(stderr, "execlp error: %s\n",strerror(errno));

	printf("execlp:%d\n",ret);
	return 0;
}
