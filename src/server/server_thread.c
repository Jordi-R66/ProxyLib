#include "server_thread.h"

#define BUFFER_SIZE 65495

void* thread_main(void* arg) {
	bool is_valid = (arg != NULL);
	bool keep_running = false;

	ServerThreadArgPtr threadArg = NULL;
	ConnStructPtr connStruct = NULL;
	uint8_t i = 0;

	uint8_t buffer[BUFFER_SIZE];
	ssize_t io_bytes = -1;

	if (is_valid) {
		threadArg = (ServerThreadArgPtr)arg;
		memset(&threadArg->connections, 0, CONN_STRUCT_SIZE * MAX_CONNS_PER_THREAD);
		threadArg->threadState = RUNNING;
		keep_running = true;
	}

	while (is_valid && keep_running) {
		if (threadArg->stopThread) {
			keep_running = false;
		}

		if (keep_running) {
			for (i = 0; i < MAX_CONNS_PER_THREAD; i++) {
				connStruct = &threadArg->connections[i];

				switch (connStruct->connState) {
					case ACCEPTED:
						

						break;

					case CONNECTED:
						break;

					default:
						break;
				}
			}
		}

		if (keep_running) {
			usleep(1000);
		}
	}

	if (is_valid) {
		for (i = 0; i < MAX_CONNS_PER_THREAD; i++) {
			connStruct = &threadArg->connections[i];

			if (connStruct->connState != NO_CONN) {
				StreamSock_close(&connStruct->remoteStream);
				StreamSock_close(&connStruct->clientStream);
				connStruct->connState = NO_CONN;
			}
		}

		threadArg->threadState = STOPPED;
	}

	return NULL;
}