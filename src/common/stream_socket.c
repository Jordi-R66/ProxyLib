#include "stream_socket.h"

void StreamSock_close(StreamSocketPtr stream_sock) {
	bool is_valid = (stream_sock != NULL);

	if (is_valid && stream_sock->fd >= 0) {
		close(stream_sock->fd);
		stream_sock->fd = -1;
	}
}

bool StreamSock_accept(socket_t server_fd, StreamSocketPtr stream_sock) {
	bool is_valid = (server_fd >= 0 && stream_sock != NULL);
	bool success = false;
	socket_t client_fd = -1;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(sockaddr_in);

	if (is_valid) {
		stream_sock->fd = -1;
		client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
		success = (client_fd >= 0);
	}

	if (success) {
		stream_sock->fd = client_fd;
	}

	return success;
}

bool StreamSock_connect(const char* hostname, uint16_t port, StreamSocketPtr stream_sock) {
	bool is_valid = (hostname != NULL && port > 0 && stream_sock != NULL);
	bool success = false;
	socket_t sock_fd = -1;
	sockaddr_in target_addr;

	if (is_valid) {
		stream_sock->fd = -1;
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);
		is_valid = (sock_fd >= 0);
	}

	if (is_valid) {
		target_addr.sin_family = AF_INET;
		target_addr.sin_port = htons(port);
		is_valid = (inet_pton(AF_INET, hostname, &target_addr.sin_addr) > 0);
	}

	if (is_valid) {
		success = (connect(sock_fd, (sockaddr*)&target_addr, sizeof(sockaddr_in)) == 0);
	}

	if (success) {
		stream_sock->fd = sock_fd;
	}

	if (!success && sock_fd >= 0) {
		close(sock_fd);
	}

	return success;
}

socket_t StreamSock_createServer(uint16_t port, int32_t backlog) {
	bool is_valid = (port > 0 && backlog > 0);
	bool success = false;
	socket_t server_fd = -1;
	int32_t opt = 1;
	sockaddr_in address;

	if (is_valid) {
		server_fd = socket(AF_INET, SOCK_STREAM, 0);
		is_valid = (server_fd >= 0);
	}

	if (is_valid) {
		is_valid = (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0);
	}

	if (is_valid) {
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(port);
		is_valid = (bind(server_fd, (sockaddr*)&address, sizeof(sockaddr_in)) == 0);
	}

	if (is_valid) {
		success = (listen(server_fd, backlog) == 0);
	}

	if (!success && server_fd >= 0) {
		close(server_fd);
		server_fd = -1;
	}

	return server_fd;
}

bool StreamSock_read(StreamSocketPtr stream_sock, void* buffer, size_t size, ssize_t* bytes_read) {
	bool is_valid = (stream_sock != NULL && stream_sock->fd >= 0 && buffer != NULL && size > 0);
	bool success = false;
	ssize_t local_read = -1;
	int local_errno = 0;

	if (is_valid) {
		local_read = recv(stream_sock->fd, buffer, size, MSG_DONTWAIT);
		local_errno = errno;
		success = (local_read > 0);
	}

	if (is_valid && !success) {
		success = (local_read == -1 && (local_errno == EAGAIN || local_errno == EWOULDBLOCK));
	}

	if (is_valid && bytes_read != NULL) {
		*bytes_read = local_read;
	}

	return success;
}

bool StreamSock_write(StreamSocketPtr stream_sock, const void* data, size_t size, ssize_t* bytes_written) {
	bool is_valid = (stream_sock != NULL && stream_sock->fd >= 0 && data != NULL && size > 0);
	bool success = false;
	ssize_t local_written = -1;
	int local_errno = 0;

	if (is_valid) {
		local_written = send(stream_sock->fd, data, size, MSG_NOSIGNAL | MSG_DONTWAIT);
		local_errno = errno;
		success = (local_written == (ssize_t)size);
	}

	if (is_valid && !success) {
		success = (local_written == -1 && (local_errno == EAGAIN || local_errno == EWOULDBLOCK));
	}

	if (is_valid && bytes_written != NULL) {
		*bytes_written = local_written;
	}

	return success;
}