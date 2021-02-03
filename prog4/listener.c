#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HELLO_PORT 12345
#define HELLO_GROUP "235.0.0.0"
#define MESSAGE_BUF_SIZE 25

int main(int argc, char *argv[])
{
    struct sockaddr_in addr;
    int fd,nbytes,addrlen;
    struct ip_mreq mreq;
    char msg[MESSAGE_BUF_SIZE];

    u_int yes=1;

    if((fd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("Socket creation failed\n");
        return -1;
    }
    printf("Socket created successfully\n");

    if(setsockopt(fd,SOL_SOCKET,SO_REUSEPORT,&yes,sizeof(yes))<0)
    {
        perror("Reuse port failed\n");
        exit(-1);
    }
    printf("Reuse port successful\n");


    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(HELLO_PORT);

    if(bind(fd,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
        perror("Bind failed\n");
        exit(-1);
    }
    printf("Bind successful\n");

    mreq.imr_multiaddr.s_addr = inet_addr(HELLO_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(setsockopt(fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0)
    {
        perror("Setsockopt error\n");
        exit(-1);
    }
    printf("Socket option set\n");

    while(1)
    {
        addrlen = sizeof(addr);
        if((nbytes=recvfrom(fd,msg,MESSAGE_BUF_SIZE,0,(struct sockaddr *)&addr,&addrlen))<0)
        {
            perror("Recvfrom error\n");            
            exit(1);
        }
        puts(msg);
        
    }
    return 0;
}

