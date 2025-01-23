#include <stdint.h>

uint16_t byteswap16(uint16_t in)
{
	return ((in & 0xFF) << 8) | (in >> 8);
}

uint32_t byteswap32(uint32_t in)
{
	return ((in & 0xFF) << 24) |
  	((in & 0xFF00) << 8) |
  	((in & 0xFF0000) >> 8) |
  	(in >> 24);
}
