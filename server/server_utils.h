
#include <pthread.h>

#define MAX_CLIENTS 100 // Maximum number of clients
#define BUFFER_SIZE 255 // Buffer size for messages

// Extern declarations of global variables
extern int client_sockets[MAX_CLIENTS];             // Array to store client socket descriptors
extern char client_names[MAX_CLIENTS][BUFFER_SIZE]; // Array to store client names
extern int num_clients;                             // Counter for the number of clients
extern pthread_mutex_t clients_mutex;               // Mutex for thread synchronization

// Function prototypes
void error(const char *msg);
int setup_server_socket(int portno);
void *client_handler(void *socket_desc);
void broadcast_message(char *message, int sender_socket);

