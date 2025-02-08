#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <macros.h>

#include "instrmnt.h"


instrument_map_t instrument_map_create(uint16_t numInstruments)
{
	instrument_map_t map;
	map.numItems = numInstruments;
	map.entries = ALLOC_TYPE(instrument_map_entry_t, numInstruments);
	memset(map.entries, 0,  numInstruments * sizeof(instrument_map_entry_t));

	return map;
}

void instrument_map_resize(instrument_map_t * map, uint16_t numInstruments)
{
	uint16_t oldNumItems = map->numItems;
	map->numItems = numInstruments;
	map->entries = REALLOC_TYPE(map->entries, instrument_map_entry_t, numInstruments);
	if (map->numItems > oldNumItems) {
		uint16_t initCount = map->numItems - oldNumItems;
		instrument_map_entry_t * start = map->entries + oldNumItems * sizeof(instrument_map_entry_t);
		memset(start, 0,  initCount * sizeof(instrument_map_entry_t));
	}
}

void instrument_map_push(instrument_map_t * map, instrument_map_entry_t entry)
{
	instrument_map_resize(map, map->numItems + 1);
	map->entries[map->numItems - 1] = entry;

	instrument_map_sort(map);
}

void instrument_map_dispose(instrument_map_t *map)
{
	free(map->entries);
	map->entries = NULL;
	map->numItems = 0;
}

int instrument_map_compare(const void *a, const void *b)
{
	const instrument_map_entry_t *instA = (instrument_map_entry_t *)a;
	const instrument_map_entry_t *instB = (instrument_map_entry_t *)b;
	return strcmp(instA->name, instB->name);
}

void instrument_map_sort(instrument_map_t * map)
{
	qsort(map->entries, map->numItems, sizeof(instrument_map_entry_t), instrument_map_compare);
}

instrument_map_entry_t * instrument_map_find(instrument_map_t *map, const char name[9])
{
	instrument_map_entry_t key;
	strncpy(key.name, name, 8);
	key.name[8] = '\0';
	void *result = bsearch(&key, map->entries, map->numItems, sizeof(instrument_map_entry_t), instrument_map_compare);
	return (instrument_map_entry_t *)result;
}
