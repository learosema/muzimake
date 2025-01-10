#ifndef __BNKFILE_H__
#define __BNKFILE_H__

#include <stdint.h>

// https://moddingwiki.shikadi.net/wiki/AdLib_Instrument_Bank_Format

typedef struct BNKHeader {
	uint8_t  versionMajor;
	uint8_t  versionMinor;
	char     signature[6];
	uint16_t numInstuments;
	uint16_t totalNumInstruments;
	int32_t  offsetNames;
	int32_t  offsetData;
} BNKHeader;

typedef struct
{
	uint16_t offsetData;
	uint8_t  isUsed;
	char     name[9];
} BNKEntry;

typedef struct OPLREGS {
	uint8_t ksl;
	uint8_t multiple;
	uint8_t feedback;
	uint8_t attack;
	uint8_t sustain;
	uint8_t eg;
	uint8_t decay;
	uint8_t releaseRate;
	uint8_t totalLevel;
	uint8_t am;
	uint8_t vib;
	uint8_t ksr;
	uint8_t con;
} OPLREGS;

typedef struct BNKInstrument {
	uint8_t isPercussive;
	uint8_t voiceNum;
	OPLREGS modulator;
	OPLREGS oplCarrier;
} BNKInstrument;

typedef struct BNKFile {
	BNKHeader *header;
	BNKEntry *entries;
	BNKInstrument *instruments;
} BNKFile;


BNKFile * bnkfile_read(char * filename);
void bnkfile_free(BNKFile * bnkFile);


#endif
