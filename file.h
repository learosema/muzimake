/**
 * Misc file utils
 */
#ifndef __FILE_H__
#define __FILE_H__
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
FILEPTR file_open(char * fileName, char *mode);
bool file_eof(FILEPTR fp);

size_t file_read(void * buffer, size_t size, size_t nItems, FILEPTR fp);
size_t file_write(void * buffer, size_t size, size_t nItems, FILEPTR fp);

bool file_close(FILEPTR fp);
long file_get_size(FILEPTR fp);

size_t file_read_chars(FILEPTR fp, char *chars, int n);
uint8_t file_read_u8(FILEPTR fp);
uint16_t file_read_u16le(FILEPTR fp);
uint32_t file_read_u32le(FILEPTR fp);
float file_read_f32le(FILEPTR fp);
uint16_t file_read_u16be(FILEPTR fp);
uint32_t file_read_u32be(FILEPTR fp);
float file_read_f32le(FILEPTR fp);
float file_read_f32be(FILEPTR fp);



#ifdef __cplusplus
}
#endif


#endif
