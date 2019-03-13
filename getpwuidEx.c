#include <stdio.h>
#include <unistd.h>     // getuid
#include <pwd.h>        // getpwuid
#include <sys/types.h>  // uid_t
#include <grp.h>

int main()
{
   uid_t          user_id;
   struct passwd *user_pw;
   struct group *grp;

   user_id  = getuid();             // 사용자 아이디를 구하고
   printf("getuid=%d\n",user_id);
   user_pw  = getpwuid(user_id);   // 아이디로 사용자 정보 구하기
   grp = getgrgid(user_pw->pw_gid);
  // grp = getgrgid(user_id);
   printf( "group name     :%s\n", grp->gr_name);
   printf( "user name      :%s\n", user_pw->pw_name  );
   printf( "user id        :%d\n", user_pw->pw_uid   );
   printf( "group id       :%d\n", user_pw->pw_gid   );
   printf( "reall name     :%s\n", user_pw->pw_gecos );
   printf( "home directory :%s\n", user_pw->pw_dir   );
   printf( "shell program  :%s\n", user_pw->pw_shell );
   return 0;
}
