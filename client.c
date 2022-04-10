// Au1940112 kandarp sharda
// AU1940064 Rushabh shah
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define MAXLINE 1024
#define BLOCKSIZE 1024
#define SEQUENCENUM 8
struct file_request
{
    int type;
    int size;
    char data[MAXLINE];
};

struct File_info_and_data
{
    int type;
    long long sequence_number;
    int filename_size;
    char filename[MAXLINE];
    long long file_size;
    long long block_size;
    char data[BLOCKSIZE];
};
struct ACK
{
    int type;
    int num_sequences;
    int sequence_no[SEQUENCENUM];
};
struct File_not_found
{
    int type;
    int filename_size;
    char filename[MAXLINE];
};

struct Data
{
    int type;
    int sequence_number;
    long long  block_size;
    char data[BLOCKSIZE];


};

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        perror("PLease enter the IP address and port number\n");
        exit(EXIT_FAILURE);
    }

    // condition checking variable
    int recvChecking; // recvChecking is used to check the recv function

    // all local struct defining
    struct file_request filerequest;
    struct File_info_and_data fileFirstData;
    struct sockaddr_in client_addr;

    // storing variable
    char buffer[MAXLINE];
    int len = sizeof(client_addr);

    // taking user input
    fgets(buffer, MAXLINE, stdin);

    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(atoi(argv[1]));
    client_addr.sin_addr.s_addr = inet_addr(argv[2]);

    filerequest.type = 1;

    buffer[MAXLINE - 1] = '\0';
    filerequest.size = strlen(buffer);
    strcpy(filerequest.data, buffer);
    sendto(sockfd, (void *)&filerequest, sizeof(filerequest), MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));

    while (1)
    {
     recvfrom(sockfd, (struct File_info_and_data *)&fileFirstData, sizeof(fileFirstData), MSG_WAITALL, (struct sockaddr *)&client_addr, &len);
     printf("%s", fileFirstData.data);
     if (strcmp("EOF", fileFirstData.data)==0)
     {
         break;
     }
    }
}