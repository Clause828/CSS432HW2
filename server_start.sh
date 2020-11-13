#!/bin/bash

# compile server
g++ -o server.out ./Server.cpp -l pthread

# start server
./server.out

echo "Server running"