#!/bin/bash
# define ip and file/webpage
#ip=localhost
ip=64.227.48.38
wp=SecretFile.html

# compile retriever
g++ -o retriever ./Retriever.cpp

# start retriever
./retriever $ip $wp
