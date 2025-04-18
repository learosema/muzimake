#include "dpmiutil.h"
#ifdef __DOS__
#include <dos.h>
#endif

#ifdef __I86__
#error "dmpiutil.cpp should not be compiled into 16 bit applications."
#endif

dos_block_t dpmi_alloc_dos_block(uint32_t size)
{
	dos_block_t dblk = {0, 0};
	#if defined __DOS__ && defined __386__
		union REGS regs;

    regs.x.eax = 0x0100; // DPMI function: Allocate DOS memory block
    regs.x.ebx = (size + 15) >> 4; // Round up to paragraphs

    int386(0x31, &regs, &regs);

    if (regs.x.cflag)
        return dblk; // Failure, return {0, 0}

    dblk.segment = regs.w.ax;
    dblk.selector = regs.w.dx;
	#endif;
	return dblk;
}

void dpmi_free_dos_block(dos_block_t dblk)
{
	#if defined __DOS__ && defined __386__
    if (dblk.segment == 0) {
			return;
		}

    union REGS regs;

    regs.w.ax = 0x0101;
    regs.w.dx = dblk.selector;

    int386(0x31, &regs, &regs);
	#endif
}
