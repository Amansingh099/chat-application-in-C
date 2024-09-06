#include "client_utils.h" // Include your own header file for client utility functions
#include <stdio.h>      // For standard input/output functions like printf and perror
#include <stdlib.h>     // For standard library functions like exit
#include <string.h>     // For string manipulation functions like memset, strlen
#include <unistd.h>     // For POSIX API functions like close, read, write
#include <arpa/inet.h>  // For IP address conversion and socket address structures
#include <sys/socket.h> // For socket creation and connection-related functions
#include <pthread.h>    // For threading functions and data structures

// Function to handle errors by printing an error message and exiting the program
void error(const char *msg)
{
    perror(msg); // Print the error message along with the reason
    exit(1);     // Exit the program with an error status code
}

// Function to connect to the server using hostname and port number
int connect_to_server(const char *hostname, int portno)
{
    int sockfd;                   // Socket file descriptor
    struct sockaddr_in serv_addr; // Server address structure

    // Create a socket with IPv4 addressing and TCP protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error opening socket"); // Handle socket creation errors
    }

    // Zero out the server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;     // Set address family to IPv4
    serv_addr.sin_port = htons(portno); // Convert port number to network byte order

    // Convert hostname (IP address) from text to binary form and set it in the server address
    if (inet_pton(AF_INET, hostname, &serv_addr.sin_addr) <= 0)
    {
        error("Invalid address or address not supported"); // Handle invalid IP addresses
    }

    // Connect to the server using the address structure
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Connection failed"); // Handle connection errors
    }

    return sockfd; // Return the socket file descriptor on successful connection
}

// Thread function to handle sending messages to the server
void *send_messages(void *sockfd)
{
    int socket = *(int *)sockfd; // Extract socket file descriptor from argument
    char buffer[BUFFER_SIZE];    // Buffer to hold outgoing messages

    while (1)
    {
        bzero(buffer, BUFFER_SIZE); // Clear the buffer
        printf("-> ");
        fgets(buffer, BUFFER_SIZE, stdin); // Read user input from stdin

        // Send the user's message to the server
        if (write(socket, buffer, strlen(buffer)) < 0)
        {
            error("Error on writing"); // Handle write errors
        }

        // Check if the message is 'bye' to initiate client disconnection
        if (strncmp("bye", buffer, 3) == 0)
        {
            printf("Successfully disconnected\n");
            exit(1); // Exit the program completely if 'bye' is typed
            break;   // Break the loop if 'bye' is detected (though redundant here)
        }
    }

    close(socket);      // Close the socket after breaking the loop
    pthread_exit(NULL); // Exit the thread gracefully
}

// Thread function to handle receiving messages from the server
void *receive_messages(void *sockfd)
{
    int socket = *(int *)sockfd; // Extract socket file descriptor from argument
    char buffer[BUFFER_SIZE];    // Buffer to hold incoming messages

    while (1)
    {
        bzero(buffer, BUFFER_SIZE);                // Clear the buffer before reading
        int n = read(socket, buffer, BUFFER_SIZE); // Read message from server

        // Check if the server has disconnected or if there was an error
        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break; // Exit loop if server disconnects or error occurs
        }

        printf("%s\n", buffer); // Print the received message
    }

    close(socket);      // Close the socket when the server disconnects
    pthread_exit(NULL); // Exit the thread gracefully
}
