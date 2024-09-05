// client_utils.c
#include "client_utils.h" // Include your own header file

#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit
#include <string.h>     // For memset, strlen, etc.
#include <unistd.h>     // For close, read, write functions
#include <arpa/inet.h>  // For inet_pton, sockaddr_in structures
#include <sys/socket.h> // For socket-related functions

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int connect_to_server(const char *hostname, int portno)
{
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if (inet_pton(AF_INET, hostname, &serv_addr.sin_addr) <= 0)
    {
        error("Invalid address or address not supported");
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection failed");
    }

    return sockfd;
}

void *send_messages(void *sockfd)
{
    int socket = *(int *)sockfd;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        printf("-> ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (write(socket, buffer, strlen(buffer)) < 0)
        {
            error("Error on writing");
        }

        if (strncmp("bye", buffer, 3) == 0)
        {
            break; // Exit if the user types 'bye'
        }
    }

    close(socket);
    pthread_exit(NULL); // Exit the thread
}

void *receive_messages(void *sockfd)
{
    int socket = *(int *)sockfd;
    char buffer[BUFFER_SIZE];

    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        int n = read(socket, buffer, BUFFER_SIZE);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break; // Exit loop if the server disconnects
        }

        printf("%s\n", buffer);
    }

    close(socket);
    pthread_exit(NULL); // Exit the thread
}