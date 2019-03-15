#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

int main(void)
{
	int ret;
	char *const ps_envp[] ={"PATH=/bin/", NULL};
	char *const arg[] = {"ps", "-ax", NULL};
	ret = execve("/bin/ps", arg, ps_envp);
	fprintf(stderr, "execve error: %s\n",strerror(errno));

	printf("execve:%d\n",ret);
	return 0;
}
