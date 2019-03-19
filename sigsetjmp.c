/* 
   이 예제는 sigsetjmp()와 siglongjmp()를 테스트하기 위한 예제 입니다.
   sigsetjmp()함수는 현재 스택 환경변수(stack enviroment)와 현재 시그널
   마스크 값을 저장한다. sigsetjmp()로 저장된 값은 siglongjmp()로 불러올
   수 있다. sigsetjmp()는 시그널 마스크 값을 추가적으로 저장하는 것 빼고는
   setjmp()와 동일하다.
*/

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t jumpok = 0;

/* ARGSUSED */
static void chandler(int signo) {
   if (jumpok == 0) return;
   siglongjmp(jmpbuf, 1);
}
int main(void)  {
   struct sigaction act;

   act.sa_flags = 0;
   act.sa_handler = chandler;
   if ((sigemptyset(&act.sa_mask) == -1) ||
       (sigaction(SIGINT, &act, NULL) == -1)) {
      perror("Failed to set up SIGINT handler");
      return 1; 
   }/* stuff goes here */
   fprintf(stderr, "This is process %ld\n", (long)getpid());
   if (sigsetjmp(jmpbuf, 1))
      fprintf(stderr, "Returned to main loop due to ^c\n");
   jumpok = 1;
   printf("test\n");

   for ( ; ; );          /* main loop goes here */
}
