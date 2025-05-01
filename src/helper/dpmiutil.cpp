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
//   https://www.delorie.com/djgpp/doc/dpmi/
//   https://www.delorie.com/djgpp/doc/ug/interrupts/inthandlers2.html

// get code segment (usually should return 0 in DOS4GW)
uint16_t dpmi_get_cs();
#pragma aux dpmi_get_cs = \
	"mov ax,cs" \
	modify [ ax ] \
	value [ ax ];

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
		regs.h.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.x.edx;
		ivec.selector = regs.w.cx;
	#endif
	return ivec;
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
		regs.h.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.w.dx;
		ivec.segment = regs.w.cx;
	#endif
	return ivec;
}

void dpmi_set_rmode_intvector(uint8_t interrupt_number, rm_address_t ivec)
{
	#if defined __DOS__ && defined __386__
		union REGS regs;

		regs.w.ax = 0x0200;
		regs.h.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		ivec.offset = regs.w.dx;
		ivec.segment = regs.w.cx;
	#endif
}


void dpmi_set_pmode_intvector(uint8_t interrupt_number, pm_address_t addr) {
	// TODO!
	#if defined __DOS__ && defined __386__
		union REGS regs;

		regs.w.ax = 0x0205;
		regs.h.bl	= interrupt_number;
		int386(0x31, &regs, &regs);

		//ivec.offset = regs.x.edx;
		//ivec.selector = regs.w.cx;
	#endif
}

/**
 * DPMI Lock linear region (Function call 0x600)
 * Memory to be used by interrupt handlers should be locked.
 */
int dpmi_lock_linear_region(uint32_t linear_addr, uint32_t size) {
	#if defined __DOS__ && defined __386__
	union REGS regs;
	struct SREGS sregs;

	regs.w.ax = 0x0600;
	regs.w.bx = (uint16_t) (linear_addr >> 16) & 0xffff;
	regs.w.cx = (uint16_t) (linear_addr & 0xffff);
	regs.w.si = (uint16_t) (size >> 16) & 0xffff;
	regs.w.di = (uint16_t) size & 0xffff;

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
 * DPMI unlock linear region (Function Call 0x601)
 */
int dpmi_unlock_linear_region(uint32_t linear_addr, uint32_t size) {
	#if defined __DOS__ && defined __386__
	union REGS regs;
	struct SREGS sregs;

	regs.w.ax = 0x0601;
	regs.w.bx = (uint16_t) (linear_addr >> 16) & 0xffff;
	regs.w.cx = (uint16_t) linear_addr & 0xffff;
	regs.w.si = (uint16_t) (size >> 16) & 0xffff;
	regs.w.di = (uint16_t) size & 0xffff;

	int386(0x31, &regs, &regs);

	if (regs.x.cflag) {
		return regs.w.ax;
	}
	return 0;
	#endif
	return -1;
}

/**
 * Disable Virtual interrupt Flag (Function Call 0x900)
 * DPMI version of "CLI" cpu instruction
 */
void dpmi_disable_virtual_interrupt() {
	#if defined __DOS__ && defined __386__
	union REGS regs;

	regs.w.ax = 0x0900;
	int386(0x31, &regs, &regs);
	#endif
}

/**
 * Enable Virtual interrupt Flag (Function Call 0x901)
 * DPMI version of "STI" cpu instruction
 */
void dpmi_enable_virtual_interrupt() {
	#if defined __DOS__ && defined __386__
	union REGS regs;

	regs.w.ax = 0x0901;
	int386(0x31, &regs, &regs);

	#endif
}

/**
 * Indicate that the program is idle (for example, waiting for keyboard input)
 * This allows the DPMI host to pass the CPU to other clients,
 * or take power-conserving measures.
 */
void dpmi_release_current_time_slice()
{
	#if defined __DOS__ && defined __386__
	union REGS regs;
	regs.w.ax= 0x1680;
	int386(0x2f, &regs, &regs);
	#endif
}

int dpmi_get_segment_base_address(uint16_t selector, uint32_t *base_address) {
	#if defined __DOS__ && defined __386__
	union REGS regs;

	regs.w.ax = 0x0006;
	regs.w.bx = selector;
	int386(0x31, &regs, &regs);
	if (regs.x.cflag) {
		return -1;
	}
	*base_address = ((uint32_t)regs.w.cx) << 16 + ((uint32_t)regs.w.dx);
	return 0;
	#endif
	return -1;
}

int dpmi_lock_code(uint32_t lockaddr, uint32_t locksize)
{
	uint32_t baseaddr = 0;
	if (dpmi_get_segment_base_address(dpmi_get_cs(), &baseaddr) == -1) {
		return -1;
	}
	uint32_t addr = baseaddr + lockaddr;
	if (dpmi_lock_linear_region(addr, locksize) == -1) {
		return -1;
	}
	return 0;
}

int dpmi_unlock_code(uint32_t lockaddr, uint32_t locksize)
{
	uint32_t baseaddr = 0;
	if (dpmi_get_segment_base_address(dpmi_get_cs(), &baseaddr) == -1) {
		return -1;
	}
	uint32_t addr = baseaddr + lockaddr;
	if (dpmi_unlock_linear_region(addr, locksize) == -1) {
		return -1;
	}
	return 0;
}

/**
 * Allocate a "real mode callback" (Function 0x303)
 * a function pointer to a stub that calls back to protected mode
 * limited resource, don't forget to free.
 */
int dpmi_alloc_real_mode_callback(const interrupt_func_t pm_func, const dos_block_t rm_buffer, rm_address_t* result)
{
	#if defined __DOS__ && defined __386__
	union REGS regs;
	struct SREGS sregs;
	regs.w.ax = 0x303;
	sregs.ds = FP_SEG(pm_func);
	regs.w.si = FP_OFF(pm_func);
	sregs.es = rm_buffer.selector;
	regs.w.di = 0;
	int386x(0x31, &regs, &regs, &sregs);
	if (regs.x.cflag) {
		return -1;
	}
	result->segment = regs.w.cx;
	result->offset = regs.w.dx;
	return 0;
	#endif
	return -1;
}

/**
 * Free real mode callback (Function 0x304)
 */
int dpmi_free_real_mode_callback(rm_address_t callback)
{
	#if defined __DOS__ && defined __386__
	union REGS regs;

	regs.w.ax = 0x304;
	regs.w.cx = callback.segment;
	regs.w.dx = callback.offset;

	int386x(0x31, &regs, &regs);
	if (regs.x.cflag) {
		return -1;
	}
	return 0;
	#endif
	return -1;
}
