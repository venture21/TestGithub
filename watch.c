// 부모 프로세스
// 이 프로세스에서 자식프로세스(BlackBox를 호출)
// fork() 실행후 execlp()
// sigHandler()생성
// signal()

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

static void sigHandler(int sig)
{
    printf("BlackBox still alive!\n");
}


int main(void)
{
    pid_t  pid;
    int status;
    int ret;

    signal(SIGUSR1,sigHandler);
    pid = fork();

    switch(pid)
    {
        // 자식 프로세스 생성에 실패한 경우
        case -1: perror("fork failed"); 
                 return -1;

          // 자식 프로세스인 경우
        case  0: ret = execlp("./BlackBox", "BlackBox", NULL);
                 // execlp함수가 정상적으로 호출되지 않은 경우에만 아래 메시지 출력
                 fprintf(stderr, "execlp error: %s\n",strerror(errno));
                 return -1;
        
          // 부모 프로세스인 경우
        default: printf("parent\n");
                 /*
                 while(1)
                 {
                     pause();
                 }
                 */
    }
    pid = wait(&status);
    printf("exited pid = %d\n",pid);
    
    return 0;
} 











