#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct Data
{
	int x;
	int y;
};


void *t_function(void *data)
{
	struct Data data1;
	data1 = *((struct Data*)data);

	return (void*)(data1.x + data1.y);
}

int main()
{
	pthread_t p_thread;
	int err;
	int status;

	struct Data data;
	data.x=11;
	data.y=3;

	err=pthread_create(&p_thread, NULL, t_function, (void*)&data);

	if(err<0)
		perror("Thread create error:");


	pthread_join(p_thread, (void**)&status);
	printf("return value : %d\n", status);
	
	return 0;
}
