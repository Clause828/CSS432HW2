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
#include <fstream>
#include <cstring>

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
const int PORT_NUMBER = 80;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << "Incorrect number of argument provided. Server address followed by File path. ";
    }
     
     server_address = argv[1];
     web_file = argv[2]; 


    //Retrieve a hostent structure
	int socketFD = settingUpSocket(argv);
    if(socketFD == 0)
    {
        cout << "Error! socket failed." << endl; 
        return -1;
    } 

    return callGetRequest(socketFD);
}

/**
 * Used for setting up the socket 
 * @param *argv[]
 * */
int settingUpSocket(char* argv[]){
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
	sendSockAddr.sin_port = htons(PORT_NUMBER); //default uotation

	// 1) Open a new socket and establish a connection to a server.
	int socketFD = socket(AF_INET, SOCK_STREAM, 0);

	if(socketFD == 0)
	{	
		std::cout << "Error! Socket failed. " << std::endl;
		return -1;
	}

	//connect to the server
	int socketConnect = connect(socketFD, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if (socketConnect == -1)
    {
        cout << "Invalid socket connection." << endl;
    }
    return socketFD;
}
/**
 * Processing the GetRequest HTTP
 *  
 **/
int callGetRequest(int socketFD)
{   
    //having the get function taken 
    string request = string("GET " + string(FILENAME) + " HTTP/1.1\r\n" +
                                "Host: " + string(server_address) + "\r\n" +
                                "\r\n"); // a get request is ended with a \r\n\r\n

    int sendRequest = send(socketFD, request.c_str(), strlen(request.c_str()), 0);
    int bufSize = 0;
    while (true)
    {
        string responseHeader = parseResponseHeader(socketFD);
        if(responseHeader.substr(0,15) == "Content-Length:" )
        {
            bufSize = atoi(responseHeader.substr(
                16, responseHeader.length()).c_str());
                // Parse the number of byte that will be in the body of the message
        }
    }
    // 6) Receive from the server an integer acknowledgement.
    ofstream outputFile;
    outputFile.open(FILENAME);
    //create a databuffer
   	char databuf[bufSize];	
    //receieve the file information
    recv(socketFD, &databuf, bufSize, 0);
    // 2) Allocate databuf[nbufs][bufsize] if the sizes are the same.
    for(int i = 0; i < databuf[bufSize]; i++)
    {
        outputFile << databuf[i];
    }
    outputFile.close();
    close(socketFD);
    cout << "Finished Writing HTTP file to the output file" << endl;
    return 0;
}
/**
 * Parsing the Response back so that we can get the relevant information
 * */
string parseResponseHeader(int socketFD){
    string responseHeader = "";
    char lastChar = 0;
    while(true)
    {
        char currentChar = 0;
        recv(socketFD, &currentChar, 1, 0);
        if(currentChar == '\r' || currentChar == '\n')
            if(lastChar == '\r' || lastChar == '\n')
                break;
        else
        {
            lastChar += currentChar;
            responseHeader += currentChar;
        }
        return responseHeader;
    }
}