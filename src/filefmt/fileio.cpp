#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "fileio.h"

uint16_t byteswap_16(uint16_t in)
{
	return ((in & 0xFF) << 8) | (in >> 8);
}

uint32_t byteswap_32(uint32_t in)
{
	return ((in & 0xFF) << 24) |
  	((in & 0xFF00) << 8) |
  	((in & 0xFF0000) >> 8) |
  	(in >> 24);
}

FILEPTR fileio_open(char * fileName, char *mode)
{
	return fopen(fileName, mode);
}

bool fileio_eof(FILEPTR fp) {
	return feof(fp) != 0;
}

bool fileio_close(FILEPTR fp)
{
	return (fclose(fp) == 0);
}

size_t fileio_write(void * buffer, size_t size, size_t nItems, FILEPTR fp)
{
	return fwrite(buffer, size, nItems, fp);
}

size_t fileio_read(void * buffer, size_t size, size_t nItems, FILEPTR fp)
{
	return fread(buffer, size, nItems, fp);
}

long fileio_get_size(FILEPTR fp)
{
	long len;
	if (fp == NULL) {
		return -1;
	}

	if (fseek(fp, 0L, SEEK_END) != 0) {
		return -1;
	}

	len = ftell(fp);
	if (len == -1) {
		return -1;
	}

	if (fseek(fp, 0L, 0) != 0) {
		return -1;
	}
	return len;
}

size_t fileio_read_chars(FILEPTR fp, char *chars, int n)
{
	return fileio_read(chars, sizeof(char), n, fp);
}

uint8_t fileio_read_u8(FILEPTR fp)
{
	uint8_t b = 0;
	fileio_read(&b, sizeof(uint8_t), 1, fp);
	return b;
}

uint16_t fileio_read_u16le(FILEPTR fp)
{
	uint16_t result = 0;
	fileio_read(&result, sizeof(uint16_t), 1, fp);
	#if __LITTLE_ENDIAN__ == 0
	return byteswap16(result);
	#endif
	return result;
}

uint16_t fileio_read_u16be(FILEPTR fp)
{
	uint16_t result = 0;
	fileio_read(&result, sizeof(uint16_t), 1, fp);
	#if __LITTLE_ENDIAN__ == 1
	return byteswap_16(result);
	#endif
	return result;
}

uint32_t fileio_read_u32le(FILEPTR fp)
{
	uint32_t result = 0;
	fileio_read(&result, sizeof(uint32_t), 1, fp);
	#if __LITTLE_ENDIAN__ == 0
	return byteswap32(result);
	#endif
	return result;
}

uint32_t fileio_read_u32be(FILEPTR fp)
{
	uint32_t result = 0;
	fileio_read(&result, sizeof(uint32_t), 1, fp);
	#if __LITTLE_ENDIAN__ == 1
	return byteswap_32(result);
	#endif
	return result;
}

float fileio_read_f32le(FILEPTR fp)
{
	float result = 0;
	fileio_read(&result, sizeof(float), 1, fp);
	#if __LITTLE_ENDIAN__ == 0
	return (uint32_t)byteswap32((uint32_t)result);
	#endif
	return result;
}

float fileio_read_f32be(FILEPTR fp)
{
	float result = 0;
	fileio_read(&result, sizeof(uint32_t), 1, fp);
	#if __LITTLE_ENDIAN__ == 1
	return (float)byteswap_32((uint32_t)result);
	#endif
	return result;
}
