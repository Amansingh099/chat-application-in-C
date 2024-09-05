#!/bin/bash

# Compile the client code
gcc -c -o client/clientsidecompile client/clientside.c
ar -rcs client/lib_client_utils.a client/clientsidecompile
gcc -o clientcompile client/client.c -L. client/lib_client_utils.a

echo "client code compiled successfully. You can now run it using ./clientcompile"
