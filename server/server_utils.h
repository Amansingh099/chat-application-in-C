void error(const char *msg);
int setup_server_socket(int portno);
void handle_client(int newsockfd);