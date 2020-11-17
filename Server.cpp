// Created by Duncan Spani and Jayden Spitek
// 2020/10/23

#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <sys/time.h>	  //for gettimeofday()
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include <netdb.h>        // gethostbyname
#include <unistd.h>       // read, write, close
#include <strings.h>      // bzero
#include <sys/uio.h>      // writev
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cstring>
#include <fstream>

using namespace std;

const string OK = "HTTP/1.1 200 OK\r\n";
const string DOES_NOT_EXIST = "HTTP/1.1 404 Not Found\r\n";
const string UNAUTHORIZED = "HTTP/1.1 401 Unauthorized\r\n";
const string FORBIDDEN = "HTTP/1.1 403 Forbidden\r\n";
const string BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\n";

const int PORT = 80;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int clientSD;
int serverSD;

struct Request{
    string method;
    string uri;
    string protocol;
};

string read_data() {
    string response = "";
    char end = 0;
    while (true)
    {
        char current = 0;
        recv(clientSD, &current , 1 , 0);
        if (current == '\n' || current == '\r')
        {
            if (end == '\r' && current == '\n')
                break;
        }
        else response += current;
        end = current;
    }

    return response;
}

Request parse_request(string request){
    Request parsed;
    string str = "";
    int arg = 0;

    for (int i = 0; i <= request.length(); i++) {
        if (arg == 3){
            return parsed;
        }
        if (request[i] == ' ' || i == request.length()) {
            if (arg == 0)
                parsed.method = str;
            else if (arg == 1)
                parsed.uri = str;
            else if (arg == 2)
                parsed.protocol = str;
            str = "";
            arg++;
        }
        else
            str += request[i];
    }
}

void contruct_response(Request &request, string &status, string &file_content){
    file_content = "";
    // cannot understand request 400
    if (request.method != "GET"){
        status = BAD_REQUEST;
        file_content = BAD_REQUEST;
    }
    else if (request.method == "GET"){
        // past contains ".." - forbidden 403
        if (request.uri.substr(0,2) == ".."){
            status = FORBIDDEN;
            file_content = FORBIDDEN;
        }
            // request for secret file
        else if (request.uri.length() >= 15 &&
                 request.uri.substr(request.uri.length() - 15, request.uri.length()) == "SecretFile.html"){
            status = UNAUTHORIZED;
            file_content = UNAUTHORIZED;
        }
            // request is accepted
        else {
            // get current directory using "."
            if (request.uri[0] != '.' && request.uri[0] != '/'){
                request.uri = "./" + request.uri;
            }
            else if (request.uri[0] == '/'){
                request.uri = "." + request.uri;
            }

            cout << "File: " << request.uri << endl;
            //try to open file, nullptr is does not exist/unathorized
            FILE *file = fopen(request.uri.c_str() , "r");
            if (file == nullptr){
                // no read permission
                if (errno == EACCES){
                    // 401
                    status = UNAUTHORIZED;
                    file_content = UNAUTHORIZED;
                }
                else{
                    // 404
                    status = DOES_NOT_EXIST;
                    ifstream file("FileNotFound.txt");
                    string line;
                    if (file.is_open()){
                        while (getline(file, line)) {
                            file_content += line;
                        }
                    }
                    file.close();
                }
            }
            else {
                // 200
                status = OK;
                while (!feof(file)) {
                    char c = fgetc(file);
                    if (c < 0) {
                        // char not in ascii table
                        continue;
                    }
                    file_content += c;
                }
                fclose(file);
            }
        }
    }
    else {
        cout << "bottom 400" << endl;
        status = BAD_REQUEST;
        file_content = BAD_REQUEST;
    }
}

void *thread_function(void *dummyPtr) {
    // use port 80 for http
    pthread_mutex_lock(&mutex1);

    // read request
    string request = read_data();
    cout << "Request: " << request << endl;

    // parse request
    Request parsed = parse_request(request);
    cout << "Parsed:\nMethod: " << parsed.method << "\nURI: " << parsed.uri << "\nProtocol: " << parsed.protocol << endl;
    // construct response
    string code;
    string file_content;
    contruct_response(parsed, code, file_content);
    string pageLength = to_string(file_content.size());
    string response = code +
                      "Content-Length: " + pageLength + "\r\n"
                      + "Content-Type: text/plain\r\n" +
                      "\r\n" + file_content;
    cout << response << endl;

    // write response
    int sendResponse = send(clientSD, response.c_str(), strlen(response.c_str()), 0);

    pthread_mutex_unlock(&mutex1);
    close(clientSD);
    return nullptr;
}


int main() // 0 args
{
    // setup socket addr
    sockaddr_in acceptSockAddr;
    bzero((char*)&acceptSockAddr, sizeof(acceptSockAddr));
    acceptSockAddr.sin_family= AF_INET; // Address Family Internet
    acceptSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    acceptSockAddr.sin_port = htons(PORT);

    serverSD = socket(AF_INET, SOCK_STREAM, 0);

    const int on = 1;
    int set = setsockopt(serverSD, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int));

    int rc = bind(serverSD, (sockaddr*)&acceptSockAddr, sizeof(acceptSockAddr));
    cout << "Socket Created" << endl;
    int listenSocket = listen(serverSD, 1);


    while(true)
    {    // create client sd
        sockaddr_in clientSockAddr;
        socklen_t clientSockAddrSize = sizeof(clientSockAddr);

        cout << "Listening on port: " << PORT << endl;
        // accept client
        clientSD = accept(serverSD, (sockaddr *) &clientSockAddr,
                          &clientSockAddrSize);
        assert(clientSD != 0);

        cout << "Client Connected" << endl;
        pthread_t thread_id; //create thread

        //std::cout <<"main() : creating thread, " << std::endl;
        int ThreadResult = pthread_create(&thread_id, NULL, thread_function, NULL);

        if(ThreadResult != 0 )
        {
            cout << "unable to create thread." << std::endl;
            continue;
        }


    }
}

/*
Your server waits for a connection and an HTTP GET request (Please perform multi-threaded handling).
Your server only needs to respond to HTTP GET request.
After receiving the GET request
If the file exists, the server opens the file that is requested and sends it (along with the HTTP 200 OK code, of course).
If the file requested does not exist, the server should return a 404 Not Found code along with a custom File Not Found page.
If the HTTP request is for SecretFile.html then the web server should return a 401 Unauthorized.
If the request is for a file that is above the directory structure where your web server is running ( for example, "GET ../../../etc/passwd" ),
 you should return a 403 Forbidden.
Finally, if your server cannot understand the request, return a 400 Bad Request.
*/