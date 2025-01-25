#include <stdio.h>
#if defined(__DOS__)
#include <conio.h>
#include <i86.h>
#include <dos.h>
#else
void delay(int);
bool kbhit();
int getch();
#endif
#include <string.h>
#include <assert.h>

#include "bnkfile.h"
#include "opl2.h"

int main()
{
	int idx = 0, tries = 0;
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

	bnk_file_t *bnkFile = bnkfile_read("STANDARD.BNK");

	assert(strncmp(bnkFile->header->signature, "ADLIB-", 6) == 0);
	printf("bankfile loaded: %d instruments\n", bnkFile->header->numInstuments);

	printf("sizeof(bnkinstrument) = %d\n", sizeof(bnk_instrument_t));
	printf("\n\nNow testing all instruments. Sometimes it can get noisy.\n");
	printf("Then, press R to make this program shut up.\n");
	printf("Or ESC to cancel the test.\n\n");
	opl2_setPercussion(true);

	instrument_t inst = {0};
	for (int j = 0; j < bnkFile->header->numInstuments; j++) {
		inst = bnk_convert_to_instrument(bnkFile->instruments[j]);
		printf("Testing instrument %d: %s", j, bnkFile->entries[j].name);
		idx = bnkFile->entries[j].index;
		printf(" (%s %s)\n", inst.isPercussive ? "drum" : "melo", inst.isAdditiveSynth?"as":"fm");

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
