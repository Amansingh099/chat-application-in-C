#include "client_utils.h" // Include header for client utility functions
#include <stdio.h>        // For standard input/output functions
#include <stdlib.h>       // For memory management and program utilities
#include <unistd.h>       // For POSIX API, including close() and read()
#include <pthread.h>      // For POSIX threading
#include <string.h>       // For string manipulation functions like strlen()

// Function declarations for message handling threads
void *receive_messages(void *socket_desc); // Function to handle incoming messages from the server
void *send_messages(void *socket_desc);    // Function to handle outgoing messages to the server

int main()
{
    int sockfd, portno;         // Socket file descriptor and port number
    char hostname[BUFFER_SIZE]; // Buffer to hold the server IP address
    char name[BUFFER_SIZE];     // Buffer to hold the client's name

    // Prompt user for server IP address
    printf("Enter server IP: ");
    fgets(hostname, BUFFER_SIZE, stdin);   // Read server IP address from user input
    hostname[strcspn(hostname, "\n")] = 0; // Remove trailing newline character

    // Prompt user for port number
    printf("Enter port number: ");
    scanf("%d", &portno); // Read port number from user input
    getchar();            // Consume the newline left by scanf to avoid input issues in the next prompt

    // Validate port number to ensure it is a positive integer
    if (portno <= 0)
    {
        fprintf(stderr, "Invalid port number. Program terminated.\n");
        exit(1); // Exit the program with an error status
    }

    // Connect to the server using provided IP address and port number
    sockfd = connect_to_server(hostname, portno);

    // Wait for and display the server's name prompt
    char server_prompt[BUFFER_SIZE];                      // Buffer to hold server prompts or messages
    int n = read(sockfd, server_prompt, BUFFER_SIZE - 1); // Read server's initial prompt
    if (n > 0)
    {
        server_prompt[n] = 0;        // Null-terminate the prompt string
        printf("%s", server_prompt); // Display the prompt to the user
    }

    // Prompt user for their name and read it
    fgets(name, BUFFER_SIZE, stdin); // Read client's name from user input
    name[strcspn(name, "\n")] = 0;   // Remove trailing newline character from the name

    // Send the user's name to the server for registration
    if (write(sockfd, name, strlen(name)) < 0)
    {
        error("Error sending name to server"); // Handle potential write errors
    }

    printf("Connected to the server. Type 'bye' to exit.\n");

    // Create threads to handle sending and receiving messages concurrently
    pthread_t send_thread, receive_thread;                            // Thread identifiers for send and receive threads
    pthread_create(&send_thread, NULL, send_messages, &sockfd);       // Create send thread
    pthread_create(&receive_thread, NULL, receive_messages, &sockfd); // Create receive thread

    // Wait for both threads to complete before exiting
    pthread_join(send_thread, NULL);    // Wait for send thread to finish
    pthread_join(receive_thread, NULL); // Wait for receive thread to finish

    // Close the socket before exiting the program to release resources
    close(sockfd);
    return 0; // Exit the program successfully
}
