#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#define PORT 8080

int main(int argc, char *argv[])
{
    int sock = 0,valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    

    if(argc!=3)
    {
        printf("\nThe usage is ./client 127.0.0.1 8080\n");
        return -1;
    }
    char *address_string = argv[1];
    int port = atoi(argv[2]);
    
    char buffer[1024] = {0};
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("\n Socket creation failed\n");
        return -1;
    }
    printf("Socket created successfully\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET,address_string,&serv_addr.sin_addr)<=0)
    {
        printf("\n Invalid address\n");
        return -1;
    }

    if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        printf("\nConnection failed\n");
        return -1;
    }
    printf("Connected successfully....\n");

    send(sock,hello,strlen(hello),0);
    printf("\nHello message sent from client\n");
    valread = read(sock,buffer,1024);
    printf("\n%s\n",buffer);
    return 0;
}