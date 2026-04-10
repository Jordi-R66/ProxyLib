#include "utils.h"

inline uint32_t ipToInt(uint8_t ip[4]) {
	uint32_t output =	((uint32_t)ip[0] << 24) |
						((uint32_t)ip[1] << 16) |
						((uint32_t)ip[2] << 8) |
						((uint32_t)ip[3]);

	return output;
}

inline void intToIP(uint32_t ipInt, uint8_t ip[4]) {
	ip[0] = (uint8_t)((ipInt >> 24) & 0xff);
	ip[1] = (uint8_t)((ipInt >> 16) & 0xff);
	ip[2] = (uint8_t)((ipInt >> 8) & 0xff);
	ip[3] = (uint8_t)((ipInt >> 0) & 0xff);
}