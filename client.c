#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8000
#define MAXLINE 1024

struct struct_data
{  int type;
    int size;
    char data[MAXLINE];
};


int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        perror("PLease enter the IP address and port number\n");
        exit(EXIT_FAILURE);
    }

    char buffer[MAXLINE];
    struct struct_data firsttype ;
   
    fgets(buffer, MAXLINE, stdin);
    
    
    
    int sizing   = sizeof(firsttype.data);

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_addr;

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(atoi(argv[1]));
    client_addr.sin_addr.s_addr = inet_addr(argv[2]);
    
   firsttype.type = 1;
   
   buffer[MAXLINE -1] = '\0';
   firsttype.size = strlen(buffer);
   strcpy(firsttype.data, buffer);
    sendto(sockfd, (void *) &firsttype ,sizeof(firsttype), MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));
}