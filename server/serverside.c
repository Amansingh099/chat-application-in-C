
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

int handle_client_name(int sockfd, int client_index)
{
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);

    // Send the name prompt to the client
    const char *name_prompt = "Enter your name: ";
    send(sockfd, name_prompt, strlen(name_prompt), 0);

    // Read the client's name
    int n = read(sockfd, buffer, BUFFER_SIZE - 1);
    if (n <= 0 || strlen(buffer) == 0)
    {
        char *error_msg = "Invalid name received. Please reconnect and provide a valid name.\n";
        write(sockfd, error_msg, strlen(error_msg));
        printf("Client disconnected before sending a valid name\n");
        return 0;
    }

    // Remove newline character from the name if present
    buffer[strcspn(buffer, "\n")] = 0;

    // Store the client's name
    pthread_mutex_lock(&clients_mutex);
    strcpy(client_names[client_index], buffer);
    pthread_mutex_unlock(&clients_mutex);

    printf("Client connected with name: %s\n", buffer);

    // Send a welcome message to the client
    char welcome_msg[BUFFER_SIZE];
    send(sockfd, welcome_msg, strlen(welcome_msg), 0);

    return 1;
}
// Function to handle communication with the client
void *client_handler(void *socket_desc)
{
    int sockfd = *(int *)socket_desc;
    free(socket_desc);
    // Handle the client's name
    pthread_mutex_lock(&clients_mutex);
    int client_index = num_clients - 1;
    pthread_mutex_unlock(&clients_mutex);

    if (!handle_client_name(sockfd, client_index))
    {
        // If the client name is invalid or if the client disconnects, exit the handler
        close(sockfd);
        pthread_exit(NULL);
    }

    char buffer[BUFFER_SIZE];
    int n;

    // Main loop for handling client messages
    while (1)
    {
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE);
        if (n <= 0)
        {
            printf("Client %s disconnected\n", client_names[client_index]);
            break;
        }

        if (strncmp(buffer, "bye", 3) == 0)
        {
            printf("Client %s disconnected\n", client_names[client_index]);
            break;
        }

        char message[1058];
        snprintf(message, sizeof(message), "%s: %s", client_names[client_index], buffer);
        printf("Received from %s: %s", client_names[client_index], buffer);

        broadcast_message(message, sockfd);
    }

    // Remove client from the list
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
    pthread_exit(NULL);
}