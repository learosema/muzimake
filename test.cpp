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

	instr->operators[0].attack = bnkInstr.oplModulator.attack & 15;
	instr->operators[0].decay = bnkInstr.oplModulator.decay & 15;
	instr->operators[0].sustain = (bnkInstr.oplModulator.sustain & 15);

	instr->operators[0].release = bnkInstr.oplModulator.releaseRate & 15;
	instr->operators[0].keyScaleLevel = bnkInstr.oplModulator.ksl & 3;
	instr->operators[0].frequencyMultiplier = bnkInstr.oplModulator.multiple & 15;
	instr->operators[0].waveForm = bnkInstr.iModWaveSel & 3;
	instr->operators[0].outputLevel = (bnkInstr.oplModulator.totalLevel & 63);

	instr->operators[0].hasSustain = (bnkInstr.oplModulator.eg & 1) != 0;
	instr->operators[0].hasEnvelopeScaling = (bnkInstr.oplModulator.ksr & 1) !=0;
	instr->operators[0].hasTremolo = (bnkInstr.oplModulator.am & 1) != 0;
	instr->operators[0].hasVibrato = (bnkInstr.oplModulator.vib & 1) != 0;

	instr->operators[1].attack = bnkInstr.oplCarrier.attack & 15;
	instr->operators[1].decay = bnkInstr.oplCarrier.decay & 15;
	instr->operators[1].sustain = (bnkInstr.oplCarrier.sustain & 15);

	instr->operators[1].release = bnkInstr.oplCarrier.releaseRate & 15;
	instr->operators[1].keyScaleLevel = bnkInstr.oplCarrier.ksl & 3;
	instr->operators[1].frequencyMultiplier = bnkInstr.oplCarrier.multiple & 15;
	instr->operators[1].waveForm = bnkInstr.iCarWaveSel & 3;

	instr->operators[1].outputLevel = (bnkInstr.oplCarrier.totalLevel & 63);
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
	int idx = 0, tries =0;
	for (tries = 0; tries < 3; tries++) {
		// retry a couple times before just quitting.
		// sometimes, adlib just has a monday and
		// it works just fine after a retry
		if (!opl2_init()) {
			delay(50);
			continue;
		} else {
			break;
		}
		printf("No adlib present\n");
		return -1;
	}

	bool escape = false;
	opl2_reset();

	BNKFile *bnkFile = bnkfile_read("STANDARD.BNK");

	assert(strncmp(bnkFile->header->signature, "ADLIB-", 6) == 0);
	printf("bankfile loaded: %d instruments\n", bnkFile->header->numInstuments);

	printf("sizeof(bnkinstrument) = %d\n", sizeof(BNKInstrument));
	printf("\n\nNow testing all instruments. Sometimes it can get noisy.\n");
	printf("Then, press R to make this program shut up.\n");
	printf("Or ESC to cancel the test.\n\n");
	opl2_setPercussion(true);

	instrument_t inst = {0};
	for (int j = 0; j < bnkFile->header->numInstuments; j++) {
		// if (!bnkFile->instruments[j].isPercussive) {
		// 	continue;
		// }
		printf("Testing instrument %d: %s", j, bnkFile->entries[j].name);
		idx = bnkFile->entries[j].index;
		printf(" (%s)", bnkFile->instruments[idx].isPercussive ? "drum" : "melo");
		printf(" (%d)", bnkFile->instruments[idx].voiceNum);
		printf(" (con: %d)\n", bnkFile->instruments[idx].oplModulator.con);
		for (size_t k = 0; k < 30; k++) {
			uint8_t *ptr = (uint8_t *)&(bnkFile->instruments[idx]);
			printf("%02x", ptr[k]);
		}
		printf("\n");
		convert_to_instrument(bnkFile, idx, &inst);

		printf("A:%02d D:%02d S:%02d R:%02d K%d L%02d %s%s%s%s | A:%02d D:%02d S:%02d R:%02d K%d L%02d %s%s%s%s FB:%d\n",
			inst.operators[1].attack,
			inst.operators[1].decay,
			15 - inst.operators[1].sustain, // values for sustain are inverted, but adlib composer displays it the other way round
			inst.operators[1].release,
			inst.operators[1].keyScaleLevel,
			63 - inst.operators[1].outputLevel, // same for outputLevel
			inst.operators[1].hasEnvelopeScaling?"E":" ",
			inst.operators[1].hasSustain?"S":" ",
			inst.operators[1].hasTremolo?"T":" ",
			inst.operators[1].hasVibrato?"V":" ",
			inst.operators[0].attack,
			inst.operators[0].decay,
			15 - inst.operators[0].sustain,
			inst.operators[0].release,
			inst.operators[0].keyScaleLevel,
			63 - inst.operators[0].outputLevel,
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
