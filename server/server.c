// server.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> // Include pthread for threading
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server_utils.h" // Include the header file with function prototypes

// Define global variables
int client_sockets[MAX_CLIENTS];             // Array to store client socket descriptors
char client_names[MAX_CLIENTS][BUFFER_SIZE]; // Array to store client names
int num_clients = 0;                         // Counter for the number of clients

// Mutex for thread synchronization
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    int portno;

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

    while (1)
    {
        int *newsockfd = malloc(sizeof(int)); // Allocate memory for client socket descriptor
        if (newsockfd == NULL)
        {
            perror("Could not allocate memory for socket descriptor");
            continue;
        }

        *newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); // Accept a client connection
        if (*newsockfd < 0)
        {
            perror("Error on accept");
            free(newsockfd);
            continue;
        }

        pthread_mutex_lock(&clients_mutex);         // Lock the mutex before modifying shared resources
        client_sockets[num_clients++] = *newsockfd; // Add new client socket to the list
        pthread_mutex_unlock(&clients_mutex);       // Unlock the mutex

        pthread_t client_thread;
        // Create a new thread for each client connection
        if (pthread_create(&client_thread, NULL, client_handler, (void *)newsockfd) < 0)
        {
            perror("Could not create thread");
            free(newsockfd);
            continue;
        }
        // Detach the thread to allow it to clean up resources upon completion
        
        pthread_detach(client_thread);
    }
    close(sockfd);

    return 0;
}
