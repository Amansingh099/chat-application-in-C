// client_utils.c
#include "client_utils.h" // Include your own header file
// Include additional headers needed for function implementations
#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit, malloc, free
#include <string.h>     // For memset, strlen, strncmp, etc.
#include <unistd.h>     // For close, read, write functions
#include <arpa/inet.h>  // For inet_pton, sockaddr_in structures
#include <sys/socket.h> // For socket-related functions

void error(const char *msg)
{
    perror(msg);
    exit(1);
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
