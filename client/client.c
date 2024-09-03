// client_utils.c
#include "client_utils.h" // Include your own header file

// Include additional headers needed for function implementations
#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit, malloc, free
#include <string.h>     // For memset, strlen, strncmp, etc.
#include <unistd.h>     // For close, read, write functions
#include <arpa/inet.h>  // For inet_pton, sockaddr_in structures
#include <sys/socket.h> // For socket-related functions

int main()
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char hostname[BUFFER_SIZE];
    char name[BUFFER_SIZE];

    printf("Enter hostname: ");
    fgets(hostname, BUFFER_SIZE, stdin);
    hostname[strcspn(hostname, "\n")] = 0; // Remove newline character

    printf("Enter port number: ");
    scanf("%d", &portno);

    if (portno <= 0)
    {
        fprintf(stderr, "Invalid port number. Program terminated.\n");
        exit(1);
    }

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

    // Prompt for the user's name and send it to the server
    printf("Enter your name: ");
    getchar(); // To consume the newline character left by scanf
    fgets(name, BUFFER_SIZE, stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline character

    // Send the name to the server
    if (write(sockfd, name, strlen(name)) < 0)
    {
        error("Error sending name to server");
    }

    printf("Connected to the server as %s. Type 'bye' to exit.\n", name);

    // Create threads for sending and receiving messages
    pthread_t send_thread, receive_thread;
    pthread_create(&send_thread, NULL, send_messages, &sockfd);
    pthread_create(&receive_thread, NULL, receive_messages, &sockfd);

    // Wait for both threads to finish
    pthread_join(send_thread, NULL);
    pthread_join(receive_thread, NULL);

    close(sockfd);
    return 0;
}
