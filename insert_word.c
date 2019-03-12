#include <unistd.h>        //write(), close(), lseek()
#include <sys/types.h>
#include <fcntl.h>		   //creat()
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 100

int main(void)
{
	int fd1, fd2;
	int rd_cnt;
	char word[10];
	char buf[BUF_SIZE];
	memset(word,0,sizeof(word));
	sprintf(word,"vehicles ");
	memset(buf,0,sizeof(buf));

	fd1=open("tesla.txt", O_RDONLY|O_NONBLOCK);
	fd2=open("result.txt", O_WRONLY|O_CREAT|O_TRUNC,0644);
	
	lseek(fd1,0,SEEK_SET);
	rd_cnt=read(fd1,buf,34);
	write(fd2,buf,34);
	printf("buf:%s\n",buf);

	memset(buf,0,sizeof(buf));
	rd_cnt=read(fd1,buf,sizeof(buf));
	printf("buf:%s\n",buf);

	write(fd2,word,9);
	write(fd2,buf,strlen(buf));

	close(fd1);
	close(fd2);
	return 0;
}
