#include <stdio.h>
#include <time.h>
#include <sys/time.h> // getimeofday( ) 함수에서 사용
#include <stdlib.h>

int main(int argc, char **argv)
{
int i, j;
 time_t UTCtime;
 struct tm *tm;
 char buf[BUFSIZ];
 struct timeval UTCtime_u;

 time(&UTCtime); // UTC 현재 시간 구하기
 printf("time : %u\n", (unsigned)UTCtime); // UTC 현재 시간 출력

 gettimeofday(&UTCtime_u, NULL); // UTC 현재 시간 구하기(마이크로초까지)
 printf("gettimeofday : %ld/%ld\n", UTCtime_u.tv_sec, UTCtime_u.tv_usec);

 printf("ctime : %s", ctime(&UTCtime)); // 현재의 시간을 문자열로 바꿔서 출력

 tm = localtime(&UTCtime); // tm = gmtime(&UTCtime);
 printf("asctime : %s", asctime(tm)); // 현재의 시간을 tm 구조체를 이용해서 출력

 //strftime(buf, sizeof(buf), "%A %m %e %H:%M:%S %Y", tm); // 사용자 정의 문자열 지정
 strftime(buf, sizeof(buf), "%H:%M", tm); // 사용자 정의 문자열 지정
 printf("strftime : %s\n", buf);

 return 0;
}


