#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>


void* t_function(void *data)
{
	// 자원 할당
	char a[100000];
	static int num = 0;
	num = *((int*)data);
	printf("Thread start\n");
	sleep(5);
	printf("Thread end\n");

	return (void*)&num;
}

int main()
{
	pthread_t p_thread;
	int t_id;
	int retValue;

	int a = 10;

	printf("Before Thread Create\n");
	t_id = pthread_create(&p_thread, NULL, t_function, (void*)&a);
	if (t_id < 0)
	{
		perror("Error:pthread_create:");
		return -1;
	}
	pthread_detach(p_thread);
	pause();
	return 0;
}