#include <stdio.h>
#include <stdlib.h>

#include "bnkfile.h"

BNKFile * bnkfile_read(char * filename)
{
	FILE * fp;
	BNKFile *bnkFile = NULL;
	int i;
	long len;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		bnkfile_last_error = BNK_ERR_READ_FAILED;
		return NULL;
	}

	if (fseek(fp, 0L, SEEK_END) != 0) {
		bnkfile_last_error = BNK_ERR_READ_FAILED;
		return NULL;
	}

	len = ftell(fp);
	if (len == -1) {
		bnkfile_last_error = BNK_ERR_READ_FAILED;
		return NULL;
	}

	if (fseek(fp, 0L, 0) != 0) {
		bnkfile_last_error = BNK_ERR_READ_FAILED;
		return NULL;
	}

	bnkFile = (BNKFile *)malloc(sizeof(BNKFile));
	bnkFile->len = len;
	bnkFile->buffer = (uint8_t *)malloc(len * sizeof(uint8_t));
	fread(bnkFile->buffer, sizeof(uint8_t), len, fp);

	bnkFile->header = (BNKHeader *)bnkFile->buffer;
	printf("%s loaded :)\nversion %d\.%d %d instruments. total %d\noffset names: %d\noffset data: %d\n", filename,
		bnkFile->header->versionMajor,
		bnkFile->header->versionMinor,
		bnkFile->header->numInstuments,
		bnkFile->header->totalNumInstruments,
		bnkFile->header->offsetNames,
		bnkFile->header->offsetData
	);

	bnkFile->entries = (BNKEntry *)(bnkFile->buffer + bnkFile->header->offsetNames);
	// TODO: the instruments aren't necessarily in order!
	bnkFile->instruments = (BNKInstrument *)(bnkFile->buffer + bnkFile->header->offsetData);
	fclose(fp);
	return bnkFile;
}

bool bnkfile_write(BNKFile * bnkFile, char *filename)
{
	FILE * fp = fopen(filename, "wb");
	if (fp == NULL) {
		bnkfile_last_error = BNK_ERR_WRITE_FAILED;
		return false;
	}
	fwrite(bnkFile->buffer, sizeof(uint8_t), bnkFile->len, fp);
	fclose(fp);
	return true;
}


void bnkfile_free(BNKFile * bnkFile)
{
	if (bnkFile->header) free(bnkFile->buffer);
	bnkFile->buffer = NULL;
	bnkFile->header = NULL;
	bnkFile->entries = NULL;
	bnkFile->instruments = NULL;
	free(bnkFile);
}
