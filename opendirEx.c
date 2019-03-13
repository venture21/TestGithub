#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
    DIR *dir_info;
    struct dirent *dir_entry;

    mkdir("makeA",0755);
    mkdir("makeB",0755);

    dir_info=opendir(".");

    if(NULL!=dir_info)
    {
        while(dir_entry=readdir(dir_info))
        {
            printf("%s\n", dir_entry->d_name); 
        }
        closedir(dir_info);
    }

}


