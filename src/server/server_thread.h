#pragma once

#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include "common/includes.h"
#include "common/socket_wrapper.h"
#include "common/types.h"

#define MAX_CONNS_PER_THREAD 5

typedef enum ThreadState {
	IDLE = 0,
	RUNNING = 1,
	STOPPED = 2
} ThreadState;

typedef struct ConnStruct ConnStruct, * ConnStructPtr;

struct ConnStruct {
	Socket clientStream;
	Socket remoteStream;
	ConnectionState connState;
	bool useCipher;
};

typedef struct {
	ConnStruct connections[MAX_CONNS_PER_THREAD];
	ThreadState threadState;
	pthread_t thread_id;
	uint8_t conn_count;
	bool stopThread;
} ServerThreadArg, *ServerThreadArgPtr;

#define CONN_STRUCT_SIZE sizeof(ConnStruct)
#define SERVER_THREAD_ARG_SIZE sizeof(ServerThreadArg)

void* thread_main(void* arg);

#endif