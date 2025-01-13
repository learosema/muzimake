#include <stdio.h>
#include <conio.h>
#include <i86.h>
#include <dos.h>
#include <string.h>
#include <assert.h>

#include "bnkfile.h"
#include "opl2.h"

void convert_to_instrument(BNKFile *bnkFile, int index, instrument_t * instr)
{
	BNKInstrument bnkInstr = bnkFile->instruments[index];

	instr->operators[0].attack = bnkInstr.oplModulator.attack;
	instr->operators[0].decay = bnkInstr.oplModulator.decay;
	instr->operators[0].sustain = bnkInstr.oplModulator.sustain;
	instr->operators[0].release = bnkInstr.oplModulator.releaseRate;
	instr->operators[0].keyScaleLevel = bnkInstr.oplModulator.ksl;
	instr->operators[0].frequencyMultiplier = bnkInstr.oplModulator.multiple;
	instr->operators[0].waveForm = bnkInstr.iModWaveSel;
	instr->operators[0].outputLevel = bnkInstr.oplModulator.totalLevel;
	instr->operators[0].hasSustain = (bnkInstr.oplModulator.eg != 0);
	instr->operators[0].hasEnvelopeScaling = bnkInstr.oplModulator.ksr !=0;
	instr->operators[0].hasTremolo = (bool)bnkInstr.oplModulator.am != 0;
	instr->operators[0].hasVibrato = (bool)bnkInstr.oplModulator.vib != 0;

	instr->operators[1].attack = bnkInstr.oplCarrier.attack;
	instr->operators[1].decay = bnkInstr.oplCarrier.decay;
	instr->operators[1].sustain = bnkInstr.oplCarrier.sustain;
	instr->operators[1].release = bnkInstr.oplCarrier.releaseRate;
	instr->operators[1].keyScaleLevel = bnkInstr.oplCarrier.ksl;
	instr->operators[1].frequencyMultiplier = bnkInstr.oplCarrier.multiple;
	instr->operators[1].waveForm = bnkInstr.iCarWaveSel;
	instr->operators[1].outputLevel = bnkInstr.oplCarrier.totalLevel;
	instr->operators[1].hasSustain = (bnkInstr.oplCarrier.eg != 0);
	instr->operators[1].hasEnvelopeScaling = bnkInstr.oplCarrier.ksr !=0;
	instr->operators[1].hasTremolo = (bool)bnkInstr.oplCarrier.am != 0;
	instr->operators[1].hasVibrato = (bool)bnkInstr.oplCarrier.vib != 0;
	instr->feedback = bnkInstr.oplModulator.feedback & 0x07;
	instr->isAdditiveSynth = bnkInstr.oplModulator.con == 0;
}


int main()
{
	BNKFile *bnkFile = bnkfile_read("STANDARD.BNK");

	assert(strncmp(bnkFile->header->signature, "ADLIB-", 6) == 0);
	printf("bankfile loaded: %d instruments\n", bnkFile->header->numInstuments);
	assert(bnkFile->header->versionMajor == 1);
	assert(bnkFile->header->versionMinor == 0);
	assert(bnkFile->header->numInstuments == 145);
	assert(bnkFile->header->totalNumInstruments == 160);

	instrument_t inst = {0};
	for (int j = 0; j < bnkFile->header->numInstuments; j++) {
		printf("Testing instrument %d: %s\n", j, bnkFile->entries[j].name);
		convert_to_instrument(bnkFile, j, &inst);
		opl2_setInstrument(0, &inst, 1);
		for(int i = 0; i < 2; i++) {
			opl2_playNote(0, i+ 3, 0);
			delay(500);
			opl2_setKeyOn(0, false);
			delay(100);
			opl2_playNote(0, i + 3, 7);
			delay(500);
			opl2_setKeyOn(0, false);
			delay(100);
		}
	}

	delay(1000);
	return 0;
}
