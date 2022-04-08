#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8000
#define MAXLINE 1024
#define BLOCKSIZE 1024
#define SEQUENCENUM 8
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

struct File_not_found
{
    int type;
    int filename_size;
    char filename[MAXLINE];
};

struct ACK
{
    int type;
    int num_sequences;
    int sequence_no[SEQUENCENUM];
};
struct file_request
{
    int type;
    int size;
    char data[MAXLINE];
};

int main()
{

    char *buffer[50];
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    struct file_request filerequest;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi("8000"));
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    int len = sizeof(client_addr);

    recvfrom(sockfd, (struct struct_data *)&filerequest, sizeof(filerequest), MSG_WAITALL, (struct sockaddr *)&client_addr, &len);

    printf("%s", filerequest.data);
}
