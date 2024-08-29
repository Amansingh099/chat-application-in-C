#include <stdio.h>
#include <stdlib.h>     // Provides functions for memory allocation, conversion, etc.
#include <string.h>     // Provides string manipulation functions
#include <unistd.h>     // Provides access to POSIX operating system API functions like read, write, etc.
#include <sys/types.h>  // Contains definitions for data types used in system calls
#include <sys/socket.h> // Contains definitions for socket functions and data structures
#include <netinet/in.h> // Contains structures for storing addresses (Internet domain address)
#include <netdb.h>      // Provides the `hostent` structure used to store information about a host (hostname or IPv4 address)

void error(const char *msg)
{
    perror(msg); 
    exit(1);     
}

void connect_to_server(const char *hostname, int portno)
{
    int sockfd, n;                
    struct sockaddr_in serv_addr; 
    struct hostent *server;       // Pointer to hostent structure which will hold information about the server
    char buffer[255];          

    // Create a socket using IPv4 (AF_INET) and TCP (SOCK_STREAM)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) // Check if socket creation failed
    {
        error("Error opening socket"); 
    }

    // Get the server information by hostname
    server = gethostbyname(hostname);
    if (!server) // Check if server is found
    {
        fprintf(stderr, "Error, no such host\n");
        exit(1); // Exit the program if server is not found
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // Set the address family to IPv4
    // Copy the server's IP address into the server address structure
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno); // Set the port number, converting to network byte order

    // Connect to the server using the socket
    int connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectfd < 0) 
    {
        error("Connection Failed"); 
    }

    while (1)
    {
        bzero(buffer, 255);        // Clear the buffer
        fgets(buffer, 255, stdin); // Read input from the user

        // Send the input to the server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) // Check if there was an error writing to the socket
        {
            error("Error on writing"); // Print error message and exit
        }

        bzero(buffer, 255);            // Clear the buffer
        n = read(sockfd, buffer, 255); // Read the response from the server
        if (n < 0)                     // Check if there was an error reading from the socket
        {
            error("Error on reading"); // Print error message and exit
        }
        printf("Server: %s\n", buffer); // Print the server's response

        // Check if the message from the server is "bye"
        if (strncmp("bye", buffer, 3) == 0)
        {
            break; // Break the loop to terminate the connection
        }
    }

    close(sockfd); // Close the socket after communication ends
}
