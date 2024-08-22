#include <stdio.h>
#include <stdlib.h>     //provides function for memory allocation
#include <string.h>     //string operations
#include <unistd.h>     //inlcuded for sytem calls like read write etc.
#include <sys/types.h>  //contains data types needed for system calls.
#include <sys/socket.h> //contains number of function needed for socket Api.
#include <netinet/in.h> //contains structure for internet domain address

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Port No not provided. Program terminated\n"); // prints the error message in the stderr stream which is used for error displaying
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr; // gives the internet address. it is provided by netinet header file

    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET-IPV4,SOCK_STREAM-TCP,DEFAULT PROTOCOL FOR TCP

    if (sockfd < 0)
    {
        error("Error opening Socket");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));

    // bzero is used to zero out the memory occupied by the serv_addr structure
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // it allows the server to bind to any ip address on this machine
    serv_addr.sin_port = htons(portno);
    int bindresponse = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); // assigning address to the socket
    if (bindresponse < 0)
    {
        error("Binding Failed");
    }
    listen(sockfd, 5); // 5 indicates the no of connection that can connect to server at a time

    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
    {
        error("Error on Accept");
    }
    
    //using a while loop for message exchange. bye for quiting the chat
    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("Error on reading");
        printf("Client : %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            error("Error on Writting");
        }
        int i = strncmp("bye", buffer, 3);
        if (i == 0)
            break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}