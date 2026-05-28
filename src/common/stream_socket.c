#include "stream_socket.h"

#include <unistd.h>

void StreamSock_create(socket_t socket_fd, StreamSocketPtr stream_sock) {
	int fd_tx = -1;
	bool success = false;

	if (stream_sock != NULL) {
		stream_sock->original_fd = socket_fd;
		stream_sock->rx = NULL;
		stream_sock->tx = NULL;

		if (socket_fd >= 0) {
			fd_tx = dup(socket_fd);
			if (fd_tx >= 0) {

				stream_sock->rx = fdopen(socket_fd, "r");
				stream_sock->tx = fdopen(fd_tx, "w");

				if (stream_sock->rx != NULL && stream_sock->tx != NULL) {

					if (setvbuf(stream_sock->rx, NULL, _IONBF, 0) == 0 &&
						setvbuf(stream_sock->tx, NULL, _IONBF, 0) == 0) {

						success = true;
					}
				}
			}

			if (!success) {
				if (stream_sock->rx != NULL) {
					fclose(stream_sock->rx);
				} else {
					close(socket_fd);
				}

				if (stream_sock->tx != NULL) {
					fclose(stream_sock->tx);
				} else if (fd_tx >= 0) {
					close(fd_tx);
				}

				stream_sock->rx = NULL;
				stream_sock->tx = NULL;
				stream_sock->original_fd = -1;
			}
		}
	}

	return;
}

void StreamSock_close(StreamSocketPtr stream_sock) {
	if (stream_sock != NULL) {
		if (stream_sock->rx != NULL) {
			fclose(stream_sock->rx);
			stream_sock->rx = NULL;
		}

		if (stream_sock->tx != NULL) {
			fclose(stream_sock->tx);
			stream_sock->tx = NULL;
		}

		stream_sock->original_fd = -1;
	}

	return;
}

bool StreamSock_accept(socket_t server_fd, StreamSocketPtr stream_sock) {
	bool success = false;

	socket_t client_fd = -1;

	sockaddr_in client_addr;

	socklen_t client_len = sizeof(sockaddr_in);

	if (server_fd >= 0 && stream_sock != NULL) {
		client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);

		if (client_fd >= 0) {
			StreamSock_create(client_fd, stream_sock);

			success = stream_sock->rx != NULL && stream_sock->tx != NULL;

			if (!success)
				close(client_fd);
		}
	}

	return success;
}

bool StreamSock_connect(const char* hostname, uint16_t port, StreamSocketPtr stream_sock) {
	bool success = false;

	socket_t sock_fd = -1;
	sockaddr_in target_addr;

	if (hostname != NULL && port > 0 && stream_sock != NULL) {
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);

		if (sock_fd >= 0) {
			target_addr.sin_family = AF_INET;
			target_addr.sin_port = htons(port);

			if (inet_pton(AF_INET, hostname, &target_addr.sin_addr) > 0) {
				if (connect(sock_fd, (sockaddr*)&target_addr, sizeof(sockaddr_in)) == 0) {
					StreamSock_create(sock_fd, stream_sock);

					success = stream_sock->rx != NULL && stream_sock->tx != NULL;
				}
			}

			if (!success)
				close(sock_fd);
		}
	}

	return success;
}

socket_t StreamSock_createServer(uint16_t port, int backlog) {
	socket_t server_fd = -1;
	int opt = 1;

	sockaddr_in address;
	bool success = false;

	if (port > 0 && backlog > 0) {
		server_fd = socket(AF_INET, SOCK_STREAM, 0);

		if (server_fd >= 0) {
			if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0) {
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_port = htons(port);

				if (bind(server_fd, (sockaddr*)&address, sizeof(sockaddr_in)) == 0) {
					success = listen(server_fd, backlog) == 0;
				}
			}

			if (!success) {
				close(server_fd);
				server_fd = -1;
			}
		}
	}

	return server_fd;
}