#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//POSIX Thread
#include <pthread.h>

#define DATASIZE	sizeof(int)*4

int glob_var = 11;
int *addr;

// Thread function
void* t_function(void* data)
{
	int i;
	int argData;
	pthread_t t_id;

	argData = *((int*)data);
	t_id = pthread_self();
	sleep(argData);
	printf("getpid=%d, tid=%lu, argData:%d\n, glob_var:%d", getpid(), t_id, argData, glob_var);
	for (i = 0; i < 4; i++)
	{
		printf("%d ", *(addr+i));
	}
	printf("\n");
	sleep(5);

	return (void*)(argData*argData);
}

int main(void)
{

	//POSIX thread id data type;
	pthread_t p_thread[2];
	int i;
	int ret;
	int a = 1;
	int b = 2;
	int status;

	addr = malloc(DATASIZE);

	for (i = 0; i < 4; i++)
	{
		*(addr+i) = i;
	}

	printf("PID=%d\n", getpid());

	if ((ret = pthread_create(&p_thread[0], NULL, t_function, (void*)&a)) < 0)
	{
		perror("Error:pthread_create1()");
		return -1;
	}

	if ((ret = pthread_create(&p_thread[1], NULL, t_function, (void*)&b)) < 0)
	{
		perror("Error:pthread_create2()");
		return -2;
	}

	pthread_join(p_thread[0], (void**)&status);
	printf("thread_join1:%d\n", status);

	pthread_join(p_thread[1], (void**)&status);
	printf("thread_join2:%d\n", status);

}
