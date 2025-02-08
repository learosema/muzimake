#ifndef __VGA_H__
#define __VGA_H__

/* VGA ports */
#define PALETTE_INDEX 0x3C8
#define PALETTE_DATA 0x3C9
#define INPUT_STATUS 0x03DA
#define MISC_OUTPUT 0x3C2
#define AC_WRITE 0x3C0
#define AC_READ 0x3C1
#define SC_INDEX 0x3C4
#define SC_DATA 0x3C5
#define GC_INDEX 0x03CE
#define GC_DATA 0x03CF
#define CRTC_INDEX 0x03D4
#define CRTC_DATA 0x03D5

/* VGA status bits */
#define DISPLAY_ENABLE 0x01
#define VRETRACE 0x08

void vga_wait_for_retrace();

#endif
