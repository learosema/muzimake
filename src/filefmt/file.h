#ifndef __FILE_H__
#define __FILE_H__

#include <stdint.h>

// Nice to have later:
// dynamic plugin system: https://homepage.stat.uiowa.edu/~luke/xls/projects/dlbasics/dlbasics.html#NWDE

// Based on: https://moddingwiki.shikadi.net/wiki/Category:All_music_formats
typedef enum file_format {
	FORMAT_INVALID = 0,
	FORMAT_BNK = 1, // Instrument bank file
	FORMAT_ROL = 2 // Music file
	// CMF -- Music file
	// INS -- Instrument file
	// MID -- Music file
	// XMI -- Music file
	// IBK -- Instrument bank file
} file_format_t;

typedef enum file_result {
	SUCCESS = 0,
	ERROR = 1
} file_result_t;

// Opaque pointer (s. https://blog.mbedded.ninja/programming/design-patterns/opaque-pointers/)
typedef struct sound_file_s* sound_file_t;

typedef struct file_opts {
	bool readonly;
} file_opts_t;

const file_opts_t default_file_opts = {
	false // bool readonly
};

file_result_t file_init(sound_file_t* file, file_format_t file_format);

file_result_t file_open(sound_file_t* file, char* filename, file_opts_t file_opts = default_file_opts);

void file_close(sound_file_t* file);

void file_dbg(sound_file_t* file);

void* file_get_raw(sound_file_t* file); // TODO: to be removed, when structure is refactored

#endif // __FILE_H__
