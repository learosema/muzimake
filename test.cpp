#include <stdio.h>
#ifdef __DOS__
#include <conio.h>
#endif
#include <string.h>
#include <assert.h>

#include "bnkfile.h"


int main() {
	BNKFile *bnkFile = bnkfile_read("STANDARD.BNK");

	assert(strncmp(bnkFile->header->signature, "ADLIB-", 6) == 0);
	assert(bnkFile->header->totalNumInstruments == 160);
	return 0;
}
