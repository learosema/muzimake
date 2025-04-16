#include "dpmiutil.h"
#ifdef __DOS__
#include <dos.h>
#endif

dos_block_t dpmi_alloc_dos_block(uint32_t size)
{
	dos_block_t dblk = {0, 0};
	#ifdef __DOS__
		union REGS regs;

    regs.x.eax = 0x0100; // DPMI function: Allocate DOS memory block
    regs.x.ebx = (size + 15) >> 4; // Round up to paragraphs

    int386(0x31, &regs, &regs);

    if (regs.x.cflag)
        return dblk; // Failure, return {0, 0}

    dblk.segment = regs.x.ax;
    dblk.selector = regs.x.dx;
	#endif;
	return dblk;
}

void dpmi_free_dos_block(dos_block_t dblk)
{
	#ifdef __DOS__
    if (dblk.segment == 0)
        return;

    union REGS regs;
    regs.x.eax = 0x0101; // DPMI function: Free DOS memory block
    regs.x.es = dblk.segment;

    int386(0x31, &regs, &regs);
	#endif
}
