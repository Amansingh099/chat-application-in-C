#include "client_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

void *receive_messages(void *socket_desc);
void *send_messages(void *socket_desc);

int main()
{
    int sockfd, portno;
    char hostname[BUFFER_SIZE];
    char name[BUFFER_SIZE];

    printf("Enter server Ip: ");
    fgets(hostname, BUFFER_SIZE, stdin);
    hostname[strcspn(hostname, "\n")] = 0; // Remove newline character

    printf("Enter port number: ");
    scanf("%d", &portno);
    getchar(); // Consume the newline left by scanf

    if (portno <= 0)
    {
        fprintf(stderr, "Invalid port number. Program terminated.\n");
        exit(1);
    }

    // Connect to the server
    sockfd = connect_to_server(hostname, portno);

    // Wait for the server's name prompt
    char server_prompt[BUFFER_SIZE];
    int n = read(sockfd, server_prompt, BUFFER_SIZE - 1);
    if (n > 0)
    {
        server_prompt[n] = 0; // Null-terminate the message
        printf("%s", server_prompt);
    }

    // Read the name from user input
    fgets(name, BUFFER_SIZE, stdin);
    name[strcspn(name, "\n")] = 0; // Remove newline character

    // Send the name to the server
    if (write(sockfd, name, strlen(name)) < 0)
    {
        error("Error sending name to server");
    }

    printf("Connected to the server. Type 'bye' to exit.\n");

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
