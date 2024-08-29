// server_utils.c
#include <stdio.h>
#include <stdlib.h>     // Provides functions for memory allocation
#include <string.h>     // String operations
#include <unistd.h>     // Included for system calls like read, write, etc.
#include <sys/types.h>  // Contains data types needed for system calls
#include <sys/socket.h> // Contains functions needed for socket API
#include <netinet/in.h> // Contains structures for internet domain addresses

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Function to set up and return the server socket file descriptor
int setup_server_socket(int portno)
{
    int sockfd;
    struct sockaddr_in serv_addr; // Server address struct

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP/IP socket
    if (sockfd < 0)
    {
        error("Error opening Socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Allow connections from any IP address
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Binding Failed");
    }

    listen(sockfd, 5); // Listen for up to 5 connections
    return sockfd;
}

// Function to handle communication with the client
void handle_client(int newsockfd)
{
    char buffer[255];
    int n;

    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255); // Read from client
        if (n < 0)
            error("Error on reading");
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);                    // Read server input
        n = write(newsockfd, buffer, strlen(buffer)); // Write to client
        if (n < 0)
            error("Error on writing");

        if (strncmp("bye", buffer, 3) == 0)
            break; // Exit loop if "bye" is entered
    }
}
