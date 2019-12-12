#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

int main()
{
   DIR            *dir_info;
   struct dirent  *dir_entry;

   dir_info = opendir( ".");              // 현재 디렉토리를 열기
   if ( NULL != dir_info)
   {
      while( dir_entry   = readdir( dir_info))  // 디렉토리 안에 있는 모든 파일과 디렉토리 출력
      {
         printf( "%s  ", dir_entry->d_name);
      }
      printf("\n");
      closedir( dir_info);
   }   
}
