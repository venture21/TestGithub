#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
//#include <unistd.h>

#define MAXLINE  511

int tcp_connect(int af, char *servip, unsigned short port) {
	struct sockaddr_in servaddr;
	int  s;
	// 소켓 생성
	if ((s = socket(af, SOCK_STREAM, 0)) < 0)
		return -1;
	// 채팅 서버의 소켓주소 구조체 servaddr 초기화
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = af;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);

	// 연결요청
	if (connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr))
		< 0)
		return -1;
	return s;
}

int main(int argc, char *argv[])
{
	struct sockaddr_in server;
	struct stat obj;
	int sock;
	char bufmsg[MAXLINE];
	char buf[100], command[5], filename[MAXLINE], *f;
	char temp[20];
	int k, size, status;
	int filehandle;

	if (argc != 3) {
		printf("사용법 : %s server_ip port\n", argv[0]);
		exit(1);
	}

	sock = tcp_connect(AF_INET, argv[1], atoi(argv[2]));
	if (sock == -1) {
		printf("tcp_connect fail");
		exit(1);
	}

	while (1) {
		printf("\033[1;33m명령어 : get, put, pwd, ls, cd, quit\n");
		printf("\033[1;32mclient> ");
		fgets(bufmsg, MAXLINE, stdin); //명령어 입력
		fprintf(stderr, "\033[97m");   //글자색을 흰색으로 변경
		if (!strcmp(bufmsg, "get\n")) {//get명령어를 입력받았다면
			printf("다운로드할 파일 : ");
			scanf("%s", filename);       //파일 이름 입력
			fgets(temp, MAXLINE, stdin); //버퍼에 남은 엔터 제거
			strcpy(buf, "get ");
			strcat(buf, filename);
			send(sock, buf, 100, 0);         //명령어 전송
			recv(sock, &size, sizeof(int), 0);
			if (!size) {//파일이 없다면
				printf("파일이 없습니다\n");
				continue;
			}
			f = malloc(size);
			recv(sock, f, size, 0);
			while (1) {
				filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
				if (filehandle == -1) //같은 이름이 있다면 이름 끝에 _1 추가
					sprintf(filename + strlen(filename), "_1");
				else break;
			}
			write(filehandle, f, size, 0);
			close(filehandle);
			printf("다운로드 완료\n");//전송이 잘 되었다면
		}
		else if (!strcmp(bufmsg, "put\n")) {//put명령어를 입력받았다면
			printf("업로드할 파일 : ");
			scanf("%s", filename);       //파일 이름 입력
			fgets(temp, MAXLINE, stdin); //버퍼에 남은 엔터 제거
			filehandle = open(filename, O_RDONLY);
			if (filehandle == -1) {//파일이 없다면
				printf("파일이 없습니다.\n");
				continue;
			}
			strcpy(buf, "put ");
			strcat(buf, filename);
			send(sock, buf, 100, 0);
			stat(filename, &obj);
			size = obj.st_size;
			send(sock, &size, sizeof(int), 0);//명령어 전송
			sendfile(sock, filehandle, NULL, size);//파일 전송
			recv(sock, &status, sizeof(int), 0);
			if (status)//업로드가 잘 되었다면
				printf("업로드 완료\n");
			else
				printf("업로드 실패\n");
		}
		else if (!strcmp(bufmsg, "pwd\n")) {//pwd명령어를 입력받았다면
			strcpy(buf, "pwd");
			send(sock, buf, 100, 0);
			recv(sock, buf, 100, 0);
			printf("--The path of the Remote Directory--\n%s", buf);
		}
		else if (!strcmp(bufmsg, "ls\n")) {//ls명령어를 입력받았다면
			strcpy(buf, "ls");
			send(sock, buf, 100, 0);
			recv(sock, &size, sizeof(int), 0);
			f = malloc(size);
			recv(sock, f, size, 0);
			filehandle = creat("temp.txt", O_WRONLY);
			write(filehandle, f, size, 0);
			close(filehandle);
			printf("--The Remote Directory List--\n");
			system("cat temp.txt");	//현재 디렉토리의 파일 출력
		}
		else if (!strcmp(bufmsg, "cd\n")) {//cd명령어를 입력받았다면
			strcpy(buf, "cd ");
			printf("이동할 경로 이름 : ");
			scanf("%s", buf + 3);        //위치 입력
			fgets(temp, MAXLINE, stdin); //버퍼에 남은 엔터 제거
			send(sock, buf, 100, 0);     //명령어 전송
			recv(sock, &status, sizeof(int), 0);
			if (status)
				printf("경로 변경 완료\n");
			else
				printf("경로 변경 실패\n");
		}
		else if (!strcmp(bufmsg, "quit\n")) {//quit명령어를 입력받았다면
			strcpy(buf, "quit");
			send(sock, buf, 100, 0);
			recv(sock, &status, 100, 0);
			if (status) {
				printf("서버를 닫는 중..\n");
				exit(0);
			}
			printf("서버 닫기 실패\n");
		}
	}
}
