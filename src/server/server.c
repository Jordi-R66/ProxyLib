#include "server.h"

#define BUFFER_SIZE 512

int main(void) {
	Socket server_sock = { 0 };
	Socket client_sock = { 0 };
	bool run = true;

	Byte buffer[BUFFER_SIZE] = { 0 };
	ssize_t bytes_read = -1;

	do {
		server_sock.fd = createServerSocket(DEFAULT_PORT, DEFAULT_BACKLOG);
	} while (server_sock.fd < 0);

	while (run) {
		setMemory(buffer, 0, BUFFER_SIZE);

		if (acceptSocket(&server_sock, &client_sock)) {
			printf("[Server] Client connected\n");

			if (readSocket(&client_sock, buffer, BUFFER_SIZE, &bytes_read)) {
				
			}
		}
	}

	return 0;
}

