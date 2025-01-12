#include <stdio.h>
#include <conio.h>
#include <i86.h>
#include <dos.h>
#include <string.h>
#include <assert.h>

#include "bnkfile.h"
#include "opl2.h"

instrument_t * convert_to_instrument(BNKFile *bnkFile, int index, uint8_t channel, instrument_t * instr)
{
	BNKInstrument *bnkInstr = bnkFile->instruments[index];
	instr->operators[0].attack = bnkInstr->oplModulator->attack;
	instr->operators[0].decay = bnkInstr->oplModulator->decay;
	instr->operators[0].sustain = bnkInstr->oplModulator->sustain;
	instr->operators[0].release = bnkInstr->oplModulator->release;
	instr->operators[0].keyScaleLevel = bnkInstr->oplModulator->ksl;
	instr->operators[0].frequencyMultiplier = bnkInstr->oplModulator->multiple;
	instr->operators[0].waveForm = bnkInstr->iModWaveSel;
	instr->operators[0].outputLevel = bnkInstr->oplModulator->totalLevel;
	instr->operators[0].hasSustain = (bnkInstr->oplModulator->sustain > 0);
	instr->operators[0].hasEnvelopeScaling = bnkInstr->oplModulator->ksr !=0; /* not sure*/
	instr->operators[0].hasTremolo = (bool)bnkInstr->oplModulator->am != 0;
	instr->operators[0].hasVibrato = (bool)bnkInstr->oplModulator->vib != 0;

	instr->operators[1].attack = bnkInstr->oplCarrier->attack;
	instr->operators[1].decay = bnkInstr->oplCarrier->decay;
	instr->operators[1].sustain = bnkInstr->oplCarrier->sustain;
	instr->operators[1].release = bnkInstr->oplCarrier->release;
	instr->operators[1].keyScaleLevel = bnkInstr->oplCarrier->ksl;
	instr->operators[1].frequencyMultiplier = bnkInstr->oplCarrier->multiple;
	instr->operators[1].waveForm = bnkInstr->iModWaveSel;
	instr->operators[1].outputLevel = bnkInstr->oplCarrier->totalLevel;
	instr->operators[1].hasSustain = (bnkInstr->oplCarrier->sustain > 0);
	instr->operators[1].hasEnvelopeScaling = bnkInstr->oplCarrier->ksr !=0; /* not sure*/
	instr->operators[1].hasTremolo = (bool)bnkInstr->oplCarrier->am != 0;
	instr->operators[1].hasVibrato = (bool)bnkInstr->oplCarrier->vib != 0;
	instr->feedback = bnkInstr->oplModulator->feedback;
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

	delay(3000);
	return 0;
}
