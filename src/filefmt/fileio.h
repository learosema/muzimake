/**
 * Misc file utils
 */
#ifndef __FILEIO_H__
#define __FILEIO_H__
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __LITTLE_ENDIAN__
#ifdef __DOS__
#define __LITTLE_ENDIAN__ 1
#else defined(__BYTE_ORDER__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#else
#define __LITTLE_ENDIAN__ 0
#endif
#endif
#endif

#define FILEPTR FILE*

FILEPTR fileio_open(char * fileName, char *mode);
bool fileio_eof(FILEPTR fp);

size_t fileio_read(void * buffer, size_t size, size_t nItems, FILEPTR fp);
size_t fileio_write(void * buffer, size_t size, size_t nItems, FILEPTR fp);

bool fileio_close(FILEPTR fp);
long fileio_get_size(FILEPTR fp);

size_t fileio_read_chars(FILEPTR fp, char *chars, int n);
uint8_t fileio_read_u8(FILEPTR fp);

uint16_t fileio_read_u16le(FILEPTR fp);
uint16_t fileio_read_u16be(FILEPTR fp);

uint32_t fileio_read_u32le(FILEPTR fp);
uint32_t fileio_read_u32be(FILEPTR fp);

float fileio_read_f32le(FILEPTR fp);
float fileio_read_f32be(FILEPTR fp);

uint16_t byteswap_16(uint16_t in);
uint32_t byteswap_32(uint32_t in);

#ifdef __cplusplus
}
#endif

#endif // __FILEIO_H__
