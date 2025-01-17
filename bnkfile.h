#ifndef __BNKFILE_H__
#define __BNKFILE_H__

#define BNK_ERR_OK 0
#define BNK_ERR_READ_FAILED 1
#define BNK_ERR_WRITE_FAILED 2

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static int bnkfile_last_error = 0;

// https://moddingwiki.shikadi.net/wiki/AdLib_Instrument_Bank_Format

typedef struct BNKHeader {
	uint8_t  versionMajor;
	uint8_t  versionMinor;
	char     signature[6];
	uint16_t numInstuments;
	uint16_t totalNumInstruments;
	int32_t  offsetNames;
	int32_t  offsetData;
	char		 pad[8];
} BNKHeader;

typedef struct
{
	uint16_t index;
	uint8_t  isUsed;
	char     name[9];
} BNKEntry;

typedef struct OPLREGS {
	uint8_t ksl;
	uint8_t multiple;
	uint8_t feedback;
	uint8_t attack;
	uint8_t sustain;
	uint8_t eg; /* ="hasSustain" */
	uint8_t decay;
	uint8_t releaseRate;
	uint8_t totalLevel;
	uint8_t am;  /* amplitude modulation/tremolo */
	uint8_t vib; /* vibrato */
	uint8_t ksr; /* key scaling/envelope rate = "hasEnvelopeScaling"*/
	uint8_t con; /* connector */
} OPLREGS;

typedef struct BNKInstrument {
	uint8_t isPercussive;
	uint8_t voiceNum; /* 	Voice number (percussive only) */
	OPLREGS oplModulator;
	OPLREGS oplCarrier;
	uint8_t iModWaveSel;
	uint8_t iCarWaveSel;
} BNKInstrument;

typedef struct BNKFile {
	uint8_t *buffer;
	long len;
	BNKHeader * header;
	BNKEntry *entries;
	BNKInstrument *instruments;
} BNKFile;


BNKFile * bnkfile_read(char * filename);
bool bnkfile_write(BNKFile * bnkFile, char *filename);
void bnkfile_free(BNKFile * bnkFile);

#ifdef __cplusplus
}
#endif
#endif
