#include <stdio>#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	unsigned short port = 0;
	int serverSocket = 0;
	int clientSocket = 0;
	char buffer[1024];
	int length = 0;
	int value = 1;
	struct sockaddr_in address;
	struct sockaddr_storage otherAddress;
	socklen_t otherSize = sizeof (otherAddress);
	
	// Read port from user
	printf ("Enter port: ");
	scanf ("%hu", &port);

	// Make socket
	serverSocket = socket (AF_INET, SOCK_STREAM, 0);
	
	// Make it able to reuse ports	
	setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

	// Bind socket to port and local IP
	memset (&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
	bind (serverSocket, (struct sockaddr*)&address, sizeof(address));

	// Listen
	listen (serverSocket, 1);
	
	// Accept	
	clientSocket = accept (serverSocket, (struct sockaddr *) &otherAddress, &otherSize);	

	printf ("\n\n*Chat Started*\n\n");
	
	// Receive messages
	while ((length = read( clientSocket, buffer, sizeof(buffer) - 1)) > 0)
	{		
		buffer[length] = '\0';
		if (strcmp (buffer, "quit") == 0)
			break;
			
		printf ("%s\n", buffer);
	}	

	printf ("\n*Chat Ended*\n");
	
	// Clean up
	close (clientSocket );
	close (serverSocket );
	
	return 0;
}


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
	

}
