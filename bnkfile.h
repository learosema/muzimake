#ifndef __BNKFILE_H__
#define __BNKFILE_H__

#define BNK_ERR_OK 0
#define BNK_ERR_READ_FAILED 1
#define BNK_ERR_WRITE_FAILED 2

#include <stdint.h>
#include "instr.h"

#ifdef __cplusplus
extern "C" {
#endif

static int bnk_last_error = 0;

// https://moddingwiki.shikadi.net/wiki/AdLib_Instrument_Bank_Format

typedef struct __bnk_header
{
	uint8_t  versionMajor;
	uint8_t  versionMinor;
	char     signature[6];
	uint16_t numInstuments;
	uint16_t totalNumInstruments;
	int32_t  offsetNames;
	int32_t  offsetData;
	char		 pad[8];
} bnk_header_t;

typedef struct __bnk_entry
{
	uint16_t index;
	uint8_t  isUsed;
	char     name[9];
} bnk_entry_t;

typedef struct __bnk_operator {
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
} bnk_operator_t;

typedef struct __bnk_instrument {
	uint8_t isPercussive;
	uint8_t voiceNum; /* 	Voice number (percussive only) */
	bnk_operator_t oplModulator;
	bnk_operator_t oplCarrier;
	uint8_t iModWaveSel;
	uint8_t iCarWaveSel;
} bnk_instrument_t;

typedef struct __bnk_file {
	uint8_t *buffer;
	long len;
	bnk_header_t * header;
	bnk_entry_t *entries;
	bnk_instrument_t *instruments;
} bnk_file_t;

bnk_file_t *bnkfile_read(char * filename);
bool bnkfile_write(bnk_file_t * bnkFile, char *filename);
void bnkfile_free(bnk_file_t * bnkFile);
instrument_t bnkfile_convert_to_instrument(bnk_instrument_t bnkInstr);
instrument_map_t bnkfile_convert_to_map(bnk_file_t * bnkFile);

#ifdef __cplusplus
}
#endif
#endif
