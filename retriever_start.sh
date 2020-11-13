#!/bin/bash
#define ip and file/webpage
ip=localhost
wp=/

# compile retriever
g++ -o retriever.out ./Retriever.cpp

# run retriever
./retriever.out $ip $wp