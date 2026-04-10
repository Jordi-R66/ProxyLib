#pragma once

#ifndef CLIENT_CONNECTION
#define CLIENT_CONNECTION

#include "../common/includes.h"
#include "../common/types.h"

#pragma pack()

typedef struct {
	socket_t socket_send, socket_recv;
	FileStream stream_send, stream_read;
	uint32_t lastPacketSent;
	uint32_t lastPacketRecv;
	uint32_t serverConnId;
} ProxyConn;

#define PROXYCONN_SIZE sizeof(ProxyConn)

#pragma pack(1)

ProxyConn connectToProxy();
int disconnectFromProxy(ProxyConn* conn);

#endif