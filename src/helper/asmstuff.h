#ifndef __ASMSTUFF_H__
#define __ASMSTUFF_H__

#if defined __DOS__ && defined __WATCOMC__
inline void asm_end_of_interrupt();
#pragma aux asm_end_of_interrupt = \
    "mov al,20H"                 \
    "out 20H,al"

inline void asm_cli(void);
#pragma aux asm_cli = \
    "cli"

inline void asm_sti(void);
#pragma aux asm_sti = \
    "sti"

inline void asm_hlt(void);
#pragma aux asm_hlt = \
	"hlt"
#else
#define asm_hlt(...)
#define asm_sti(...)
#define asm_cli(...)
#define asm_end_of_interrupt()
#define asm_rep_movsd(...) memcpy(dest, src, (n_dwords)<<2)
#define asm_rep_movsw(src, dest, n_words) memcpy(dest, src, (n_words)<<1)
#endif
#endif
