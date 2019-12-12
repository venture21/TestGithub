#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pwd.h>	// getpwuid()
#include <grp.h>	// getgrgid()

#define PERM_LENGTH 	11
#define PATH_LENGTH 	100

#define DEBUG

//=========================================
// stat 구조체
//=========================================
/*
struct stat {
    dev_t     st_dev;     // ID of device containing file 
    ino_t     st_ino;     // inode number 
    mode_t    st_mode;    // 파일의 종류 및 접근권한
    nlink_t   st_nlink;   // hardlink 된 횟수
    uid_t     st_uid;     // 파일의 owner
    gid_t     st_gid;     // group ID of owner
    dev_t     st_rdev;    // device ID (if special file)
    off_t     st_size;    // 파일의 크기(bytes)
    blksize_t st_blksize; // blocksize for file system I/O
    blkcnt_t  st_blocks;  // number of 512B blocks allocated
    time_t    st_atime;   // time of last access
    time_t    st_mtime;   // time of last modification
    time_t    st_ctime;   // time of last status change
};
*/

void print_inode(struct stat sb)
{
    printf("===================================\n");

    printf("File type :");

    switch(sb.st_mode & S_IFMT)
    {
        case S_IFBLK:  printf("block device\n");    break;
        case S_IFCHR:  printf("char device\n");     break;
        case S_IFDIR:  printf("directory\n");       break;
        case S_IFIFO:  printf("FIFO\n");            break;
        case S_IFLNK:  printf("symlink\n");         break;
        case S_IFREG:  printf("regular file\n");    break;
        case S_IFSOCK: printf("socket\n");          break;
        default:       printf("unknown?\n");        break;
    }

    printf("I-node number:            %ld\n", (long) sb.st_ino);
    printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
    printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);
    printf("Preferred I/O block size: %ld bytes\n",         (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",        (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",              (long long) sb.st_blocks);
    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));

}


int main(int argc, char *argv[])
{
	struct stat statbuf;	// inode info

	char pathname[PATH_LENGTH];
	char dirname[PATH_LENGTH];

	int flag;
	char temp[20];



	if (argc==1)	
		flag = 0;
		strcpy(dirname,".");

	if(argc>1)
	{
		// "-al" 문자열을 비교
		sprintf(temp,"-al");
        	if(!strcmp(temp,argv[1]))
		{
			flag = 1;
			strcpy(dirname,".");
		}
		else
		{
			flag = 2;
			strcpy(dirname, argv[1]);
		}
	}

		
#ifdef DEBUG	
	printf("dir=%s\n",dirname);
	printf("flag=%d\n",flag);
#endif

	// 디렉토리의 inode정보 읽기
	stat(dirname, &statbuf);
	print_inode(statbuf);

	return 0;
}
