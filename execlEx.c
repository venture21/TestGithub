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
	// int execl(const char *path, const char *arg, ...)
	// 디렉토리+파일이름을 path와 arg로 입력해 준다.
	//====================================================
	ret = execl("/bin/ps", "/bin/ps", NULL);
	fprintf(stderr, "execl error: %s\n",strerror(errno));

	printf("execl:%d\n",ret);
	return 0;
}
