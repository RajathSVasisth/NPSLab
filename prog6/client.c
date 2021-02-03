#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define MAXLINE 1000
#define SERV_PORT 8080

void str_cli(FILE *fp, int sockfd)
{
  int n;
  char sendline[MAXLINE], recvline[MAXLINE];
  printf("Me: ");
  while (fgets(sendline, MAXLINE, fp))
  {
    bzero(recvline,MAXLINE);
    write(sockfd, sendline, strlen(sendline));
    if (strcmp(sendline, "exit\n") == 0)
    {
      exit(0);
    }
    bzero(recvline,MAXLINE);
    printf("From server: ");
    n=read(sockfd, recvline, MAXLINE);
    if (n==0)
    {
      perror("str_cli: server terminated prematurely");
      exit(EXIT_FAILURE);
    }
    recvline[n]='\0';
    printf("%s",recvline);
    printf("\n");
    printf("Me: ");
    bzero(sendline,MAXLINE);
  }
}

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2)
  {
    perror("usage: tcpcli <IPaddress>");
    exit(EXIT_FAILURE);
  }
  printf("Socket created successfully\n");

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
  {
    perror("connect failed\n");
    exit(0);
  }
  printf("Connected successfully\n");
  str_cli(stdin, sockfd); /* do it all */
  exit(0);
}
