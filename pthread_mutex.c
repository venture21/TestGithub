#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

int cnt=0;

pthread_mutex_t mutex_cnt;

void* inc_function(void*arg)
{
	pthread_mutex_lock(&mutex_cnt);
	printf("Before Inc:%d\n", cnt);
	cnt++;
	printf("After Inc:%d\n", cnt);
	pthread_mutex_unlock(&mutex_cnt);
}

void* dec_function(void*arg)
{
	pthread_mutex_lock(&mutex_cnt);
	printf("Before dec:%d\n", cnt);
	cnt--;
	printf("After decc:%d\n", cnt);
	pthread_mutex_unlock(&mutex_cnt);
}


int main(int argc, char* argv[])
{
	pthread_t p_thread[2];

	//Mutex 초기화 함수
	pthread_mutex_init(&mutex_cnt, NULL);

	pthread_create(&p_thread[0], NULL, inc_function, NULL);
	pthread_create(&p_thread[1], NULL, dec_function, NULL);

	pthread_join(p_thread[0], NULL);
	pthread_join(p_thread[1], NULL);

	pthread_mutex_destroy(&mutex_cnt);

	return 0;
}

