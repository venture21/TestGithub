#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h> // getimeofday( ) 함수에서 사용
#include <stdlib.h>

struct value
{
	int start;
	int end;
};


void *add_func(void *data)
{
	int i;
	long long result=0;
	struct value value1;
	pthread_t t_id; value1 = *((struct value *)data);

	//printf("start = %d\n", value1.start);
	//printf("end = %d\n", value1.end);

	for(i=value1.start;i<value1.end+1;i++)
	{
		result += i;
	}
	//printf("result = %lld\n", result);
	//printf("(void *)result = %p\n",(void *)result);
	return (void *)(result);
}


long long adder(int start, int end)
{
	int i;
	long long result=0;

	for(i = start ;i<=end; i++)
	{
		result += i;
	}

	return result;
}

int main(int argc, char *argv[])
{
	pthread_t p_thread[2];
	int thread_num;
	int err;
	long long sum1, sum2;
	long long result;
	int end = atoi(argv[2]);
	struct value value1,value2;
	clock_t sTime, eTime; 

	thread_num = atoi(argv[1]); 

	switch(thread_num)
	{
		case 1: sTime = clock();
			result = adder(1, end);
			eTime = clock();
			printf("runtime : %lf\n",(double)(eTime-sTime)/CLOCKS_PER_SEC);
			printf("1 thread result = %lld\n", result); 
			break;

		case 2: value1.start = 1;
			value1.end   = end>>1;
			value2.start = (value1.end)+1;
			value2.end   = end;

			sTime = clock();
    			if((err = pthread_create(&p_thread[0], NULL, add_func, (void*)&value1)) < 0)
			{
				perror("thread create error : "); 
				exit(1);
			}

			if((err = pthread_create (&p_thread[1], NULL, add_func, (void*)&value2)) < 0)
			{
				perror("thread create error : ");
				exit(2);
			}

			pthread_join(p_thread[0], (void **)&sum1);
			//printf("add1 result : %lld\n", sum1);
			pthread_join(p_thread[1], (void **)&sum2);
			//printf("add2 result : %lld\n", sum2);
			
			eTime = clock();
			result = sum1 + sum2;
			printf("runtime : %lf\n",(double)(eTime-sTime)/CLOCKS_PER_SEC);	
			printf("2 thread result = %lld\n", result); 
			break;
		default:
			printf("parameter error\n");
	}

	return 0;
}
