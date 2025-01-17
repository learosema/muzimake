#include <stdio.h>
#include <conio.h>
#include <i86.h>
#include <dos.h>
#include <string.h>
#include <assert.h>

#include "bnkfile.h"
#include "opl2.h"

/* ... just the human behind this parsing the format...

KS = ksl
ML = Frequency Multiplier
FB = Feedback
A  = Attack
S  = Sustain
EG = Envelope Gain (hasSustain)
D  = Decay
R  = Release Rate
AM = Amplitude Modulation (Tremolo)
VB = Vibrato
ER = Key scaling envelope rate
CT = Connection Type
WT = Wave Type
upper case = modulator
lower case = carrier

$RASS1:
      KS ML FB A  S  EG D  R  TL AM VB
00 00 00 01 07 07 0a 01 01 0e 16 00 00

ER CT ks ml fb a  s  eg d  r  tl am vb
00 01 00 01 07 08 09 01 01 0e 00 00 00
er ct WT wt
00 01 00 00
M: 7 1 5 14
C  8 1 6 14


AALTO
      KS ML FB A  S  EG D  R  TL AM VB
01 07 02 04 01 01 0a 00 0b 02 00 00 00
ER CT ks ml fb a  s  eg d  r  tl am vb
01 01 fa 90 41 8a fa 90 90 24 38 a7 7a
er ct WT wt
cf 91 92 52

// Should have ADSR 10 0 5 4 ???


Strange Instrument ART5:
Sometimes there are cryptic messages within instruments
It's important to filter out unused bytes.

      KS ML FB A  S  EG D  R  TL AM VB
0d 0a 53 72 63 75 65 6f 20 49 53 66 6c
00 00 01 02 03 05 05 01 00 09 03 00 01   <- cleaned up

ER CT ks ml fb a  s  eg d  r  tl am vb
73 20 20 73 70 6c 65 20 79 4d 74 20 6f
01 00 00 03 00 0c 05 00 09 0d 04 00 01   <- cleaned up

er ct WT wt
6c 79 37 37 00
01 01 03 03 00 <- cleaned up

*/

/* STD3.bnk

      KS ML FB A  S  EG D  R  TL AM VB
00 00 00 01 07 07 0a 01 01 0e 16 00 00

ER CT ks ml fb a  s  eg d  r  tl am vb
00 01 00 01 07 08 09 01 01 0e 00 00 00

er ct WT wt
00 01 00 00

      KS ML FB A  S  EG D  R  TL AM VB
00 00 00 01 05 07 0a 01 01 0e 13 01 01

ER CT ks ml fb a  s  eg d  r  tl am vb
00 01 00 02 05 0f 08 01 00 0a 00 01 01

00 01 00 01 00 00 00 01 05 07 0a 01 01 0e 13 01

0100010001050f0f01000c0001010001000100000101050f0001020b2800010001000005050001020b0000010101000000000104070f0001





*/


void convert_to_instrument(BNKFile *bnkFile, int index, instrument_t * instr)
{
	BNKInstrument bnkInstr = bnkFile->instruments[index];

	instr->operators[0].attack = bnkInstr.oplModulator.attack & 15;
	instr->operators[0].decay = bnkInstr.oplModulator.decay & 15;
	instr->operators[0].sustain = 15 - (bnkInstr.oplModulator.sustain & 15);
	instr->operators[0].release = bnkInstr.oplModulator.releaseRate & 15;
	instr->operators[0].keyScaleLevel = bnkInstr.oplModulator.ksl & 3;
	instr->operators[0].frequencyMultiplier = bnkInstr.oplModulator.multiple & 15;
	instr->operators[0].waveForm = bnkInstr.iModWaveSel & 3;
	instr->operators[0].outputLevel = bnkInstr.oplModulator.totalLevel & 63;
	instr->operators[0].hasSustain = (bnkInstr.oplModulator.eg & 1) != 0;
	instr->operators[0].hasEnvelopeScaling = (bnkInstr.oplModulator.ksr & 1) !=0;
	instr->operators[0].hasTremolo = (bnkInstr.oplModulator.am & 1) != 0;
	instr->operators[0].hasVibrato = (bnkInstr.oplModulator.vib & 1) != 0;

	instr->operators[1].attack = bnkInstr.oplCarrier.attack & 15;
	instr->operators[1].decay = bnkInstr.oplCarrier.decay & 15;
	instr->operators[1].sustain = 15 - (bnkInstr.oplCarrier.sustain & 15);
	instr->operators[1].release = bnkInstr.oplCarrier.releaseRate & 15;
	instr->operators[1].keyScaleLevel = bnkInstr.oplCarrier.ksl & 3;
	instr->operators[1].frequencyMultiplier = bnkInstr.oplCarrier.multiple & 15;
	instr->operators[1].waveForm = bnkInstr.iCarWaveSel & 3;
	instr->operators[1].outputLevel = bnkInstr.oplCarrier.totalLevel & 63;
	instr->operators[1].hasSustain = (bnkInstr.oplCarrier.eg & 1) != 0;
	instr->operators[1].hasEnvelopeScaling = (bnkInstr.oplCarrier.ksr & 1) !=0;
	instr->operators[1].hasTremolo = (bnkInstr.oplCarrier.am & 1) != 0;
	instr->operators[1].hasVibrato = (bnkInstr.oplCarrier.vib & 1) != 0;
	instr->feedback = bnkInstr.oplModulator.feedback & 0x07;
	instr->isAdditiveSynth = (bnkInstr.oplModulator.con & 1) == 0;
	instr->isPercussive = (bnkInstr.isPercussive & 1) != 0;
	switch (bnkInstr.voiceNum) {
		case 6:
			instr->drumType = OPL2_DRUM_BASS;
			break;

		case 7:
			instr->drumType = OPL2_DRUM_SNARE;
			break;

		case 8:
			instr->drumType = OPL2_DRUM_TOM;
			break;

		case 9:
			instr->drumType = OPL2_DRUM_CYMBAL;
			break;

		case 10:
			instr->drumType = OPL2_DRUM_HI_HAT;
			break;

		default:
			instr->drumType = 0;
	}
}


int main()
{
	if (!opl2_init()) {
		printf("No adlib present\n");
		return -1;
	}
	bool escape = false;
	opl2_reset();
	BNKFile *bnkFile = bnkfile_read("STD3.BNK");

	assert(strncmp(bnkFile->header->signature, "ADLIB-", 6) == 0);
	printf("bankfile loaded: %d instruments\n", bnkFile->header->numInstuments);
/*	assert(bnkFile->header->versionMajor == 1);
	assert(bnkFile->header->versionMinor == 0);
	assert(bnkFile->header->numInstuments == 145);
	assert(bnkFile->header->totalNumInstruments == 160);*/
	printf("sizeof(bnkinstrument) = %d\n", sizeof(BNKInstrument));
	opl2_setPercussion(true);

	instrument_t inst = {0};
	for (int j = 0; j < bnkFile->header->numInstuments; j++) {
		// if (!bnkFile->instruments[j].isPercussive) {
		// 	continue;
		// }
		printf("Testing instrument %d: %s", j, bnkFile->entries[j].name);
		printf(" (%s)", bnkFile->instruments[j].isPercussive ? "drum" : "melo");
		printf(" (%d)", bnkFile->instruments[j].voiceNum);
		printf(" (con: %d)\n", bnkFile->instruments[j].oplModulator.con);
		for (size_t k = 0; k < 30; k++) {
			uint8_t *ptr = (uint8_t *)&(bnkFile->instruments[j]);
			printf("%02x", ptr[k]);
		}
		printf("\n");
		convert_to_instrument(bnkFile, j, &inst);

		printf("A:%02d D:%02d S:%02d R:%02d K%d L%02d %s%s%s%s | A:%02d D:%02d S:%02d R:%02d K%d L%02d %s%s%s%s FB:%d\n",
			inst.operators[1].attack,
			inst.operators[1].decay,
			inst.operators[1].sustain,
			inst.operators[1].release,
			inst.operators[1].keyScaleLevel,
			inst.operators[1].outputLevel,
			inst.operators[1].hasEnvelopeScaling?"E":" ",
			inst.operators[1].hasSustain?"S":" ",
			inst.operators[1].hasTremolo?"T":" ",
			inst.operators[1].hasVibrato?"V":" ",
			inst.operators[0].attack,
			inst.operators[0].decay,
			inst.operators[0].sustain,
			inst.operators[0].release,
			inst.operators[0].keyScaleLevel,
			inst.operators[0].outputLevel,
			inst.operators[0].hasEnvelopeScaling?"E":" ",
			inst.operators[0].hasSustain?"S":" ",
			inst.operators[0].hasTremolo?"T":" ",
			inst.operators[0].hasVibrato?"V":" ",
			inst.feedback
		);
		printf("\n");

		opl2_setInstrument(0, &inst, 1);
		if (inst.isPercussive) {
			opl2_setDrumInstrument(&inst, inst.drumType, 1);
		}

		for(int i = 0; i < 2; i++) {
			if (inst.isPercussive) {

				opl2_playDrum(inst.drumType, i, 0);
				delay(300);
				opl2_playDrum(inst.drumType, i, 7);
				delay(300);
				continue;
			}
			opl2_playNote(0, i+ 3, 0);
			delay(200);
			opl2_setKeyOn(0, false);
			delay(10);
			opl2_playNote(0, i + 3, 7);
			delay(200);
			opl2_setKeyOn(0, false);
			delay(10);
			if (kbhit()) {
				char ch = getch();
				switch (ch) {
					case 27: escape=true; break;
					case 'r': opl2_reset(); break;
				}
			}
		}
		if (escape) {
			break;
		}
	}

	delay(1000);
	opl2_reset();
	return 0;
}
