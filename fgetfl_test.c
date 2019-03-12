#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int mode, fd, value;

    fd = open("test.sh", O_RDONLY|O_CREAT|O_NONBLOCK,0644);
    if (fd < 0)
    {
        perror("open error : ");
        exit(0);
    }

    value = fcntl(fd, F_GETFL, 0);

    mode = value & O_ACCMODE;
    if (mode == O_RDONLY)
        printf("O_RDONLY setting\n");
    else if (mode == O_WRONLY)
        printf("O_WRONLY setting\n");
    else if (mode == O_RDWR)
        printf("O_RDWR setting\n");

    if (value & O_NONBLOCK)
        printf("O_NONBLOCK setting\n");
    else
        printf("BLOCKING mode setting\n");
    if (value & O_ASYNC)
        printf("O_ASYNC setting\n");
}

