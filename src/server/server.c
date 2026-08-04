#include "server.h"

#define PORT 8080


int main(void) {
	signal(SIGPIPE, SIG_IGN);

	socket_t server_fd;
	StreamSocket client_sock;
	ServerThreadArg server_threads[THREAD_NUMBER];

	char buffer[BUFFER_SIZE];

	memset(server_threads, 0, sizeof(server_threads));

	for (uint8_t i = 0; i < THREAD_NUMBER; i++) {
		pthread_create(&server_threads[i].thread_id, NULL, thread_main, &server_threads[i]);
	}

	server_fd = StreamSock_createServer(PORT, 5);

	if (server_fd >= 0) {
		printf("[Serveur] En écoute sur le port %d...\n", PORT);

		while (true) {
			if (StreamSock_accept(server_fd, &client_sock)) {
				printf("[Serveur] Client connecté.\n");

				if (fgets(buffer, BUFFER_SIZE, client_sock.rx) != NULL) {
					printf("[Serveur] Reçu : %s", buffer);
					fprintf(client_sock.tx, "Message reçu avec succès par le serveur !\n");
				}

				StreamSock_close(&client_sock);
				printf("[Serveur] Connexion client fermée.\n");
			}
		}

		close(server_fd);
	} else {
		for (uint8_t i = 0; i < (THREAD_NUMBER / 2); i++) {
			server_threads[i].stopThread = true;
		}
	}

	for (uint8_t i = 0; i < THREAD_NUMBER; i++) {
		pthread_join(server_threads[i].thread_id, NULL);
	}

	return 0;
}

