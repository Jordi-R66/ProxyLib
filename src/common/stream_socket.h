#pragma once

#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "includes.h"

typedef struct {
	int original_fd;
	FileStream rx;
	FileStream tx;
} StreamSocket, * StreamSocketPtr;

void StreamSock_create(socket_t socket_fd, StreamSocketPtr stream_sock);

void StreamSock_close(StreamSocketPtr stream_sock);

/**
 * Bloque en attente d'une connexion cliente sur une socket serveur existante.
 * Initialise le stream_sock passé en paramètre avec le client accepté.
 * 
 * @param server_fd Le descripteur de la socket qui écoute (listen)
 * @param stream_sock Le pointeur vers la structure à remplir pour le client
 * @return true si un client a été accepté et initialisé avec succès, false sinon.
 */
bool StreamSock_accept(socket_t server_fd, StreamSocketPtr stream_sock);

/**
 * Crée une socket et se connecte à un serveur distant, puis initialise la structure.
 * 
 * @param hostname L'adresse IP ou le nom de domaine cible (ex: "127.0.0.1")
 * @param port Le port cible (ex: 80)
 * @param stream_sock Le pointeur vers la structure à initialiser
 * @return true si la connexion et l'initialisation ont réussi, false sinon.
 */
bool StreamSock_connect(const char* hostname, uint16_t port, StreamSocketPtr stream_sock);

socket_t StreamSock_createServer(uint16_t port, int backlog);

#endif
