#include <stdio.h>
#include <conio.h>

#include "bnkfile.h"

int answer() {
	return 42;
}

int main() {
	BNKFile *bnkFile = bnkfile_read("STANDARD.BNK");
	char ch = getch();
	return 0;
}
