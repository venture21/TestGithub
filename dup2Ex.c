#include <stdio.h>
#include <unistd.h>  // STDOUT_FILENO
#include <fcntl.h>

int main(int argc,char**argv)
{
    int fd = 0;

    if(argc != 2)
    {
        fprintf(stderr,"usage: %s [filename]\n",argv[0]);        
        return 1;
    }

    fd= open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd == -1)
    {
        perror("failed open ");      
        return 1;
    }

    if(dup2(fd,STDOUT_FILENO) == -1)
    {
        perror("failed dup2");      
        return 1;
    }
    printf("hello world\n");

    close(fd);
    return 0;
}

