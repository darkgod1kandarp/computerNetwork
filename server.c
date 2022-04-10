// Au1940112 kandarp sharda
// AU1940064 Rushabh shah

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
struct Data
{
    int type;
    int sequence_number;
    long long block_size;
    char data[BLOCKSIZE];
};

int main()
{

    int sockfd;
    struct sockaddr_in server_addr, client_addr;

    // storing variable
    char buffer[MAXLINE];
    char filename[MAXLINE];
    int filenameLength;
    int fileSize;

    // all local struct defining
    struct file_request filerequest;
    struct File_info_and_data fileFirstData;

    // File pointer defining
    FILE *FILE1;

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

    recvfrom(sockfd, (struct file_request *)&filerequest, sizeof(filerequest), MSG_WAITALL, (struct sockaddr *)&client_addr, &len);

    // storing the file name in the filename variable
    strcpy(filename, filerequest.data);

    // storing the filename length in the filenameLength variable
    filenameLength = strlen(filename);

    // removing the newline character from the filename
    filename[filenameLength - 1] = '\0';

    // FILE1 is the file pointer to the file
    FILE1 = fopen(filename, "rb");

    // if the file is not founde
    if (FILE1 == NULL)
    {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    // fileSize finding and storing in the fileSize variable
    fseek(FILE1, 0, SEEK_END); // seek to end of file
    fileSize = ftell(FILE1);   // get current file pointer
    fseek(FILE1, 0, SEEK_SET); // seek back to beginning of file

    // reading and sending the file
    while (!feof(FILE1))
    {

        fread(buffer, sizeof(buffer), 1, FILE1);

        // storing the file data  and other data in the fileFirstData variable
        fileFirstData.type = 1;
        fileFirstData.sequence_number = 3;
        fileFirstData.filename_size = fileSize;
        fileFirstData.block_size = BLOCKSIZE;
        fileFirstData.file_size = fileSize;
        strcpy(fileFirstData.data, buffer);

        // sending the first block
        sendto(sockfd, (struct File_info_and_data *)&fileFirstData, sizeof(fileFirstData), MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));
    }
    strcpy(fileFirstData.data, "EOF");
    sendto(sockfd, (struct File_info_and_data *)&fileFirstData, sizeof(fileFirstData), MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));
}
