
#include <stdio.h>
#include <stdlib.h>     // Provides functions for memory allocation
#include <string.h>     // String operations
#include <unistd.h>     // Included for system calls like read, write, etc.
#include <sys/types.h>  // Contains data types needed for system calls
#include <sys/socket.h> // Contains functions needed for socket API
#include <netinet/in.h> // Contains structures for internet domain addresses
#include <pthread.h>
#include "server_utils.h" // Include the header file

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

// Function to send a message to all clients except the sender
void broadcast_message(char *message, int sender_socket)
{
    pthread_mutex_lock(&clients_mutex); // Lock the mutex before accessing shared resources

    for (int i = 0; i < num_clients; i++)
    {
        if (client_sockets[i] != sender_socket)
        { // Send to all clients except the sender
            if (send(client_sockets[i], message, strlen(message), 0) < 0)
            {
                perror("Error sending message to client");
            }
        }
    }

    pthread_mutex_unlock(&clients_mutex); // Unlock the mutex after accessing shared resources
}

// Function to handle communication with the client
void *client_handler(void *socket_desc)
{
    int sockfd = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int n;

    // Ask for the client's name
    bzero(buffer, BUFFER_SIZE);
    n = write(sockfd, "Enter your name: ", 17);
    if (n < 0)
        error("Error writing to socket");

    // Read the client's name
    bzero(buffer, BUFFER_SIZE);
    n = read(sockfd, buffer, BUFFER_SIZE - 1); // Read name from the client
    if (n <= 0)
    {
        printf("Client disconnected before sending name\n");
        close(sockfd);
        free(socket_desc);
        pthread_exit(NULL);
    }

    // Remove newline character from the name
    buffer[strcspn(buffer, "\n")] = 0;

    // Store the client's name
    pthread_mutex_lock(&clients_mutex);
    strcpy(client_names[num_clients - 1], buffer);
    pthread_mutex_unlock(&clients_mutex);

    printf("Client connected with name: %s\n", buffer);

    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE); // Read from the client
        if (n <= 0)
        {
            printf("Client %s disconnected\n", client_names[num_clients - 1]);
            break;
        }

        // Format the message with the client's name
        char message[1058];
        snprintf(message, sizeof(message), "%s: %s", client_names[num_clients - 1], buffer);
        printf("Received from %s: %s", client_names[num_clients - 1], buffer);

        // Broadcast the message to all other clients
        broadcast_message(message, sockfd);
    }

    // Remove the client from the list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < num_clients; i++)
    {
        if (client_sockets[i] == sockfd)
        {
            // Shift remaining clients in the list
            for (int j = i; j < num_clients - 1; j++)
            {
                client_sockets[j] = client_sockets[j + 1];
                strcpy(client_names[j], client_names[j + 1]);
            }
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(sockfd);
    free(socket_desc);
    pthread_exit(NULL);
}
