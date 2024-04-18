/*******************************
 *
 *  Project Name: Project 6: You Got Served
 *  Description: Creation of a webserver. This project allows for the user to input thier client, webserver, port, command, and requests.
 *  Date: Friday, April 19, 2024
 *  Authors: Joel Justice and Jacob Littler
 *
 *******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>


char* contentType(char* content);
long contentLength(char* content);

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		printf("Usage: ./server <port> <path to root>\n");
        return -1;
	}
    
    int port = atoi(argv[1]);
	char* directory = argv[2];
	
	//ERROR CASES
	
	//invaid port
	if(port < 1024 || port > 65535)
	{
		printf("Bad port: %d\n", port);
        return -2;
	}
	
    //cannot change directory
	else if(chdir(directory) == -1)
	{
		printf("Could not change to directory: %s\n", directory);
        return -3;
	}
	
    //get socket
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    //bind socket
    bind(sockFD, (struct sockaddr*) &address, sizeof(address));
    int value = 1;
    //allows port reuse
    setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
    //begins listening
    listen(sockFD, 1);

    while(1)
    {
        //accept connection
        struct sockaddr_storage otherAddress;
        socklen_t otherSize = sizeof(otherAddress);
        int otherSocket = accept(sockFD, (struct sockaddr*) &otherAddress, &otherSize);

        //successful conection accept
        if(otherSocket > 0)
        {
            char request [1024];
            int bytes = read(otherSocket, request, 1023);
            request[bytes] = '\0';
            char command[1024];
            char path[1024];
            char http[1024];
            char* realPath;
            sscanf(request, "%s%s%s", command, path, http);

            if(!strcmp(command, "GET") || !strcmp(command, "HEAD"))
            {
                realPath = path;
                //concatenate index.html onto path if ending in a /
                if(path[strlen(path)-1] == '/')
                    strcat(path, "index.html");
                
                //skip / if path starts with that
                if(path[0] == '/')
                    ++realPath;
                
                //try to open file
                printf("Path: %s\n", realPath);
                int openFD = open(realPath, O_RDONLY);
                
                //file is available
                if(openFD != -1)
                {
                    //writes the content type and content length. MUST SUPPORT
                    char* pathConfirm = "HTTP/1.0 200 OK\r\n";
                    write(otherSocket, pathConfirm, strlen(pathConfirm)); //passes in the buffer. realPath or request? 
                    char* conType = contentType(realPath);
                    //making sure there is a valid content type
                    if(conType != NULL)
                        write(otherSocket, conType, strlen(conType)); //content type. Need a function.
                    long conLength = contentLength(realPath);
                    char lengthString[100];
                    sprintf(lengthString, "Content-Length: %ld\r\n\r\n", conLength);
                    write(otherSocket, lengthString, strlen(lengthString)); //content length.
                   
		            if(!strcmp(command, "GET"))
		            {
                        //might have to do it over again
                        char buffer[1024];
                        //tells us number of bytes read
                        bytes = read(openFD, buffer, sizeof(buffer));
                        //while the number of bytes read is not 0
                        while(bytes > 0)
                        {
                            write(otherSocket, buffer, bytes);
                            bytes = read(openFD, buffer, sizeof(buffer));
                        }
		            } 
		        }
                else
                {
                    char* error = "HTTP/1.1 404 Not Found\r\n\r\n";
                    write(otherSocket, error, strlen(error));
                    if(!strcmp(command, "GET"))
                    {
                        //write error message HTML to server is they requested the webpage
                        char errorMessage[1024];
                        sprintf(errorMessage, "<HTML><HEAD><TITLE>404 Not Found</TITLE></HEAD><BODY><H1>Not found</H1><P>The requested URL %s was not found on this server.</P></BODY></HTML>", realPath);
                        write(otherSocket, errorMessage, strlen(errorMessage));
                    }
                }
            }
            close(otherSocket);
        }
    }
    	

}

char* contentType(char* content) //path .com, .net
{
//we are searching from the back in order to find the period. this will tell us what file type it is AFTER we find the period. 
	char* type = strrchr(content, '.');
	
    if(type == NULL)
        return NULL;
	if(!strcmp(type, ".html") || !strcmp(type, ".htm"))
		return "Content-Type: text/html\r\n";
    else if(!strcmp(type, ".jpg") || !strcmp(type, ".jpeg"))
        return "Content-Type: image/jpeg\r\n";
    else if(!strcmp(type, ".gif"))
        return "Content-Type: image/gif\r\n";
    else if(!strcmp(type, ".png"))
        return "Content-Type: image/png\r\n";
    else if(!strcmp(type, ".txt") || !strcmp(type, ".c") || !strcmp(type, ".h"))
        return "Content-Type: text/plain\r\n";
    else if(!strcmp(type, ".pdf"))
        return "Content-Type: application/pdf\r\n";
    else
        return NULL;
}

long contentLength(char* content)
{
    //make stat buffer struct
	struct stat buf;
    stat(content, &buf);
    //get the numbr of bytes in the path from the struct
    return buf.st_size;
}
