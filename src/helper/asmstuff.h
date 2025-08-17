#ifndef __ASMSTUFF_H__
#define __ASMSTUFF_H__

#if defined __DOS__ && defined __WATCOM__
inline void asm_rep_movsd(void *src, void *dest, int num_dwords);
#pragma aux asm_rep_movsd =     \
    "cld"                   \
    "rep movsd"             \
    parm [esi] [edi] [ecx]  \
    modify [esi edi ecx];

inline void asm_hlt(void);
#pragma aux asm_hlt = \
	"hlt"
#else
#define asm_hlt(...)
#define asm_rep_movsd(...)
#endif
#endif
