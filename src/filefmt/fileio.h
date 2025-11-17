/**
 * Misc file utils
 */
#ifndef __FILEIO_H__
#define __FILEIO_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdint.h>
#include "list.h"

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

#if defined __DOS__ && defined __WATCOMC__
#include <direct.h>
#define DIRENT_IS_DIR(dirent) ((dirent->d_attr & _A_SUBDIR)>0)
#define DIRENT_IS_FILE(dirent) ((dirent->d_attr & (_A_SUBDIR | _A_VOLID)) == 0)
#else
#include <dirent.h>
#define DIRENT_IS_DIR(dirent) (dirent->d_type == DT_DIR)
#define DIRENT_IS_FILE(dirent) (dirent->d_type == DT_REG)
#endif

#define DIRPTR DIR*
#define FILEPTR FILE*
#define DIRENT struct dirent
#define DIRENTPTR DIRENT *

typedef struct dir_entry_s {
	char *filename;
	size_t len;
	bool is_dir;
} dir_entry_t;

FILEPTR fileio_open(const char * const fileName, const char * const mode);
bool fileio_eof(const FILEPTR fp);

size_t fileio_read(void * const buffer, const size_t size, const size_t nItems, FILEPTR const fp);
size_t fileio_write(void * const buffer, const size_t size, const size_t nItems, FILEPTR const fp);

bool fileio_close(FILEPTR const fp);
long fileio_get_size(FILEPTR const fp);

size_t fileio_read_chars(FILEPTR const fp, char * const chars, const size_t num_chars);
uint8_t fileio_read_u8(FILEPTR const fp);

uint16_t fileio_read_u16le(FILEPTR const fp);
uint16_t fileio_read_u16be(FILEPTR const fp);

uint32_t fileio_read_u32le(FILEPTR const fp);
uint32_t fileio_read_u32be(FILEPTR const fp);

float fileio_read_f32le(FILEPTR const fp);
float fileio_read_f32be(FILEPTR const fp);

uint16_t byteswap_16(const uint16_t in);
uint32_t byteswap_32(const uint32_t in);

dir_entry_t *fileio_dir_entry_create(const char *filename, const bool is_dir);
void fileio_dir_entry_dispose(dir_entry_t * entry);

DIRPTR fileio_open_dir(const char * const path);
DIRENTPTR fileio_read_dir(DIRPTR const dir);
bool fileio_close_dir(DIRPTR const dir);

linked_list_t *fileio_list_files(const char *path);
void fileio_list_files_dispose(linked_list_t * const list);

#ifdef __cplusplus
}
#endif

#endif // __FILEIO_H__
