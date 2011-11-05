#ifndef LIBSOCKET_H
#define LIBSOCKET_H

#define NETWORK_SOCKET 1
#define FILE_SOCKET 2

#define STREAM 1

#define STDIN_BUFFER 8
#define STDOUT_BUFFER 8

int createSocket(int domain, int type, char *address, int port);

int stdinToSocket(int socket);
int stdoutFromSocket(int socket);

#endif
