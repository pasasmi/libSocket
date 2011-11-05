#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> //needed for socket.h and in.h
#include <sys/socket.h>
#include <netinet/in.h> //internet domain stuff
#include <netdb.h> //server info
#include <string.h> //bzero warning
#include <strings.h> //strlen warning

#include "libSocket.h"

//Domain:
//#define NETWORK_SOCKET 1
//#define FILE_SOCKET 2

//Socket type:
//#define STREAM 1

/*
 *createSocket(Domain, socket type, port)
 * */

int createSocket(int domain, int type, char *address, int port)
{
	int domainSocket;
	switch(domain)
	{
		case 2:
			domainSocket = AF_UNIX;
			break;
		default:
		case 1:
			domainSocket = AF_INET;
			break;
	}

	int typeSocket;
	switch(type)
	{
		default:
		case 1:
			typeSocket = SOCK_STREAM;
	}
	
	int socketDescriptor = socket(domainSocket, typeSocket, 0);
	if (socketDescriptor < 0 ) return -10; //fails to create socket

	struct hostent *server;
	struct sockaddr_in server_address;
	
	server = gethostbyname(address);
	if (server == NULL ) return -20; //fails to resolve server

	bzero((char *) &server_address, sizeof(server_address));
	//initializing the server address to zero
	
	server_address.sin_family = domainSocket;
	bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);

	server_address.sin_port = htons(port);
	
	if(connect(socketDescriptor, &server_address, sizeof(server_address)) < 0) return -30; //fails to connect

	return socketDescriptor;
}

int stdinToSocket(int socket)
{
	char buffer[STDIN_BUFFER];
	bzero(buffer, STDIN_BUFFER);
	fgets(buffer, STDIN_BUFFER-1, stdin);

	int status = write(socket, buffer, strlen(buffer));
	return status;
	/*
	 *status == 0 -> socket closed
	 *status < 0  -> unable to write
	 *status > 0  -> OK
	 */
}

int stdoutFromSocket(int socket)
{
	char buffer[STDOUT_BUFFER];
	bzero(buffer,STDIN_BUFFER);

	int status = read(socket, buffer, STDOUT_BUFFER-1);
	
	if (status > 0)
	{
		int i;
		for(i=0; i < STDOUT_BUFFER-1; i++) fputc(buffer[i], stdout);
		fflush(stdout);
	}

	return status;
	/*
	 *status == 0 -> socket closed
	 *status < 0  -> unable to read
	 *status > 0  -> OK
	 * */
}

