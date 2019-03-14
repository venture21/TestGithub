#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//const char *path = ".";

int main(int argc, char *argv[])
{
    struct  dirent **namelist;
    int     count;
    int     idx;

    if(argc<2)
    {
	printf("argv[1] is empty\n");
	return 1;
    }

    if((count = scandir(argv[1], &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", argv[1], strerror(errno));
        return 1;
    }

    for(idx = 0; idx < count; idx++) {
        printf("%s\n", namelist[idx]->d_name);
    }

     // 건별 데이터 메모리 해제
    for(idx = 0; idx < count; idx++) {
        free(namelist[idx]);
    }

    // namelist에 대한 메모리 해제
    free(namelist);

    return 0;
}
