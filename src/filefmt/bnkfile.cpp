#include <stdlib.h>
#include <string.h>

#include <fileio.h>
#include <opl2.h>

#include "bnkfile.h"

#define ALLOC_TYPE(T, N) ((T *)malloc(sizeof(T) * N))

uint8_t bnk_convert_voiceNum_to_drumType(uint8_t voiceNum) {
	switch (voiceNum) {
		case 6:
			return OPL2_DRUM_BASS;

		case 7:
			return OPL2_DRUM_SNARE;

		case 8:
			return OPL2_DRUM_TOM;

		case 9:
			return OPL2_DRUM_CYMBAL;
			break;

		case 10:
			return OPL2_DRUM_HI_HAT;

		default:
			return 0;
	}
}

/**
 * Convert an instrument from the bnk file to an opl2.h instrument
 * @param bnkFile the bank file
 * @param entryIndex the index of the instrument in the namelist
 * (beware, not necessarily the same as the instrument index)
 */
instrument_t bnkfile_convert_to_instrument(bnk_instrument_t bnkInstr)
{
	instrument_t result = {0};

	result.operators[0].attack = bnkInstr.oplModulator.attack & 15;
	result.operators[0].decay = bnkInstr.oplModulator.decay & 15;
	result.operators[0].sustain = (bnkInstr.oplModulator.sustain & 15);

	result.operators[0].release = bnkInstr.oplModulator.releaseRate & 15;
	result.operators[0].keyScaleLevel = bnkInstr.oplModulator.ksl & 3;
	result.operators[0].frequencyMultiplier = bnkInstr.oplModulator.multiple & 15;
	result.operators[0].waveForm = bnkInstr.iModWaveSel & 3;
	result.operators[0].outputLevel = (bnkInstr.oplModulator.totalLevel & 63);

	result.operators[0].hasSustain = (bnkInstr.oplModulator.eg & 1) != 0;
	result.operators[0].hasEnvelopeScaling = (bnkInstr.oplModulator.ksr & 1) !=0;
	result.operators[0].hasTremolo = (bnkInstr.oplModulator.am & 1) != 0;
	result.operators[0].hasVibrato = (bnkInstr.oplModulator.vib & 1) != 0;

	result.operators[1].attack = bnkInstr.oplCarrier.attack & 15;
	result.operators[1].decay = bnkInstr.oplCarrier.decay & 15;
	result.operators[1].sustain = (bnkInstr.oplCarrier.sustain & 15);

	result.operators[1].release = bnkInstr.oplCarrier.releaseRate & 15;
	result.operators[1].keyScaleLevel = bnkInstr.oplCarrier.ksl & 3;
	result.operators[1].frequencyMultiplier = bnkInstr.oplCarrier.multiple & 15;
	result.operators[1].waveForm = bnkInstr.iCarWaveSel & 3;

	result.operators[1].outputLevel = (bnkInstr.oplCarrier.totalLevel & 63);
	result.operators[1].hasSustain = (bnkInstr.oplCarrier.eg & 1) != 0;
	result.operators[1].hasEnvelopeScaling = (bnkInstr.oplCarrier.ksr & 1) !=0;
	result.operators[1].hasTremolo = (bnkInstr.oplCarrier.am & 1) != 0;
	result.operators[1].hasVibrato = (bnkInstr.oplCarrier.vib & 1) != 0;
	result.feedback = bnkInstr.oplModulator.feedback & 0x07;
	result.isAdditiveSynth = (bnkInstr.oplModulator.con & 1) == 0;
	result.isPercussive = (bnkInstr.isPercussive & 1) != 0;

	result.drumType = bnk_convert_voiceNum_to_drumType(bnkInstr.voiceNum);

	return result;
}

instrument_map_t bnkfile_convert_to_map(bnk_file_t * bnkFile)
{
	instrument_map_t map = instrument_map_create(bnkFile->header->numInstuments);
	for (uint16_t i = 0; i < bnkFile->header->numInstuments; i++)
	{
		strcpy(map.entries[i].name, bnkFile->entries[i].name);
		map.entries[i].instrument = bnkfile_convert_to_instrument(bnkFile->instruments[i]);
	}
	return map;
}

//-----------------------------------------------

void bnkfile_init(file_data_t* file)
{
	file->type = INSTRUMENT_BANK;
}

void bnkfile_read(file_data_t* file, char* filename)
{
	FILEPTR fp = fileio_open(filename, "rb");

	long len = fileio_get_size(fp);

	bnk_file_t* bnkFile = (bnk_file_t *)malloc(sizeof(bnk_file_t));
	bnkFile->len = len;
	bnkFile->buffer = (uint8_t *)malloc(len * sizeof(uint8_t));
	fileio_read(bnkFile->buffer, sizeof(uint8_t), len, fp);

	bnkFile->header = (bnk_header_t *)bnkFile->buffer;

	bnkFile->entries = (bnk_entry_t *)(bnkFile->buffer + bnkFile->header->offsetNames);
	bnkFile->instruments = (bnk_instrument_t *)(bnkFile->buffer + bnkFile->header->offsetData);

	fileio_close(fp);

	file->data = bnkFile; // TODO: to be removed, when structure is refactored
}

void bnkfile_debug(file_data_t* file)
{
	bnk_file_t* bnkFile = (bnk_file_t*)file->data; // TODO: to be removed, when structure is refactored

	fprintf(stderr, "bnkfile version %d\.%d\n%d instruments. total %d\noffset names: %d\noffset data: %d\n",
		bnkFile->header->versionMajor,
		bnkFile->header->versionMinor,
		bnkFile->header->numInstuments,
		bnkFile->header->totalNumInstruments,
		bnkFile->header->offsetNames,
		bnkFile->header->offsetData
	);
}

bool bnkfile_write(file_data_t* file, char* filename)
{
	bnk_file_t* bnkFile = (bnk_file_t*)file->data; // TODO: to be removed, when structure is refactored

	FILE * fp = fileio_open(filename, "wb");
	if (fp == NULL) {
		bnk_last_error = BNK_ERR_WRITE_FAILED;
		return false;
	}
	fileio_write(bnkFile->buffer, sizeof(uint8_t), bnkFile->len, fp);
	fileio_close(fp);

	return true;
}

void bnkfile_free(file_data_t* file)
{
	bnk_file_t* bnkFile = (bnk_file_t*)file->data; // TODO: to be removed, when structure is refactored

	if (bnkFile->header) free(bnkFile->buffer);
	bnkFile->buffer = NULL;
	bnkFile->header = NULL;
	bnkFile->entries = NULL;
	bnkFile->instruments = NULL;
	free(bnkFile);
}
