#include <unistd.h>        //write(), close(), lseek()
//#include <sys/types.h>
#include <fcntl.h>		   //creat()


int main(void)
{
	int fd;
	//create holefile
	fd=creat("./holefile",0644);
	write(fd,"hello",5);

	// current cursor pos + 10
	lseek(fd,10,SEEK_CUR);
	write(fd,"world",5);

	// file zero pos + 1024
	lseek(fd, 100, SEEK_SET);
	write(fd,"bye",3);

	close(fd);
	return 0;
}
