#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#define SERV_PORT 9002
#define MAXLINE 1024

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{

	int n;
	struct sockaddr_in client;
	socklen_t len;

	char mesg[MAXLINE];

	for (;;)
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		getpeername(sockfd,(struct sockaddr *)&client,&len);

		printf("From client %s:%d\n",inet_ntoa(client.sin_addr),client.sin_port);
		printf("%s\n",mesg);
		sendto(sockfd, mesg, strlen(mesg), 0, pcliaddr, len);
		bzero(mesg,sizeof(mesg));
	}
}

int main(int argc, char **argv)
{

	int sockfd;

	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(!sockfd)
	{
		printf("socket creation failed\n");
		return -1;
	}
	printf("socket creation successful\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)
	{
		printf("Bind failed\n");
		return -1;
	}
	printf("Bind successful\n");

	dg_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
}
