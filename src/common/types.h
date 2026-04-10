#pragma once

#ifndef COMMON_TYPES
#define COMMON_TYPES

#include "includes.h"

#pragma pack()
typedef struct {
	uint16_t connId;
	uint32_t packetNumber;
	uint32_t size;
} ProxyHeader;

#pragma pack(1)

#define PROXY_HDR_SIZE sizeof(ProxyHeader)
#endif