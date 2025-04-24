#include "dpmiutil.h"
#ifdef __DOS__
#include <dos.h>
#endif

#ifdef __I86__
#warning "dmpiutil.cpp should not be compiled into 16 bit applications."
#endif

// Kudos to
// - the openwatcom manuals: https://openwatcom.org/ftp/manuals/current/pguide.pdf
// - DJ Delorie, who provided a lot of information on DPMI and interrupts:
//   https://www.delorie.com/djgpp/doc/ug/interrupts/inthandlers2.html



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

/**
 * Get Protected Mode Interrupt Vector
 * @param interrupt_number int number
 * @returns protected mode address (selector:32bit-offset) of the given interrupt
 */
pm_address_t dpmi_get_pmode_intvector(uint8_t interrupt_number) {
	pm_address_t ivec = {0, 0};
	#if defined __DOS__ && defined __386__
		union REGS regs;

		regs.w.ax = 0x0204;
		regs.w.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.x.edx;
		ivec.selector = regs.w.cx;
	#endif
}

/**
 * Get Real Mode Interrupt Vector
 * @param
 */
rm_address_t dpmi_get_rmode_intvector(uint8_t interrupt_number) {
	rm_address_t ivec = {0, 0};
	#if defined __DOS__ && defined __386__
		union REGS regs;

		regs.w.ax = 0x0200;
		regs.w.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.x.dx;
		ivec.segment = regs.w.cx;
	#endif
	return ivec;
}

void dpmi_set_pmode_intvector(uint8_t interrupt_number, pm_address_t addr) {
	#if defined __DOS__ && defined __386__
		union REGS regs;

		regs.w.ax = 0x0205;
		regs.w.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.x.edx;
		ivec.selector = regs.w.cx;
	#endif
}

/**
 * DPMI Lock linear region
 */
int dpmi_lock_linear_region(uint32_t linear_addr, uint32_t size) {
	#if defined __DOS__ && defined __386__
	union REGS regs;
	struct SREGS sregs;

	regs.w.ax = 0x0600;
	regs.w.bx = (uint16_t) (linear_addr >> 16) && 0xffff;
	regs.w.cx = (uint16_t) linear_addr & 0xffff;
	regs.w.si = (uint16_t) (size >> 16) 0xffff;
	regs.w.di = (uint16_t) size && 0xffff;

	int386(0x31, &regs, &regs);


	if (regs.x.cflag) {
		return regs.w.ax;
	}
	return 0;

	#endif
	// error: not implemented
	return -1;
}

/**
 * DPMI unlock linear region
 */
int dpmi_unlock_linear_region(uint32_t linear_addr, uint32_t size) {
	#if defined __DOS__ && defined __386__
	union REGS regs;
	struct SREGS sregs;

	regs.w.ax = 0x0601;
	regs.w.bx = (uint16_t) (linear_addr >> 16) && 0xffff;
	regs.w.cx = (uint16_t) linear_addr & 0xffff;
	regs.w.si = (uint16_t) (size >> 16) 0xffff;
	regs.w.di = (uint16_t) size && 0xffff;

	int386(0x31, &regs, &regs);

	if (regs.x.cflag) {
		return regs.w.ax;
	}
	return 0;
	#endif
	return -1;
}
