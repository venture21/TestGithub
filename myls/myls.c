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
#include <errno.h>
#include "myColor.h"	// printf 색상을 넣기 위해

#include <sys/ioctl.h>	// terminal창의 크기를 읽기 위해
#include <termios.h>	// terminal창의 크기를 읽기 위해

#define PERM_LENGTH 	11
#define PATH_LENGTH 	100
#define NAME_LENGTH	30

//#define DEBUG

//=========================================
// dirent 구조체
//=========================================
/*
struct dirent
{
	long d_ino; //inode 번호
	off_t d_off; // offset
	unsigned short d_reclen; //d_name 길이
	char d_name[NAME_MAX+1]; //파일 이름

};
*/

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

//=========================================
// group 구조체
//=========================================
/*
struct group {
    char   *gr_name;       // group name 
    char   *gr_passwd;     // group password 
    gid_t   gr_gid;        // group ID 
    char  **gr_mem;        // group members 
};
*/

//=========================================
// group 구조체
//=========================================
/*
struct passwd {
	char *pw_name;
	char *pw_passwd;
	uid_t pw_uid;
	gid_t pw_gid;
	time_t pw_change;
	char *pw_class;
	char *pw_gecos;
	char *pw_dir;
	char *pw_shell;
	time_t pw_expire;
}; 
*/


int getColumSize(void)
{

    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);

    //printf ("lines %d\n", ws.ws_row);
    //printf ("columns %d\n", ws.ws_col);

    return ws.ws_col;
}


void print_inode(struct stat sb)
{

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
    printf("\n");
}



//=========================================
// Function Name : access Name
// arg1 : permission value
// arg2 : mode 
// return : void
// Description : 
//=========================================
void access_perm(char *perm, mode_t mode)
{
	int i;
	char permchar[] = "rwx";
	memset(perm, '-', 10);
	perm[10] = '\0';

	if (S_ISDIR(mode)) perm[0] = 'd';
	else if (S_ISCHR(mode)) perm[0] = 'c';
	else if (S_ISBLK(mode)) perm[0] = 'b';
	else if (S_ISFIFO(mode)) perm[0] = 'p';
	else if (S_ISLNK(mode)) perm[0] = 'l';

	for (i = 0; i < 9; i++) 
	{
		if ((mode << i) & 0x100)
			perm[i+1] = permchar[i%3];
	}

	if (mode & S_ISUID) perm[3] = 's';
	if (mode & S_ISGID) perm[6] = 's';
	if (mode & S_ISVTX) perm[9] = 't';
}


int main(int argc, char *argv[])
{
	DIR *dp;		// DIR pointer
	struct stat statbuf;	// inode info
	//struct dirent *dent;
	struct dirent **namelist;
	struct group *group_entry;
	struct passwd *user_pw;

	char perm[PERM_LENGTH];
	char pathname[PATH_LENGTH];
	char dirname[PATH_LENGTH];

	int flag;
	int idx, count;
	int i;
	char temp[20];
	int maxLength;
	int nameLength;
	int colCount;
	int rowCount;
	char buf[NAME_LENGTH];

	//time
	struct tm *tm;
	int columSize;

//=================================================================
// STEP 1. 인자값 받아들이기 & stat함수 사용하기
//=================================================================
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

	if(argc>2)
	{
		flag = 3;
		strcpy(dirname, argv[2]);
	}

        // 디렉토리의 inode정보 읽기
        stat(dirname, &statbuf);
		
#ifdef DEBUG	
	printf("=============================================\n");
	printf(" STEP 1. 인자값 및 inode값 출력하기 \n");
	printf("=============================================\n");
	printf("dir=%s\n",dirname);
        printf("argv: %s %s %s\n",argv[0], argv[1], argv[2]);
	printf("flag=%d\n",flag);
	print_inode(statbuf);
#endif

//=================================================================
// STEP 2. 디렉토리인지 확인하기
//=================================================================

	// 디렉토리가 아닌경우 에러 메시지 출력 후 종료
	if (!S_ISDIR(statbuf.st_mode)) 
	{
		fprintf(stderr, "%s is not directory\n",dirname);
		exit(1);
	}

#ifdef DEBUG
        printf("=============================================\n");
        printf(" STEP 2. 디렉토리가 맞습니다\n");
        printf("=============================================\n");
#endif


//=================================================================
// STEP 3. 디렉토리의 내용을 읽어온다.(더 이상 읽을 내용이 없을때까지
//=================================================================
	if((count = scandir(dirname, &namelist, NULL, alphasort)) == -1)
	{
		fprintf(stderr, "%s Directory Scan Error: %s\n", dirname, strerror(errno));
		return 1;
	}

	columSize = getColumSize();
#ifdef DEBUG
	printf("columSize=%d\n",columSize);
#endif
	
	// 현재
	maxLength=0;
	for(idx=0; idx<count; idx++)
	{
		nameLength = strlen(namelist[idx]->d_name);
		if(maxLength < nameLength)
			maxLength = nameLength;
	}
#ifdef DEBUG
	printf("maxLength=%d\n",maxLength);
#endif
	
	if(flag==1 | flag==3)
	{
		for(idx=0; idx<count; idx++)
		{
			// 경로 + 파일명을 pathname에 저장
			sprintf(pathname, "%s/%s", dirname, namelist[idx]->d_name);

			// lstat을 통해 inode정보를 읽어온다.
			lstat(pathname, &statbuf);

			// 파일의 종류와 권한 정보를 문자열로 변환한다.  
			access_perm(perm, statbuf.st_mode);

			// uid 값을 소유자 문자열로 변환
			user_pw=getpwuid(statbuf.st_uid);

			// gid 값을 그룹 문자열로 변환
			group_entry=getgrgid(statbuf.st_gid);

			//파일 수정 시간을 문자열로 변환 
			tm = localtime(&statbuf.st_mtime); 
			strftime(temp, sizeof(temp), "%m월 %e %H:%M", tm);

			// 화면에 아래 정보를 출력
			// 파일의 종류, 권한, 링크 갯수, 소유자, 소유자그룹, 파일크기, 날짜, 시간을 출력
			printf("%s %3ld %6s %6s %8ld %s ", \
				perm, statbuf.st_nlink, user_pw->pw_name, \
				group_entry->gr_name, statbuf.st_size, temp);

			// 파일의 타입을 읽고 파일명에 색상을 입힌다.
    			switch(statbuf.st_mode & S_IFMT)
    			{
				case S_IFDIR:	printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "\n",namelist[idx]->d_name);
						break;
        			case S_IFREG:	printf("%s\n",namelist[idx]->d_name);    
						break;
        			default:	printf("%s\n",namelist[idx]->d_name);       
						break;
    			}
			
		}
	}
	else
	{

		colCount = columSize/maxLength;
#ifdef DEBUG
		printf("colCount = %d\n", colCount);
#endif
		for(i=0;i<count;i++)
		{
			memset(buf, 0,sizeof(buf));
			sprintf(buf, "%s",namelist[i]->d_name);
			printf("%s",buf);
			for(idx=0;idx<maxLength-strlen(buf);idx++)
				printf(" ");

			if((i%colCount)==colCount-2)
				printf("\n");
			else
				printf("  ");
			//sleep(1);
			
		}
		printf("\n");
	}
        // 건별 데이터 메모리 해제 
        for(idx = 0; idx < count; idx++)
        {
                free(namelist[idx]);
        }

        // namelist에 대한 메모리 해제 
        free(namelist);

	return 0;
}
