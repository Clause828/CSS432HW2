#!/bin/bash

# compile server
g++ -o server ./Server.cpp -l pthread

# start server
./server
echo "Server running"
