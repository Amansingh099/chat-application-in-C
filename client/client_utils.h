// client_utils.h
#include <pthread.h> // For pthread_t type and threading functions
#define BUFFER_SIZE 255

void error(const char *msg);
void *send_messages(void *sockfd);
void *receive_messages(void *sockfd);
int connect_to_server(const char *hostname, int portno);