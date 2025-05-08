#ifdef __DOS__
#pragma aux asm_hlt = \
	"hlt"
#else
inline void asm_hlt(void) {}
#endif
