//======================================
//  파일시스템 정보 얻어오기
// /proc/mounts 정보와 statfs를 이용한다. 
//======================================
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

const char *MMOUNT = "/proc/mounts";

struct f_size
{
    long blocks;
    long avail; 
};

typedef struct _mountinfo 
{
    FILE *fp;               // 파일 스트림 포인터    
    char devname[80];       // 장치 이름
    char mountdir[80];      // 마운트 디렉토리 이름
    char fstype[12];        // 파일 시스템 타입
    long f_type;
    long f_blocks;	    // Total Block count
    long f_bavail;	    // unused block count(Normal User)
    long f_bsize;           // FileSystem Block size
    long f_bfree;
    long f_files;
    long f_ffree;
    struct f_size size;     // 파일 시스템의 총크기/사용율 
} MOUNTP;

MOUNTP *dfopen()
{
    MOUNTP *MP;

    // /proc/mounts 파일을 연다.
    MP = (MOUNTP *)malloc(sizeof(MOUNTP));
    if(!(MP->fp = fopen(MMOUNT, "r")))
    {
        return NULL;
    }
    else
        return MP;
}

MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char *bname;
    char null[16];
    struct statfs lstatfs;
    struct stat lstat; 
    int is_root = 0;

    // /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
    while(fgets(buf, 256, MP->fp))
    {
        is_root = 0;
        sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);
         if (strcmp(MP->mountdir,"/") == 0) is_root=1;
        if (stat(MP->devname, &lstat) == 0 || is_root)
        {
            if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)
            {
                	// 파일시스템의 총 할당된 크기와 사용량을 구한다.        
                statfs(MP->mountdir, &lstatfs);
                MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024); 
                MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024);

		MP->f_type	= lstatfs.f_type;
		MP->f_bsize     = lstatfs.f_bsize;
		MP->f_blocks	= lstatfs.f_blocks;
		MP->f_bavail	= lstatfs.f_bavail;
		MP->f_files     = lstatfs.f_files;
		MP->f_bfree	= lstatfs.f_bfree;
		MP->f_ffree	= lstatfs.f_ffree;
                return MP;
            }
        }
    }
    rewind(MP->fp);
    return NULL;
}

int dfclose(MOUNTP *MP)
{
    fclose(MP->fp);
}

int main()
{
    MOUNTP *MP;
    if ((MP=dfopen()) == NULL)
    {
        perror("error");
        return 1;
    }

    while(dfget(MP))
    {
	printf("====================================\n\n");
	printf("Mount Dir = %s\n", MP->mountdir);
	printf("f_type = %ld\n", MP->f_type);
	printf("Device Name = %s\n", MP->devname);
	printf("FileSystem Type(f_type) = %s\n",MP->fstype);
	printf("block size(f_bsize) = %ld\n",MP->f_bsize);
	printf("Total Block = %ld\n", MP->f_blocks);
	printf("Unused Block(f_bfree) = %ld\n", MP->f_bfree);
	printf("Unused Block(f_bavail) = %ld\n",MP->f_bavail);
	printf("Total inode count(f_files) = %ld\n",MP->f_files);
	printf("Unused inode count(f_ffree) = %ld\n",MP->f_ffree);
        printf("====================================\n\n");
    }
  
    return 0;   
}
