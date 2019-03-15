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
	ret = execle("/bin/ps", "ps", "-ax", NULL, ps_envp);
	fprintf(stderr, "execle error: %s\n",strerror(errno));

	printf("execle:%d\n",ret);
	return 0;
}
