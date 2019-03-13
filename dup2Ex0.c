#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	int fd2;
	fd = creat("dup_result", 0644);
	fd2 = dup2(fd, STDOUT_FILENO);

	printf("fd=%d\n", fd);
	printf("fd2=%d\n", fd2);
	close(fd);

	printf("hello world\n");
	//close(fd2);


	return 0;
}
