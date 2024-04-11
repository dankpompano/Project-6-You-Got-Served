#include <stdio>#include <stdio.h>
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
	}
	
	else if(port < 1024 || port > 65535){
		printf("Bad port: %d", port);
	}
	
	else if(chdir(directory) == -1){
		printf("Could not change to directory: %s", directory);
	}
	
	//get socket
	int sockFD = socket(AF_INET, SOCK_STREAM, 0);
	struck sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	bind(sockFD, (struck sockaddr*) &address, sizeof(address));
	int value = 1;
	setsockopt(sockFD, SOL_SOCKET, SOLSO_REUSEADDR, &value, sizeof(value));
	listen(sockFD, 1);

}
