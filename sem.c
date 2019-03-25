#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// IPC & semaphore
#include <sys/ipc.h>
#include <sys/sem.h>

/*
struct sembuf {
	short sem_num;    �������� ��ȣ
	short sem_op;     �������� ������
	short sem_flg;    �ɼ�
}
IPC_NOWAIT : ȣ�� ��� �������� ������ ��� ��ٸ��� �ʰ�
             ���з� �ٷ� �����մϴ�.
SEM_UNDO   : ���μ����� ����Ǹ� �ý��ۿ��� �������� ������
             ���� ���·� �ǵ����ϴ�.(���� �� �ɼ��� ���)

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
			cnt++;
			v();
			printf("increase :%d\n", cnt);
			usleep(100);
		}
		else
		{
			p();
			cnt--;
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

