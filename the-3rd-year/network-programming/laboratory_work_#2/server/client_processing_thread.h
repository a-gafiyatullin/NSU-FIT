#ifndef CLIENT_PROCESSING_THREAD_H
#define CLIENT_PROCESSING_THREAD_H

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <pthread.h>
#include <iostream>
#include <fcntl.h>
#include <dirent.h>
#include <string>

#define MAX_FILE_SIZE (1024 * 1024 * 1024 * 1024) //  1 Tb
#define MAX_FILE_NAME_LENGTH 4096                 // byte

void *client_processing(void *client_socket);

#endif // CLIENT_PROCESSING_THREAD_H
