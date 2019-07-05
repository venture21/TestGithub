// 시그널을 부모 프로세스한테 전송하려면
// 부모 프로세스의 PID을 확인 : getppid();

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t ppid;

    ppid = getppid();
    
    while(1)
    {
        printf("Hi\n");
        kill(ppid, SIGUSR1);
        sleep(1);
    }

    return 0;
}
