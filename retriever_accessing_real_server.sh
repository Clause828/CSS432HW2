#!/bin/bash
# define ip and file/webpage
ip=www.washington.edu/

# compile retriever
g++ -o retriever ./Retriever.cpp

# start retriever
# 200 ok
./retriever $ip research.html
