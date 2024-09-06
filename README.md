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
│   └── libclientutils.a          # Client utilities static library
├── src/                          # Source files
│   ├── server.c                  # Main server source file
│   ├── client.c                  # Main client source file
│   ├── server_utils.c            # Server utilities source file
│   └── client_utils.c            # Client utilities source file
├── bin/                          # Compiled executables
│   ├── server                    # Server executable
│   └── client                    # Client executable
└── run_chat_app.sh 
