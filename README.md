# Chat Application

This project is a multi-client chat application implemented in C using socket programming and threading. The application consists of a server that handles multiple clients and a client program that connects to the server, allowing users to send and receive messages in real-time.

## Features

- Multi-client support using threads.
- Each client has a unique name and can send/receive messages from the server.
- Server broadcasts messages to all connected clients except the sender.
- Error handling and clean exit on disconnect or errors.

## Project Structure

├── include/                      # Header files
│   ├── server_utils.h            # Server utilities header
│   └── client_utils.h            # Client utilities header
├── lib/                          # Static libraries
│   ├── libserverutils.a          # Server utilities static library
│   └── lib_client_utils.a        # Client utilities static library
├── src/                          # Source files
│   ├── server.c                  # Main server source file
│   ├── client.c                  # Main client source file
│   ├── server_utils.c            # Server utilities source file
│   └── client_utils.c            # Client utilities source file
├── bin/                          # Compiled executables
│   ├── server                    # Server executable
│   └── client                    # Client executable
├── server.sh                     # Shell script for building and running the server
└── client.sh                     # Shell script for building and running the client





## overview

- **Server**: Listens for client connections, handles incoming messages, broadcasts messages to all connected clients, and allows disconnection of clients.
- **Client**: Connects to the server, sends and receives messages, and allows the user to disconnect gracefully.

## Getting Started

### Prerequisites

- **GCC**: GNU Compiler Collection
- **linux**: linux based operating system
- **pthread**: generaly present in linux based system 

### Running the Application

To compile the server and client, use the provided shell script:

Start the server by running server.sh. It will listen for incoming client connections on the specified port.

Start one or more clients by running client.sh. Each client will connect to the server using the server's IP address and port.

