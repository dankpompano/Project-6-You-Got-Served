#include <stdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, int **argv)
{
	int port = argv[1];
	char directory[1024] = argv[2];
	
	//ERROR CASES
	if(argc != 3){
		printf("Usage: ./server <port> <path to root>");
        return -1;
	}
	
	else if(port < 1024 || port > 65535){
		printf("Bad port: %d", port);
        return -2;
	}
	
	else if(chdir(directory) == -1){
		printf("Could not change to directory: %s", directory);
        return -3;
	}
	
    //get socket
    int sockFD = socket(AF_INET, SOCK_STREAM, 0);
    struck sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    //bind socket
    bind(sockFD, (struck sockaddr*) &address, sizeof(address));
    int value = 1;
    //allows port reuse
    setsockopt(sockFD, SOL_SOCKET, SOLSO_REUSEADDR, &value, sizeof(value));
    //begins listening
    listen(sockFD, 1);

    while(1)
    {
        //accept connection
        struct sockaddr_storage otherAddress;
        socklen_t otherSize = sizeof(otherAddress);
        int otherSocket = accept(socketFD, (struct sockaddr*) &otherAddress, &otherSize);

        //successful conection accept
        if(otherSocket > 0)
        {
            char request [1024];
            int contentLength = read(otherSocket, request, 1023);
            request[cententLength] = '\0';
            char command[1024];
            char path[1024];
            char http[1024];
            char* realPath;
            sscanf(request, "%s%s%s", command, path, http);

            if(!strcmp(command, "GET") || !strcmp(command, "HEAD"))
            {
                realPath = path;
                if(path[strlen(path)-1] == '/')
                    strcat(path, "index.html");
                if(path[0] == '/')
                    ++realPath;

                if(!strcmp(command, "GET"))
                {
                    

                }
            }
            
            

            close(otherSocket);
        }
    }
    
    




	
	

}
