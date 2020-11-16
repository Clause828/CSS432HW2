#!/bin/bash
# define ip and file/webpage
#ip=localhost
ip=64.227.48.38

# compile retriever
g++ -o retriever ./Retriever.cpp

# start retriever
# 401 unauthorized
./retriever $ip SecretFile.html
# 200 ok
./retriever $ip content.txt
# 403 forbidden
./retriever $ip ../file.html
# 200 ok
./retriever $ip /content.txt
# 404 not found
./retriever $ip ././/hjklh

