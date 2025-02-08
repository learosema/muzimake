#include <stdio.h>

#include <file.h>

int main() {

	sound_file_t soundfile = NULL;

	file_result_t result;
	result = file_init(&soundfile, FORMAT_BNK);

	result = file_open(&soundfile, "STANDARD.BNK");

	if(result == ERROR){
		printf("*** Error ***\n");
	}
	else {
		printf("*** Success ***\n");
	}

	//---

	file_dbg(&soundfile);

	//---

	file_close(&soundfile);
}
