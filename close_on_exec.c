#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    int fd;
    int val;
    fd = open("exec_copy.txt", O_CREAT);
/*
    // FD_CLOEXEC 값을 fcntl 을 이용해서 
    // 가져온 다음 세팅되어 있는지 검사한다.  
    val = fcntl(fd, F_GETFD, 0);
    if (val & FD_CLOEXEC)
        printf("close-on-exec bit on\n");
    else
        printf("close-on-exec bit off\n");

    // FD_CLOEXEC 를 세팅한다. 
    val |= FD_CLOEXEC;
    if (val & FD_CLOEXEC)
        printf("close-on-exec bit on\n");
    else
        printf("close-on-exec bit off\n");
    fcntl(fd, F_SETFD, val);
*/
    // loop 프로그램을 exec 시킨다.  
    execl("./loop", "./loop", (char*)0);

    return 0;
}

