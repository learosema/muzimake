#ifndef __FILEDATA_H__
#define __FILEDATA_H__

#include <stdint.h>

// Opaque pointer (s. https://blog.mbedded.ninja/programming/design-patterns/opaque-pointers/)
typedef struct sound_file_s* sound_file_t;

// Filetype
typedef enum file_type {
	UNKNOWN = 0,
	MUSIC = 1,
	INSTRUMENT_BANK = 2,
	INSTRUMENT = 3
} file_type_t;

// Properties
typedef struct file_prop {
	//
} file_prop_t;

// Instrument
typedef struct file_inst {
	//
} file_inst_t;

// Instrument bank
typedef struct file_bank {
	//
} file_bank_t;

// Items / Entries / Tracks
typedef struct file_trck {
	//
} file_trck_t;

// Full sound file data
typedef struct file_data {
	file_type_t type;
	file_prop_t properties;
	file_bank_t instrument_bank;
	file_trck_t tracks;

	void* data; // TODO: to be removed later!

} file_data_t;

// Function pointer (s. https://www.geeksforgeeks.org/function-pointer-in-c/)
// --> return_type (*pointer_name)(parameter_types);
typedef struct file_func {
	void (*init_file)(file_data_t*);
	void (*read_file)(file_data_t*, char*);
	void (*close_file)(file_data_t*);

	// Debug only
	void (*dbg_file)(file_data_t*);
} file_func_t;

#endif // __FILEDATA_H__
