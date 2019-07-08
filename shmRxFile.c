#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

//For IPC 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	8192
#define BUF_SIZE 8192

int main(void)
{
	void *shared_Mem = (void*)0;
	void *shared_Mem2 = (void*)0;
	int shmid;
	int shmid2;
	int *shmaddr;
	int *shmaddr2;
        int rdData;
	int i;
	int fd;
	int len;
	char buf[BUF_SIZE];

	// step0. ftok()

	// step1.1 shmget data
	shmid = shmget((key_t)1234, sizeof(char)*SHMSIZE, 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "shmget data failed\n");
		exit(EXIT_FAILURE);
	}

        // step1.2 shmget flag
        shmid2 = shmget((key_t)1235, sizeof(int)*2, 0666 | IPC_CREAT);
        if (shmid == -1)
        {
                fprintf(stderr, "shmget flag failed\n");
                exit(EXIT_FAILURE);
        }

	// step2.1 shmat data
	shared_Mem = shmat(shmid, (void*)0, 0);
	if (shared_Mem == (void*)-1)
	{
		fprintf(stderr, "shmat data failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Memory attached at %p\n", (char*)shared_Mem);
	shmaddr = (char*)shared_Mem;

        // step2.2 shmat flag
        shared_Mem2 = shmat(shmid2, (void*)0, 0);
        if (shared_Mem2 == (void*)-1)
        {
                fprintf(stderr, "shmat flag failed\n");
                exit(EXIT_FAILURE);
        }
        printf("Memory attached at %p\n", (char*)shared_Mem2);
        shmaddr2 = (char*)shared_Mem2;


	// step3. memory access
	fd = open("lena_copy.jpg", O_WRONLY | O_CREAT | O_TRUNC);
	printf("flag1=%d\n", *(shmaddr2));
	while(*(shmaddr2)!=2)
	{
		len = *(shmaddr2+1);
		memcpy(buf,shmaddr,len);
		*(shmaddr2)=0;
		write(fd,buf,len);
		printf("flag2=%d\n",*(shmaddr2));
		//sleep(1);
	}
		
	// step4.1 shmdt data
	if (shmdt(shared_Mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	
        // step4.2 shmdt flag
        if (shmdt(shared_Mem2) == -1)
        {
                fprintf(stderr, "shmdt flag failed\n");
                exit(EXIT_FAILURE);
        }


	// step 5.1 shmctl : IPC_RMID
	if (shmctl(shmid, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl (IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}

        // step 5.2 shmctl : IPC_RMID
        if (shmctl(shmid2, IPC_RMID, 0) == -1)
        {
                fprintf(stderr, "shmctl (IPC_RMID) failed\n");
                exit(EXIT_FAILURE);
        }
	
	exit(EXIT_SUCCESS);
}
