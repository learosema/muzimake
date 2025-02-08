#include <stdlib.h>
#include <stdio.h> // printf

#include "file.h"
#include "filedata.h"

#include "bnkfile.h"
#include "rolfile.h"

// private struct "sound_file_s"
struct sound_file_s {
	file_format_t format;
	file_data_t file_data;
	const file_func_t* func_ptr;
};

file_result_t file_init(sound_file_t* file, file_format_t file_format)
{
	sound_file_t soundfile = (sound_file_t)calloc(1, sizeof(struct sound_file_s));

	soundfile->format = file_format;

	soundfile->func_ptr = NULL;
	switch (file_format)
	{
	case FORMAT_BNK:
		soundfile->func_ptr = &bnkfile_func;
		break;

	case FORMAT_ROL:
		soundfile->func_ptr = &rolfile_func;
		break;

	default:
		return ERROR;
	}

	soundfile->func_ptr->init_file(&soundfile->file_data);

	*file = soundfile;

	return *file != NULL ? SUCCESS : ERROR;
}

file_result_t file_open(sound_file_t* file, char *filename, file_opts_t file_opts)
{
	sound_file_t soundfile = *file;

	soundfile->func_ptr->read_file(&soundfile->file_data, filename);

	return SUCCESS;
}

void file_close(sound_file_t* file)
{
	sound_file_t soundfile = *file;

	soundfile->func_ptr->close_file(&soundfile->file_data);

	free(soundfile);
}

// Debug only
void file_dbg(sound_file_t* file)
{
	sound_file_t soundfile = *file;

	soundfile->func_ptr->dbg_file(&soundfile->file_data);
}

void *file_get_raw(sound_file_t *file)
{
  return (*file)->file_data.data;
}
