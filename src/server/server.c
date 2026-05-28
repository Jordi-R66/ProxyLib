#include "common/includes.h"
#include "common/stream_socket.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
	socket_t server_fd;
	StreamSocket client_sock;

	char buffer[BUFFER_SIZE];

	server_fd = StreamSock_createServer(PORT, 5);
	if (server_fd >= 0) {
		printf("[Serveur] En écoute sur le port %d...\n", PORT);

		if (StreamSock_accept(server_fd, &client_sock)) {
			printf("[Serveur] Client connecté.\n");

			if (fgets(buffer, BUFFER_SIZE, client_sock.rx) != NULL) {
				printf("[Serveur] Reçu : %s", buffer);
				fprintf(client_sock.tx, "Message reçu avec succès par le serveur !\n");
			}

			StreamSock_close(&client_sock);
			printf("[Serveur] Connexion cliente fermée.\n");
		}
		close(server_fd);
	}

	return 0;
}