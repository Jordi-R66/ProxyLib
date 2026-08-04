#pragma once

#ifndef STREAM_SOCKET_H
#define STREAM_SOCKET_H

#include "includes.h"

typedef struct StreamSocket {
	socket_t fd;
} StreamSocket, * StreamSocketPtr;

#define STREAM_SOCKET_SIZE sizeof(StreamSocket)

void StreamSock_close(StreamSocketPtr stream_sock);

/**
 * Bloque en attente d'une connexion cliente sur un socket serveur existant.
 * Initialise le stream_sock passé en paramètre avec le client accepté.
 *
 * @param server_fd Le descripteur du socket qui écoute (listen)
 * @param stream_sock Le pointeur vers la structure à remplir pour le client
 * @return true si un client a été accepté et initialisé avec succès, false sinon.
 */
bool StreamSock_accept(socket_t server_fd, StreamSocketPtr stream_sock);

/**
 * Crée un socket et se connecte à un serveur distant, puis initialise la structure.
 *
 * @param hostname L'adresse IP ou le nom de domaine cible (ex: "127.0.0.1")
 * @param port Le port cible (ex: 80)
 * @param stream_sock Le pointeur vers la structure à initialiser
 * @return true si la connexion et l'initialisation ont réussi, false sinon.
 */
bool StreamSock_connect(const char* hostname, uint16_t port, StreamSocketPtr stream_sock);

socket_t StreamSock_createServer(uint16_t port, int32_t backlog);

/**
 * Tente de lire des données depuis le socket.
 *
 * @param stream_sock Le pointeur vers la structure initialisée.
 * @param buffer Le tampon de destination.
 * @param size Le nombre d'octets maximum à lire.
 * @param bytes_read Pointeur optionnel pour récupérer le nombre d'octets réellement lus.
 * @return true si au moins un octet a été lu, false en cas de déconnexion ou d'erreur.
 */
bool StreamSock_read(StreamSocketPtr stream_sock, void* buffer, size_t size, ssize_t* bytes_read);

/**
 * Tente d'écrire des données sur le socket.
 *
 * @param stream_sock Le pointeur vers la structure initialisée.
 * @param data Le tampon source contenant les données à envoyer.
 * @param size Le nombre d'octets à écrire.
 * @param bytes_written Pointeur optionnel pour récupérer le nombre d'octets réellement envoyés.
 * @return true si la totalité des octets a été écrite, false sinon.
 */
bool StreamSock_write(StreamSocketPtr stream_sock, const void* data, size_t size, ssize_t* bytes_written);

#endif