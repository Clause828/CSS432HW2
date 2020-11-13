#!/bin/bash
# compile server and retriever
g++ -o retriever ./Retriever.cpp
g++ -o server ./Server.cpp -l pthread
