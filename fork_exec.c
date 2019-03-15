#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void my_execv(const char *path, char *const argv[]) 
{
    int ret;
    pid_t pid;

    pid = fork();

    if (pid == -1) 
	{ 
		perror("fork"); 
		exit(EXIT_FAILURE); 
	}

    if (pid == 0) 
	{
        ret = execv(path, argv);

        if (ret == -1) 
		{ 
			perror("execv"); 
			exit(EXIT_FAILURE); 
		}
    }

    wait(NULL);
}

int main() 
{
    char path[] = "/bin/ls";
    char *argv_ls[] = { "ls", "-al", NULL };

    my_execv(path, argv_ls);

    return 0;
}
