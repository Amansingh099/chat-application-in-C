#include <stdio.h>
#include <string.h>
#include "connect_to_server.h"

int main()
{
    char hostname[255];
    int portno;

    // Ask user for hostname
    printf("Enter hostname: ");
    fgets(hostname, 255, stdin);
    hostname[strcspn(hostname, "\n")] = 0; // Remove the newline character

    // Ask user for port number
    printf("Enter port number: ");
    scanf("%d", &portno);

    // Call the function from the static library
    connect_to_server(hostname, portno);

    return 0;
}
