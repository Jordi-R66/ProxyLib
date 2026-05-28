#pragma once

#ifndef COMMON_INCLUDES
#define COMMON_INCLUDES

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef int socket_t;
typedef FILE* FileStream;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

#endif