#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bnkfile.h"


int main()
{
	bnk_file_t *bnkFile = bnkfile_read("STANDARD.BNK");
	instrument_map_t map = bnkfile_convert_to_map(bnkFile);

	instrument_map_entry_t * entry = instrument_map_find(map, "BDRUM1");
	printf(entry->name);
}
