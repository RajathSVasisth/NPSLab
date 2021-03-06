#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if((server_fd=socket(AF_INET,SOCK_STREAM,0))==0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("\nSocket created successfully\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd,(struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("\nBinded successfully\n");
    if(listen(server_fd,3)<0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("\nListen successful\n");
    if((new_socket=accept(server_fd,(struct sockaddr *)&address,(socklen_t *)&addrlen))<0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("\nAccept successful\n");
    printf("Server listening for other connections\n");

    while(1)
    {
        valread = read(new_socket,buffer,1024);
        buffer[valread]='\0';
        if(strcmp(buffer,"exit")==0)
            break;
        printf("\n%s: command received from client\n",buffer);    
        system(buffer);        
        bzero(&buffer,sizeof(buffer));

    }

    close(new_socket);
    close(server_fd);

    return 0;
   
}