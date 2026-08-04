#pragma once

#ifndef COMMON_TYPES
#define COMMON_TYPES

#include "includes.h"

typedef enum ConnectionState {
	NO_CONN			= 0,
	ACCEPTED		= 1,
	WAITING_CRYPTO	= 2,
	WAITING_REMOTE	= 3,
	CONNECTED		= 4,
	REMOTE_LOST		= 5,
	CLIENT_LOST		= 6
} ConnectionState;

#endif