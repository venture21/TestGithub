#include <unistd.h>	// open()
//#include <stdlib.h>
#include <fcntl.h>	//O_CREAT
//#include <sys/types.h>
//#include <sys/stat.h>
#include <stdio.h>

#define DEBUG

int main()
{
    int fd;
    int val;
    fd = open("exec_copy.txt", O_CREAT|O_TRUNC, 0644);
    
    val = fcntl(fd,F_GETFD, 0);  	// GET FD_CLOEXEC

#ifdef DEBUG
    if(val & FD_CLOEXEC)
        printf("Before : close-on-exec bit on\n");
    else
        printf("Before : close-on-exec bit off\n");
#endif

    val |= FD_CLOEXEC;			// Modify FD_CLOEXEC

#ifdef DEBUG
    if(val & FD_CLOEXEC)
        printf("After : close-on-exec bit on\n");
    else
        printf("After : close-on-exec bit off\n");
#endif

    fcntl(fd, F_SETFD, val);		// SET FD_CLOEXEC

    // loop 프로그램을 exec 시킨다.  
    execl("./loop", "./loop", (char*)0);
    return 0;
}

