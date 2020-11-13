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
    if(argc != 3)
    {
        cout << "Incorrect number of argument provided. Server address folloed by File path. ";
    }
     
     server_address = argv[1];
     web_file = argv[2]; 

    //having the get function taken 
     string request = string("GET " + string(FILENAME) + " HTTP/1.1\r\n" +
                                "Host: " + string(server_address) + "\r\n" +
                                "\r\n"); // a get request is ended with a \r\n\r\n

    //Retrieve a hostent structure
	struct hostent* host = gethostbyname(server_address);
	if (host == NULL)
	{
		std::cout << "Error: HOSTNAME failed" << std::endl;
		return -1;
	}
    //Get the returning file
    sockaddr_in sendSockAddr;
	bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
	sendSockAddr.sin_family = AF_INET; // Address Family Internet
	sendSockAddr.sin_addr.s_addr =
		inet_addr(inet_ntoa(*(struct in_addr*)host->h_addr_list));
	sendSockAddr.sin_port = htons(80); //default uotation

    //Is there an error 

}