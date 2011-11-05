#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <strings.h>
#include <string.h>

#include "libSocket.h"

int mySocket;

int checkArgs(int argc)
{
	if (argc != 3)
	{
		printf("Wrong number of arguments.\n");
		printf("Usage: ProgramName <address> <port>\n\n");
		return 0;
	}
	else return 1;
}


void errorSocket()
{
	printf("ERROR: ");
	switch(mySocket)
	{
		case -10:
			printf("Failed to create socket\n");
			break;
		case -20:
			printf("Failed to resolve server\n");
			break;
		case -30:
			printf("Failed to connect to server\n");
			break;
		default:
			printf("Something strange happened: call the ghostbusters\n");
	}
	printf("\n");
}

void *writing(void *dummy)
{
	int status;
	while(1)
	{
	status = stdinToSocket(mySocket);
	if(status == 0) break;
	if(status < 0) printf("ERROR: unable to write to socket\n");
	}
}

void *reading(void *dummy)
{
	int status;
	while(1)
	{
		status = stdoutFromSocket(mySocket);
		if(status == 0) break;
		if(status < 0 ) printf("ERROR: unable to read from socket\n");
	}
}

int main(int argc, char *argv[])
{
	if (checkArgs(argc) != 1) exit(0);

	pthread_t readThread, writeThread;
	pthread_attr_t attr;

	int port = atoi(argv[2]);

	mySocket = createSocket(NETWORK_SOCKET, STREAM, argv[1], port);
	
	if (mySocket < 0)
	{
		errorSocket(mySocket);
		exit(0);
	}
	
	pthread_attr_init(&attr);
	pthread_create(&readThread, &attr, reading, "0");
	pthread_create(&writeThread, &attr, writing, "0");

	pthread_join(readThread, NULL);

	return 0;
}
