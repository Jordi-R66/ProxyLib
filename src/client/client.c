#include <stdio.h>
#include "common/stream_socket.h"

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) {
	StreamSocket server_sock;
	char buffer[BUFFER_SIZE];

	printf("[Client] Connexion vers %s:%d...\n", SERVER_IP, PORT);

	if (StreamSock_connect(SERVER_IP, PORT, &server_sock)) {
		printf("[Client] Connecté au serveur.\n");

		fprintf(server_sock.tx, "Hello Server, je teste notre architecture !\n");
		printf("[Client] Message envoyé.\n");

		if (fgets(buffer, BUFFER_SIZE, server_sock.rx) != NULL) {
			printf("[Client] Réponse : %s", buffer);
		}

		StreamSock_close(&server_sock);
		printf("[Client] Déconnexion.\n");
	}
	else {
		printf("[Client] Échec de la connexion.\n");
	}

	return 0;
}