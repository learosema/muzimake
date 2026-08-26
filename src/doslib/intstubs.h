#ifndef __INTSTUBS_H__
#define __INTSTUBS_H__
#if !defined(__DOS__)

struct WORDREGS {
	unsigned short ax;
	unsigned short bx;
	unsigned short cx;
	unsigned short dx;
	unsigned short si;
	unsigned short di;
	unsigned int cflag;
};

/* byte registers */
struct BYTEREGS {
	unsigned char al, ah;
	unsigned char bl, bh;
	unsigned char cl, ch;
	unsigned char dl, dh;
};

union REGS {
	struct WORDREGS w;
	struct BYTEREGS h;
};

inline void int86(int intno, union REGS *inregs, union REGS *outregs) {}

#endif
#endif
