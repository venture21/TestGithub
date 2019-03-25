#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// IPC & semaphore
#include <sys/ipc.h>
#include <sys/sem.h>

/*
struct sembuf {
	short sem_num;    세마포어 번호
	short sem_op;     세마포어 증감값
	short sem_flg;    옵션
}
IPC_NOWAIT : 호출 즉시 실행하지 못했을 경우 기다리지 않고
             실패로 바로 복귀합니다.
SEM_UNDO   : 프로세스가 종료되면 시스템에서 세마포어 설정을
             원래 상태로 되돌립니다.(보통 이 옵션을 사용)

*/

union semun {
	int				val;
	struct semid_ds *buf;
	unsigned short int *array;
}arg;


int cnt = 0;
static int semid;

void p()
{
	struct sembuf pbuf;
	pbuf.sem_num = 0;
	pbuf.sem_op  = -1;
	pbuf.sem_flg = SEM_UNDO;
	if (semop(semid, &pbuf, 1) == -1)
		perror("p:semop()");
}


void v()
{
	struct sembuf vbuf;
	vbuf.sem_num = 0;
	vbuf.sem_op  = 1;
	vbuf.sem_flg = SEM_UNDO;
	if (semop(semid, &vbuf, 1) == -1)
		perror("v:semop()");
}


int main(int argc, char *argv[])
{
	//step1. semget
	if((semid=semget(IPC_PRIVATE, 1, IPC_CREAT|0666))==-1)
	{
		perror("Error:semget()");
		return -1;
	}

	arg.val = 1;

	//step2. semctl SETVAL
	if (semctl(semid, 0, SETVAL, arg) == -1)
	{
		perror("Error:semctl():SETVAL");
		return -1;
	}

	//Test Code
	while (1)
	{
		if (cnt < 9)
		{
			p();
			cnt++;
			v();
			printf("increase :%d\n", cnt);
			usleep(100);

		}
		else
		{
			p();
			cnt--;
			v();
			printf("decrease :%d\n", cnt);
			break;
		}
	}
	//step3. semctl IPC_RMID
	if (semctl(semid, 0, IPC_RMID, arg) == -1)
	{
		perror("semctl():IPC_RMID");
	}
	return 0;
}

