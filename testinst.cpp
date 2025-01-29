#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "bnkfile.h"
#define EXPECT(str, condition) printf("expect %s: ",str);assert(condition);printf("ok\n");

int main()
{
	bnk_file_t *bnkFile = bnkfile_read("STANDARD.BNK");
	instrument_map_t map = bnkfile_convert_to_map(bnkFile);

	instrument_map_entry_t * entry = instrument_map_find(&map, "BDRUM1");
	EXPECT("to find the instrument BDRUM1: ", strcmp(entry->name, "BDRUM1") == 0);


	instrument_map_entry_t newEntry = {0};
	strcpy(newEntry.name, "000_test");
	uint16_t numItemsExpectedAfterPush = map.numItems + 1;
	printf("num instruments: %d\n", map.numItems);


	printf("adding instrument 000_test to instrument map.\n");
	instrument_map_push(&map, newEntry);

	EXPECT("numItems to be increased by one", map.numItems == numItemsExpectedAfterPush);
	EXPECT("expect the first entry to be 000_test", strcmp(map.entries[0].name, "000_test") == 0);
}
