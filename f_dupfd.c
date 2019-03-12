#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    int testfd;
    int fd;

    fd  = open("test.txt", O_CREAT);

    testfd = fcntl(fd, F_DUPFD, 10);
    printf("testfd :%d\n", testfd);
    testfd = fcntl(fd, F_DUPFD, 10);
    printf("testfd :%d\n", testfd);

    getchar();
}

