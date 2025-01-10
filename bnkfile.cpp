#include <stdio.h>
#include <stdlib.h>

#include "bnkfile.h"

BNKFile * bnkfile_read(char * filename) {
	FILE * fp;
	BNKFile *bnkFile = NULL;
	int i;
	size_t len;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Unable to read bankfile: %s\n", filename);
		return NULL;
	}

	bnkFile = (BNKFile *)malloc(sizeof(BNKFile));
	bnkFile->header = (BNKHeader *)malloc(sizeof(BNKHeader));

	fread(bnkFile->header, sizeof(BNKHeader), 1, fp);
	printf("%s loaded :)\nversion %d\.%d %d instruments. total %d\noffset names: %d\noffset data: %d\n", filename,
		bnkFile->header->versionMajor,
		bnkFile->header->versionMinor,
		bnkFile->header->numInstuments,
		bnkFile->header->totalNumInstruments,
		bnkFile->header->offsetNames,
		bnkFile->header->offsetData
		);

	fseek(fp, bnkFile->header->offsetNames, SEEK_SET);
	bnkFile->entries = (BNKEntry *)malloc(bnkFile->header->totalNumInstruments * sizeof(BNKEntry));
	bnkFile->instruments = (BNKInstrument *)malloc(bnkFile->header->totalNumInstruments * sizeof(BNKInstrument));
	fread(bnkFile->entries, sizeof(BNKEntry), bnkFile->header->totalNumInstruments, fp);
	fseek(fp, bnkFile->header->offsetData, SEEK_SET);
	fread(bnkFile->instruments, sizeof(BNKInstrument), bnkFile->header->totalNumInstruments, fp);
	for (i = 0; i < bnkFile->header->totalNumInstruments; i++) {
		printf("Instrument %d: %s\n", i, bnkFile->entries[i].name);
	}
	fclose(fp);
	return bnkFile;
}

void bnkfile_free(BNKFile * bnkFile) {
	if (bnkFile->header) free(bnkFile->header);
	if (bnkFile->entries) free(bnkFile->entries);
	if (bnkFile->instruments) free(bnkFile->instruments);
	bnkFile->header = NULL;
	bnkFile->entries = NULL;
	bnkFile->instruments = NULL;
	free(bnkFile);



}
