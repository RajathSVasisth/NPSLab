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

int main(int argc, char * argv[])
{
    struct sockaddr_in addr;
    int fd,cnt;
    struct ip_mreq mreq;
    char message[] = "RVCE CSE";

    if((fd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("Socket creation failed\n");
        return -1;
    }
    printf("Socket created successfully\n");

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(HELLO_GROUP);
    addr.sin_port = htons(HELLO_PORT);

    while(1)
    {
        if(sendto(fd,message,sizeof(message),0,(struct sockaddr *)&addr,sizeof(addr))<0)
        {
            printf("Inside send to");
            perror("Sendto error\n");
            exit(1);
        }
        
        sleep(1);
    }

    return 0;
}
