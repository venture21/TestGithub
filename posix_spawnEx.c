#include <stdio.h>
//#include <unistd.h>
//#include <stdlib.h>

#include <sys/wait.h>
#include <spawn.h>

/*
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

    waitpid(-1, NULL, 0);
}
*/

int my_posix_spawn(const char *path, char *const argv[]) {
    pid_t pid;
    int ret = posix_spawn(&pid, path, NULL, NULL, argv, NULL);

    int status;
    waitpid(pid, &status, 0);

    return ret;
}

int main() 
{
    char path[] = "/bin/ls";
    char *argv_ls[] = { "ls", "-al", NULL };

    
    //my_execv(path, argv_ls);
    my_posix_spawn(path, argv_ls);

    return 0;
}
