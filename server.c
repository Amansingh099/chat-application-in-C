// server.c
#include <stdio.h>
#include <stdlib.h>
#include "server_utils.h" // Include the header file with function prototypes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // Provides access to POSIX operating system API functions like close

int main()
{
    int portno;

    // Prompt user for the port number
    printf("Enter port number: ");
    scanf("%d", &portno); // Read the port number from the user

    if (portno <= 0)
    {
        fprintf(stderr, "Invalid port number. Program terminated.\n");
        exit(1);
    }

    int sockfd = setup_server_socket(portno); // Set up the server socket

    struct sockaddr_in cli_addr; // Client address struct
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // Accept client connection
    if (newsockfd < 0)
    {
        error("Error on Accept");
    }

    handle_client(newsockfd); // Handle communication with the client

    close(newsockfd);
    close(sockfd);
    return 0;
}
