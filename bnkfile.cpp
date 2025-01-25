#include <stdio.h>
#include <stdlib.h>
#include "bnkfile.h"

/**
 * Convert an instrument from the bnk file to an opl2.h instrument
 * @param bnkFile the bank file
 * @param entryIndex the index of the instrument in the namelist
 * (beware, not necessarily the same as the instrument index)
 */
instrument_t bnk_convert_to_instrument(bnk_instrument_t bnkInstr)
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

	switch (bnkInstr.voiceNum) {
		case 6:
			result.drumType = OPL2_DRUM_BASS;
			break;

		case 7:
			result.drumType = OPL2_DRUM_SNARE;
			break;

		case 8:
			result.drumType = OPL2_DRUM_TOM;
			break;

		case 9:
			result.drumType = OPL2_DRUM_CYMBAL;
			break;

		case 10:
			result.drumType = OPL2_DRUM_HI_HAT;
			break;

		default:
			result.drumType = 0;
	}

	return result;
}

bnk_file_t * bnkfile_read(char * filename)
{
	FILE * fp;
	bnk_file_t *bnkFile = NULL;
	int i;
	long len;

	fp = fopen(filename, "rb");


	bnkFile = (bnk_file_t *)malloc(sizeof(bnk_file_t));
	bnkFile->len = len;
	bnkFile->buffer = (uint8_t *)malloc(len * sizeof(uint8_t));
	fread(bnkFile->buffer, sizeof(uint8_t), len, fp);

	bnkFile->header = (bnk_header_t *)bnkFile->buffer;
	printf("%s loaded :)\nversion %d\.%d %d instruments. total %d\noffset names: %d\noffset data: %d\n", filename,
		bnkFile->header->versionMajor,
		bnkFile->header->versionMinor,
		bnkFile->header->numInstuments,
		bnkFile->header->totalNumInstruments,
		bnkFile->header->offsetNames,
		bnkFile->header->offsetData
	);

	bnkFile->entries = (bnk_entry_t *)(bnkFile->buffer + bnkFile->header->offsetNames);
	bnkFile->instruments = (bnk_instrument_t *)(bnkFile->buffer + bnkFile->header->offsetData);
	fclose(fp);
	return bnkFile;
}

bool bnkfile_write(bnk_file_t * bnkFile, char *filename)
{
	FILE * fp = fopen(filename, "wb");
	if (fp == NULL) {
		bnk_last_error = BNK_ERR_WRITE_FAILED;
		return false;
	}
	fwrite(bnkFile->buffer, sizeof(uint8_t), bnkFile->len, fp);
	fclose(fp);
	return true;
}


void bnkfile_free(bnk_file_t * bnkFile)
{
	if (bnkFile->header) free(bnkFile->buffer);
	bnkFile->buffer = NULL;
	bnkFile->header = NULL;
	bnkFile->entries = NULL;
	bnkFile->instruments = NULL;
	free(bnkFile);
}
