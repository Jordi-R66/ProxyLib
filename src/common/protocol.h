#pragma once

#ifndef PROXY_PROTOCOL_H
#define PROXY_PROTOCOL_H

#include "types.h"

#pragma pack()

typedef enum {
	CRYPTO_OFF = 0,
	CRYPTO_ON = 1,
	CRYPTO_PUBLIC_KEY = 2,
	CRYPTO_SECRET_KEY = 3
} CryptoPacketType;

typedef union {
	uint8_t bytes[2];
	uint16_t value;
} MagicNumber;

static const MagicNumber PROTOCOL_MAGIC = {
	.bytes = {0xAC, 0xAB}
};

typedef struct {
	MagicNumber magic_number;
	uint16_t total_length;
	uint16_t protocol_version;
	uint8_t packet_type;
} ProtocolHeader;

typedef struct {
	uint8_t crypto_type;	// Asking for cryptography, key exchange
} ProtcolCryptoData;

#pragma pack(1)

#define PROTOCOL_HDR_SIZE sizeof(ProtocolHeader)

#endif