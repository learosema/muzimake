#ifndef __ASMSTUFF_H__
#define __ASMSTUFF_H__

#if defined __DOS__ && defined __WATCOM__
inline void asm_rep_movsd(void *src, void *dest, int num_dwords);
#pragma aux asm_rep_movsd =     \
    "cld"                   \
    "rep movsd"             \
    parm [esi] [edi] [ecx]  \
    modify [esi edi ecx];


inline void asm_rep_movsw(void *src, void *dest, int num_words);
#pragma aux asm_rep_movsw =     \
    "cld"                   \
    "rep movsw"             \
    parm [esi] [edi] [ecx]  \
    modify [esi edi ecx];


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
#define asm_rep_movsd(...)
#define asm_sti(...)
#define asm_cli(...)
#define asm_end_of_interrupt()
#endif
#endif
