#if !defined(__DOS__)
	void delay(int) { /* stub */ }
	bool kbhit() { return true; }
	int getch() { return 0; }
	uint8_t inp(int) { return 0; }
	void outp(int, int) { /* stub */ };
#endif
