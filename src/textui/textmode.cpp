#include <stdio.h>
#include <stdlib.h>
#if defined __DOS__ && defined __386__
#include <dpmiutil.h>
#include <i86.h>
#endif
#ifdef __DOS__
#include <dos.h>
#include <malloc.h>
#else
#include <intstubs.h>
#endif
#include <string.h>

#include "textmode.h"
#include "vendor/cp437.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define CLAMP(X,A,B) MIN(MAX(A,X),B)

MODEINFO g_currentMode = {0};


void _retrieve_modeinfo_headless(uint8_t mode)
{
	g_currentMode.mode = mode;
	g_currentMode.numCols = (mode == 3) || (mode == 7) ? 80 : 40;
	g_currentMode.numRows = 25;
	g_currentMode.page = 0;
	g_currentMode.pageSize = 160 * 25;
	g_currentMode.videoPortAddress = 0;
	g_currentMode.hasColors = true;
}

void _retrieve_modeinfo(uint8_t mode)
{
	#ifdef __DOS__
	union REGS regs;
	regs.w.ax = 0x0f00;
	INTR(0x10, &regs, &regs);
	g_currentMode.mode = (uint8_t)regs.h.al;
	g_currentMode.numCols = (uint8_t)regs.h.ah;
	g_currentMode.numRows = 25;
	g_currentMode.page = (uint8_t)regs.h.bh;
	g_currentMode.pageSize = PAGE_SIZE_80X25;
	g_currentMode.videoPortAddress = *(BIOS_VIDEO_PORT_ADDRESS);
	g_currentMode.hasColors = (g_currentMode.videoPortAddress == 0x3d4);
	g_currentMode.vram = g_currentMode.hasColors ?
		TEXT_VRAM_BASE : TEXT_VRAM_BASE_MONO;
	#else
	_retrieve_modeinfo_headless(mode);
	#endif
}

#ifdef __DOS__
void _setmode(uint8_t mode)
{
	union REGS regs;
	if (mode > 3 && mode != 7) {
		printf("mode: %d not supported by library. Defaulting to mode 3\n");
		mode = 3;
	}
	regs.h.ah = 0;
	regs.h.al = mode;
	INTR(0x10, &regs, &regs);
}
#else
#define _setmode()
#endif

void textmode_init_headless(uint8_t mode)
{
	#if defined __I86__
	g_currentMode.vram = (uint8_t *)_fmalloc(160 * 25 * 8);
	#else
	g_currentMode.vram = (uint8_t *)malloc(160 * 25 * 8);
	#endif
	_retrieve_modeinfo_headless(mode);
}

void textmode_setmode(uint8_t mode)
{
	#ifdef __DOS__
	_setmode(3);
	_retrieve_modeinfo(3);
	#else
	textmode_init_headless(mode);
	_retrieve_modeinfo(mode);
	#endif
}


void textmode_dispose()
{
	if (g_currentMode.vram != TEXT_VRAM_BASE && g_currentMode.vram != TEXT_VRAM_BASE_MONO) {
		#if defined __386__
		free(g_currentMode.vram);
		#endif
		#if defined __I86__
		_ffree(g_currentMode.vram);
		#endif
	} else {
		_setmode(3);
	}
	g_currentMode.vram = nullptr;
}

MODEINFO *textmode_get_modeinfo()
{
	return &g_currentMode;
}

void textmode_font8()
{
	#ifdef __DOS__
	union REGPACK regs;
	if (g_currentMode.mode == 3 && g_currentMode.hasColors == false) {
		// Hercules/MDA won't support switching to 50 lines
		return;
	}
	regs.w.ax = 0x1112;
	regs.w.bx = 0;
	intr(0x10, &regs);
	#endif
	g_currentMode.pageSize= PAGE_SIZE_80X50;
	g_currentMode.numRows = 50;
}

void textmode_set_page(uint8_t page)
{
	if (g_currentMode.pageSize == PAGE_SIZE_80X25 && page >= 8) {
		page = page % 8;
	}

	if (g_currentMode.pageSize == PAGE_SIZE_80X50 && page >= 4) {
		page = page % 4;
	}
	#ifdef __DOS__
	union REGS regs;
	regs.h.ah = 0x05;
	regs.h.al = page;
	INTR(0x10, &regs, &regs);
	#endif
	g_currentMode.page = page;
}

void textmode_cursor(const uint8_t startRow, const uint8_t endRow)
{
	#ifdef __DOS__
	union REGS regs;
	regs.h.ah = 0x01;
	regs.h.ch = startRow;
	regs.h.cl = endRow;
	INTR(0x10, &regs, &regs);
	#endif
}

void textmode_clear(uint8_t color)
{
	VRAMPTR ptr = TEXT_VRAM;
	uint16_t size = ((uint16_t)g_currentMode.numRows) * 160;
	uint16_t i = 0;

	for (i = 0; i < size; i++)
	{
		*ptr = ' ';
		ptr++;
		*ptr = color;
		ptr++;
	}
}

void textmode_hline(
	int x,
	int y,
	uint8_t width,
	uint8_t character,
	uint8_t color)
{
	uint8_t x0 = (uint8_t)(MAX(0, x));
	uint8_t y0 = (uint8_t)(MAX(0, y));
	if ((x0 >= g_currentMode.numCols) || (y0 >= g_currentMode.numRows)) {
		return;
	}
	uint8_t w = MIN((uint8_t)(g_currentMode.numCols - x0), width);
	VRAMPTR ptr = TEXT_VRAM + x0 * 2 + y0 * g_currentMode.numCols * 2;
	uint8_t i;

	for (i = 0; i < w; i++)
	{
		*ptr = character;
		ptr++;
		*ptr = color;
		ptr++;
	}
}

void textmode_vline(
		int x,
		int y,
		uint8_t height,
		uint8_t character,
		uint8_t color)
{
	uint8_t x0 = (uint8_t)(MAX(0, x));
	uint8_t y0 = (uint8_t)(MAX(0, y));
	if ((x0 >= g_currentMode.numCols) || (y0 >= g_currentMode.numRows)) {
		return;
	}
	uint8_t h = MIN((uint8_t)(g_currentMode.numRows - x0), height);
	VRAMPTR ptr = TEXT_VRAM + x0 * 2 + y0 * g_currentMode.numCols * 2;
	uint8_t i;

	for (i = 0; i < h; i++)
	{
		*ptr = character;
		*(ptr+1) = color;
		ptr+= g_currentMode.numCols * 2;
	}
}

void textmode_colorize_line(
	int x,
	int y,
	uint8_t width,
	uint8_t color
)
{
	uint8_t x0 = (uint8_t)(MAX(0, x));
	uint8_t y0 = (uint8_t)(MAX(0, y));
	if ((x0 >= g_currentMode.numCols) || (y0 >= g_currentMode.numRows)) {
		return;
	}
	uint8_t w = MIN((uint8_t)(g_currentMode.numCols - x0), width);
	VRAMPTR ptr = TEXT_VRAM + x0 * 2 + y0 * g_currentMode.numCols * 2;
	uint8_t i;

	for (i = 0; i < w; i++)
	{
		ptr++;
		*ptr = color;
		ptr++;
	}
}

void textmode_fill_area(
		int x,
		int y,
		uint8_t width,
		uint8_t height,
		uint8_t character,
		uint8_t color)
{
	uint8_t y0 = MAX(0, y);
	uint8_t y1 = MIN(y0 + height - 1, g_currentMode.numRows - 1);
	uint8_t yi;

	for (yi = y0; yi <= y1; yi++)
	{
		textmode_hline(x, yi, width, character, color);
	}
}

void textmode_colorize_area(
		int x,
		int y,
		uint8_t width,
		uint8_t height,
		uint8_t color)
{
	uint8_t y0 = MAX(0, y);
	uint8_t y1 = MIN(y0 + height - 1, g_currentMode.numRows - 1);
	uint8_t yi;

	for (yi = y0; yi <= y1; yi++)
	{
		textmode_colorize_line(x, yi, width, color);
	}
}

void textmode_print(const char *str, const int x, const int y, const uint8_t color)
{
	VRAMPTR ptr;
	uint8_t i;
	uint8_t x0 = (uint8_t)(MAX(0, x));
	uint8_t len = strlen(str);
	if ((y < 0) || (y >= g_currentMode.numRows))
	{
		return;
	}
	ptr = TEXT_VRAM + g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	for (i = 0; i < len; i++)
	{
		if ((x + i < 0) || (x + i >= g_currentMode.numCols))
		{
			ptr += 2;
			continue;
		}
		*ptr = str[i];
		ptr++;
		*ptr = color;
		ptr++;
	}
}

uint8_t textmode_printn_color(const char *str, uint8_t len, int x, int y, uint8_t color)
{
	VRAMPTR ptr;
	uint8_t i;
	uint8_t x0 = (uint8_t)(MAX(0, x));
	bool ended = false;

	if ((y < 0) || (y >= g_currentMode.numRows))
	{
		return 0;
	}
	ptr = TEXT_VRAM + g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);

	for (i = 0; i < len; i++)
	{
		if (str[i] == '\0') {
			ended = true;
		}
		if ((x + i < 0) || (x + i >= g_currentMode.numCols))
		{
			ptr += 2;
			continue;
		}
		*ptr = ended ? ' ' : str[i];
		ptr++;
		*ptr = color;
		ptr++;
	}
	return i;
}

uint8_t textmode_printn(const char *str, uint8_t len, int x, int y)
{
	VRAMPTR ptr;
	uint8_t i;
	uint8_t x0 = (uint8_t)(MAX(0, x));
	bool ended = false;

	if ((y < 0) || (y >= g_currentMode.numRows))
	{
		return 0;
	}
	ptr = TEXT_VRAM + g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);

	for (i = 0; i < len; i++)
	{
		if (str[i] == '\0') {
			ended = true;
		}
		if ((x + i < 0) || (x + i >= g_currentMode.numCols))
		{
			ptr += 2;
			continue;
		}
		*ptr = ended ? ' ' : str[i];
		ptr++;
		ptr++;
	}
	return i;
}

void textmode_putchar(int x, int y, char ch)
{
	VRAMPTR ptr;
	if ((y < 0) || (y >= g_currentMode.numRows) || (x < 0) || (x >= g_currentMode.numCols))
	{
		return;
	}
	ptr = TEXT_VRAM +
		g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	*ptr = ch;
}

void textmode_putcolor(int x, int y, uint8_t color)
{
	VRAMPTR ptr;
	if ((y < 0) || (y >= g_currentMode.numRows) || (x < 0) || (x >= g_currentMode.numCols))
	{
		return;
	}
	ptr = TEXT_VRAM +
		g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	*(ptr+1) = color;
}

void textmode_putchar_color(int x, int y, char ch, uint8_t color)
{
	VRAMPTR ptr;
	if ((y < 0) || (y >= g_currentMode.numRows) || (x < 0) || (x >= g_currentMode.numCols))
	{
		return;
	}
	ptr = TEXT_VRAM +
		g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	*ptr = ch;
	*(ptr+1) = color;
}

char textmode_getchar(int x, int y)
{
	VRAMPTR ptr;
	if ((y < 0) || (y >= g_currentMode.numRows) || (x < 0) || (x >= g_currentMode.numCols))
	{
		return 0;
	}
	ptr = TEXT_VRAM +
		g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	return (*ptr);
}

uint8_t textmode_getcolor(int x, int y)
{
	VRAMPTR ptr;
	if ((y < 0) || (y >= g_currentMode.numRows) || (x < 0) || (x >= g_currentMode.numCols))
	{
		return 0;
	}
	ptr = TEXT_VRAM +
		g_currentMode.page * g_currentMode.pageSize +
		(uint16_t)(y*g_currentMode.numCols*2+x*2);
	return (*ptr + 1);
}

void textmode_rect(int x, int y, uint8_t width, uint8_t height, uint8_t color)
{
	uint8_t i;

	textmode_hline(
		x, y,1,
		CP_THIN_RIGHT_THIN_DOWN, color);

	textmode_hline(
		x + 1, y, width - 2,
		CP_THIN_HORIZONTAL, color);

	textmode_hline(
		x + width - 1, y, 1,
		CP_THIN_LEFT_THIN_DOWN, color);

	textmode_vline(x, y+1, height - 2,
		CP_THIN_VERTICAL, color);

	textmode_vline(x + width -1, y+1, height - 2,
		CP_THIN_VERTICAL, color);

	textmode_hline(
		x, y + height - 1, 1,
		CP_THIN_RIGHT_THIN_UP, color);

	textmode_hline(
		x + 1, y + height - 1, width - 2,
		CP_THIN_HORIZONTAL, color);

	textmode_hline(
		x + width - 1, y + height - 1, 1,
		CP_THIN_LEFT_THIN_UP, color);

}

void textmode_box(int x, int y, uint8_t width, uint8_t height, uint8_t color)
{
	textmode_rect(x, y, width, height, color);
	if (width > 2 && height > 2) {
		textmode_fill_area(x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}

void textmode_dblrect(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
)
{
	uint8_t i;

	textmode_hline(
		x, y, 1,
		CP_THICK_RIGHT_THICK_DOWN, color);

	textmode_hline(
		x + 1, y, width - 2,
		CP_THICK_HORIZONTAL, color);

	textmode_hline(
		x + width - 1, y, 1,
		CP_THICK_LEFT_THICK_DOWN, color);

	textmode_vline(x, y+1, height - 2,
		CP_THICK_VERTICAL, color);

	textmode_vline(x + width -1, y+1, height - 2,
		CP_THICK_VERTICAL, color);

	textmode_hline(
		x, y + height - 1, 1,
		CP_THICK_RIGHT_THICK_UP, color);

	textmode_hline(
		x + 1, y + height - 1, width - 2,
		CP_THICK_HORIZONTAL, color);

	textmode_hline(
		x + width - 1, y + height - 1, 1,
		CP_THICK_LEFT_THICK_UP, color);
}

void textmode_dblbox(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
)
{
	textmode_dblrect(x, y, width, height, color);

	if (width > 2 && height > 2) {
		textmode_fill_area(x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}

void textmode_gotoxy(const uint8_t x, const uint8_t y) {
	#ifdef __DOS__
	union REGS regs;

	regs.h.ah = 0x02;
	regs.h.bh = 0;
	regs.h.dl = x;
	regs.h.dh = y;
	INTR(0x10, &regs, &regs);
	#endif
}

void textmode_init_font(const uint8_t *charData, const uint16_t charHeight, const uint16_t offset, const uint16_t count)
{
	if (charHeight == 8) {
		g_currentMode.numRows = 50;
		g_currentMode.pageSize = (g_currentMode.vram != TEXT_VRAM_BASE && g_currentMode.vram != TEXT_VRAM_BASE_MONO) ? (160 * 25) : PAGE_SIZE_80X50;
	}
	#if defined __DOS__ && defined __386__
	union REGPACK regs = {0};
	uint16_t sizeInBytes = count * charHeight;
	regs.w.ax = 0x1110;
	regs.w.dx = offset;
	regs.w.cx = count;
	regs.h.bh = charHeight & 255;
	regs.h.bl = 0;
	dos_block_t memblock = dpmi_alloc_dos_block(sizeInBytes);
	void *linear_addr = (void*)(memblock.selector<<4);
	dpmi_lock_linear_region(linear_addr, sizeInBytes);
	if (memblock.segment == 0) {
		fprintf(stderr, "Error allocating memory for font. Exiting.\n");
		exit(1);
	}
	uint8_t far *rm_buffer = (uint8_t far *)MK_FP(memblock.selector, 0);
	_fmemcpy(rm_buffer, charData, sizeInBytes);
	regs.w.es = memblock.selector;
	regs.x.ebp = 0x00000000;
	intr(0x10, &regs);
	dpmi_unlock_linear_region(linear_addr, sizeInBytes);
	dpmi_free_dos_block(memblock);
	#endif

	#if defined __DOS__ && defined __I86__
	union REGPACK regs = {0};
	uint16_t sizeInBytes = count * charHeight;
	regs.w.ax = 0x1110;
	regs.w.dx = offset;
	regs.w.cx = count;
	regs.h.bh = charHeight & 255;
	regs.h.bl = 0;
	regs.w.es = FP_SEG(charData);
	regs.w.bp = FP_OFF(charData);
	intr(0x10, &regs);
	#endif
}

bool textmode_check_box(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	char topleft_corner = TEXT_GET_CHAR(info, x, y);
	char topright_corner = TEXT_GET_CHAR(info, x + width - 1, 0);
	char btmleft_corner = TEXT_GET_CHAR(info, y, y + height - 1);
	char btmright_corner = TEXT_GET_CHAR(info, x + width - 1, y + height - 1);
	if (CP_THIN_RIGHT_THIN_DOWN != topleft_corner) return false;
	if (CP_THIN_LEFT_THIN_DOWN != topright_corner) return false;
	if (CP_THIN_RIGHT_THIN_UP != btmleft_corner) return false;
	if (CP_THIN_LEFT_THIN_UP != btmright_corner) return false;
	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THIN_HORIZONTAL != TEXT_GET_CHAR(info, x + i, y)) return false;
		if (CP_THIN_HORIZONTAL != TEXT_GET_CHAR(info, x + i, y + height - 1)) return false;
	}
	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THIN_HORIZONTAL != TEXT_GET_CHAR(info, x, y + i)) return false;
		if (CP_THIN_HORIZONTAL != TEXT_GET_CHAR(info, x + width - 1, y + i)) return false;
	}
	return true;
}

bool textmode_check_dblbox(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	char topleft_corner = TEXT_GET_CHAR(info, x, y);
	char topright_corner = TEXT_GET_CHAR(info, x + width - 1, 0);
	char btmleft_corner = TEXT_GET_CHAR(info, y, y + height - 1);
	char btmright_corner = TEXT_GET_CHAR(info, x + width - 1, y + height - 1);
	if (CP_THICK_RIGHT_THICK_DOWN != topleft_corner) return false;
	if (CP_THICK_LEFT_THICK_DOWN != topright_corner) return false;
	if (CP_THICK_RIGHT_THICK_UP != btmleft_corner) return false;
	if (CP_THICK_LEFT_THICK_UP != btmright_corner) return false;
	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THICK_HORIZONTAL != TEXT_GET_CHAR(info, x + i, y)) return false;
		if (CP_THICK_HORIZONTAL != TEXT_GET_CHAR(info, x + i, y + height - 1)) return false;
	}
	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THICK_HORIZONTAL != TEXT_GET_CHAR(info, x, y + i)) return false;
		if (CP_THICK_HORIZONTAL != TEXT_GET_CHAR(info, x + width - 1, y + i)) return false;
	}
	return true;
}

void textmode_get_area(char *buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	
	if (x >= g_currentMode.numCols || y >= g_currentMode.numRows) {
		// outside bounds
		return;
	}

	uint8_t clamped_x = MAX(x, 0);
	uint8_t clamped_y = MAX(y, 0);
	uint8_t clamped_width = MIN(width, g_currentMode.numCols) + MIN(x, 0);
	uint8_t clamped_height = MIN(height, g_currentMode.numRows) + MIN(y, 0); 

	uint8_t byte_width = clamped_width * 2;
	char *src = g_currentMode.vram + (clamped_x + clamped_y * g_currentMode.numCols) * 2;
	char *dest = buffer;
	
	for (uint8_t yy = 0; yy < clamped_height; yy++) {
		asm_rep_movsw(src, dest, width);
		src += byte_width;
		dest += byte_width;
	}
}

void textmode_put_area(char *buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	MODEINFO* info = textmode_get_modeinfo();
	
	if (x >= g_currentMode.numCols || y >= g_currentMode.numRows) {
		// outside bounds
		return;
	}

	uint8_t clamped_x = MAX(x, 0);
	uint8_t clamped_y = MAX(y, 0);
	uint8_t clamped_width = MIN(width, g_currentMode.numCols) + MIN(x, 0);
	uint8_t clamped_height = MIN(height, g_currentMode.numRows) + MIN(y, 0); 

	uint8_t byte_width = clamped_width * 2;
	char *dest = g_currentMode.vram + (clamped_x + clamped_y * g_currentMode.numCols) * 2;
	char *src = buffer;
	
	for (uint8_t yy = 0; yy < clamped_height; yy++) {
		asm_rep_movsw(src, dest, width);
		src += byte_width;
		dest += byte_width;
	}
}
