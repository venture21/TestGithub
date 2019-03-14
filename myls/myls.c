#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	DIR *dp;				// DIR pointer
	struct stat statbuf;	// inode info
	struct dirent *dent;

	char perm[11];
	char pathname[80];
	
	if (argc < 2) 
		exit(1);

	stat(argv[1], &statbuf);
	if (!S_ISDIR(statbuf.st_mode)) 
	{
		fprintf(stderr, "%s is not directory\n",argv[1]);
		exit(1);
	}

	if ((dp = opendir(argv[1])) == NULL) 
	{
		perror("Error:");
		exit(1);
	}
	
	printf("Lists of Directory(%s):\n", argv[1]);

	while((dent = readdir(dp)) != NULL) 
	{
		sprintf(pathname, "%s/%s", argv[1], dent->d_name);
		lstat(pathname, &statbuf);
		access_perm(perm, statbuf.st_mode);
		printf("%s %8ld %s\n", perm,statbuf.st_size, dent->d_name);
	}
	closedir(dp);

	return 0;
}
