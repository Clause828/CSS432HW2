/*Your server waits for a connection and an HTTP GET request (Please perform multi-threaded handling).
Your server only needs to respond to HTTP GET request.
After receiving the GET request
If the file exists, the server opens the file that is requested and sends it (along with the HTTP 200 OK code, of course).
If the file requested does not exist, the server should return a 404 Not Found code along with a custom File Not Found page.
If the HTTP request is for SecretFile.html then the web server should return a 401 Unauthorized.
If the request is for a file that is above the directory structure where your web server is running ( for example, "GET ../../../etc/passwd" ), you should return a 403 Forbidden.
Finally, if your server cannot understand the request, return a 400 Bad Request.*/

int main(int argc, char* argv[])
{
    
}