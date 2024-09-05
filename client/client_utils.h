// client_utils.h
// Standard library headers for data types and functions used in declarations
#include <pthread.h> // For pthread_t type and threading functions
#define BUFFER_SIZE 255
// Function prototypes
void error(const char *msg);
void *send_messages(void *sockfd);
void *receive_messages(void *sockfd);
int connect_to_server(const char *hostname, int portno);