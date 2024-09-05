// client_main.c
#include "client_utils.h" // Include the header file for static library functions

#include <stdio.h>   // For printf, scanf, fgets
#include <stdlib.h>  // For exit
#include <unistd.h>  // For close
#include <pthread.h> // For pthread_create, pthread_join
#include <string.h>  // For string operations like strcspn

int main()
{
    int sockfd, portno;
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

    // Connect to the server
    sockfd = connect_to_server(hostname, portno);

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