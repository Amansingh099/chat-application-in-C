#!/bin/bash

# Compile the server code
gcc -c -o server/servercompile server/serverside.c
ar -rcs server/lib_server_utils.a server/servercompile
gcc -o servercompile server/server.c -L. server/lib_server_utils.a

echo "Server compiled successfully. You can now run it using ./servercompile"
