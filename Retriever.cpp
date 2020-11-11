// Created by Duncan Spani and Jayden Spitek
// 2020/11/11

#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <sys/time.h>	 //for gettimeofday()
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include <netdb.h>        // gethostbyname
#include <unistd.h>       // read, write, close
#include <strings.h>      // bzero
#include <sys/uio.h>      // writev
#include <cstdlib>
#include <iostream>

/*Retriever
Your retriever takes in an input from the command line and parses the server address and file (web page) that is being requested.
The program then issues a GET request to the server for the requested file.
When the file is returned by the server, the retriever outputs the file to the screen and saves the retrieved file to the file system.
If the server returns an error code instead of an OK code, then the retriever should not save the file and should display on the screen whatever error page was sent with the error.
Your retriever should exit after receiving the response.
The server name can be either an IP address for simplicity. */

using namespace std; 
string FILENAME = "content.txt"; 
char *server_address;
char *web_file;

int main(int argc, char* argv[])
{
     server_address = argv[1];
     web_file = argv[2]; 


}