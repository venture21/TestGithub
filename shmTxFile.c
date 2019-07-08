#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

//For IPC 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	8192
#define BUF_SIZE 8192

int main(void)
{
	void *shared_Mem  = (void*)0;
	void *shared_Mem2 = (void*)0; 
	int shmid;
	int shmid2;
	int *shmaddr;
	int *shmaddr2;
	int i;
	int len;

	int fd;
	char buf[BUF_SIZE];

	// step 0. ftok()

	// step 1.1 shmget data
	shmid = shmget((key_t)1234, sizeof(char)*SHMSIZE, 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// step 1.2 shmget flag
        shmid2 = shmget((key_t)1235, sizeof(int)*2, 0666 | IPC_CREAT);
        if (shmid2 == -1)
        {
                fprintf(stderr, "shmget failed\n");
                exit(EXIT_FAILURE);
        }

	// step 2.1 shmat data
	shared_Mem = shmat(shmid, (void*)0, 0);
	if (shared_Mem == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}	
	printf("Memory attached at %p\n", (char*)shared_Mem);
	shmaddr = (char*)shared_Mem;

	// step 2.2 shmat flag
        shared_Mem2 = shmat(shmid2, (void*)0, 0);
        if (shared_Mem2 == (void*)-1)
        {
                fprintf(stderr, "shmat failed\n");
                exit(EXIT_FAILURE);
        }

        printf("Memory attached at %p\n", (int*)shared_Mem2);
        shmaddr2 = (int*)shared_Mem2;

	// step 3. memory access
	fd = open("lena.jpg",O_RDONLY);
	//fd=open("test.txt",O_RDONLY);
	while((len=read(fd, buf, sizeof(buf)))>0)
	{
		memcpy(shmaddr, buf, sizeof(buf));
		*(shmaddr2+1)=len;
		*(shmaddr2)=1;
		printf("*(shmaddr2)=%d\n",*(shmaddr2));
		printf("len=%d\n",len);
		//sleep(1);
		while(*(shmaddr2)!=0);
		//printf("shmaddr:%X, data:%d\n", shmaddr + i, *(shmaddr + i));
	}
	printf("123\n");
	*(shmaddr2)=2;
	
	// step 4.1 shmdt data
	if (shmdt(shared_Mem) == -1)
	{
		fprintf(stderr, "shmdt data failed\n");
		exit(EXIT_FAILURE);
	}

        // step 4.2 shmdt flag
        if (shmdt(shared_Mem2) == -1)
        {
                fprintf(stderr, "shmdt flag failed\n");
                exit(EXIT_FAILURE);
        }


	/*
	// step 5. shmctl : IPC_RMID
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl (IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	*/
	exit(EXIT_SUCCESS);
}
