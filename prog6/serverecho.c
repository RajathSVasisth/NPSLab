#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#define SERV_PORT 9002
#define LISTENQ 1024
void str_echo(int sockfd)
{
	struct sockaddr_in client;
	socklen_t sockLen;
	ssize_t n;
	char address[100];
	char buf[1000];

again:

	while ((n = read(sockfd, buf, 1000)) > 0)
	{
		
		getpeername(sockfd,(struct sockaddr *)&client,&sockLen);
		printf("From client: address: %s port: %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
		printf("Client: %s\n",buf);
		if(strcmp(buf,"exit\n")==0)
		{
			exit(0);
		}
		write(sockfd, buf, n);
		memset(buf, '\0', strlen(buf));
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		perror("str_echo: read error");
}

int main(int argc, char **argv)
{
	int listenfd, connfd;
	id_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (!listenfd)
	{
		printf("socket creation failed\n");
		return -1;
	}
	printf("socket creation successful\n");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Bind failed\n");
		return -1;
	}
	printf("Bind successful\n");

	listen(listenfd, LISTENQ);
	printf("Server listening....\n");

	for (;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		if ((childpid = fork()) == 0)
		{					  /* child process */
			close(listenfd);  /* close listening socket */
			str_echo(connfd); /* process the request */
			exit(0);
		}
		close(connfd); /* parent closes connected socket */
	}
}
