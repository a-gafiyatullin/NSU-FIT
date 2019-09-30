#ifndef CLIENT_PROCESSING_THREAD_H
#define CLIENT_PROCESSING_THREAD_H

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>

#define MAX_FILE_SIZE (1024 * 1024 * 1024 * 1024)
#define MAX_FILE_NAME_LENGTH 4096

void *client_processing(void *client_socket);

#endif // CLIENT_PROCESSING_THREAD_H
