#if !defined(__DOS__)
	void delay(int) { /* stub */ }
	bool kbhit() { return true; }
	int getch() { return 0; }
	int inp(unsigned short) { return 0; }
	void outp(int, int) { /* stub */ };
#endif
