#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

#include "textmode.h"
#include "vendor/cp437.h"

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define CLAMP(X,A,B) MIN(MAX(A,X),B)

MODEINFO g_currentMode = {0};

void _retrieve_modeinfo()
{
	union REGS regs;
	regs.w.ax = 0x0f00;
	INTR(0x10, &regs, &regs);
	g_currentMode.mode = (uint8_t)regs.h.al;
	g_currentMode.numCols = (uint8_t)regs.h.ah;
	g_currentMode.numRows = 25;
	g_currentMode.page = (uint8_t)regs.h.bh;
	g_currentMode.pageSize=PAGE_SIZE_80X25;
	g_currentMode.videoPortAddress = *(BIOS_VIDEO_PORT_ADDRESS);
	g_currentMode.hasColors = (g_currentMode.videoPortAddress == 0x3d4);
	g_currentMode.vram = g_currentMode.hasColors ?
		TEXT_VRAM_BASE : TEXT_VRAM_BASE_MONO;
}

void textmode_setmode(uint8_t mode)
{
	union REGS regs;
	if (mode > 3 && mode != 7) {
		printf("mode: %d not supported by library. Defaulting to mode 3\n");
		mode = 3;
	}
	regs.h.ah = 0;
	regs.h.al = mode;
	INTR(0x10, &regs, &regs);
	_retrieve_modeinfo();
}

MODEINFO *textmode_get_modeinfo()
{
	return &g_currentMode;
}

void textmode_font8()
{
	union REGS regs;
	if (g_currentMode.mode == 3 && g_currentMode.hasColors == false) {
		// Hercules/MDA won't support switching to 50 lines
		return;
	}
	regs.w.ax = 0x1112;
	regs.w.bx = 0;
	INTR(0x10, &regs, &regs);
	g_currentMode.numRows = 50;
	g_currentMode.pageSize= PAGE_SIZE_80X50;
}

void textmode_set_page(uint8_t page)
{
	if (g_currentMode.pageSize == PAGE_SIZE_80X25 && page >= 8) {
		page = page % 8;
	}

	if (g_currentMode.pageSize == PAGE_SIZE_80X25 && page >= 4) {
		page = page % 4;
	}

	union REGS regs;
	regs.h.ah = 0x05;
	regs.h.al =
	INTR(0x10, &regs, &regs);
	g_currentMode.page = page;
}

void textmode_cursor(uint8_t startRow, uint8_t endRow)
{
	union REGS regs;
	regs.h.ah = 0x01;
	regs.h.ch = startRow;
	regs.h.cl = endRow;
	INTR(0x10, &regs, &regs);
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

void textmode_print(char *str, int x, int y, uint8_t color)
{
	VRAMPTR ptr;
	uint8_t i;
	uint8_t x0 = (uint8_t)(MAX(0, x));
	uint8_t len = strlen(str);
	if ((y < 0) || (y >= g_currentMode.numRows))
	{
		return;
	}
	ptr = TEXT_VRAM + (uint16_t)(y*g_currentMode.numCols*2+x*2);
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

void textmode_box(int x, int y, uint8_t width, uint8_t height, uint8_t color)
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

	if (width > 2 && height > 2) {
		textmode_fill_area(x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}


void textmode_dblbox(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
)
{
	uint8_t i;

	textmode_hline(
		x, y,1,
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

	if (width > 2 && height > 2) {
		textmode_fill_area(x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}

void textmode_gotoxy(uint8_t x, uint8_t y) {
	union REGS regs;

	regs.h.ah = 0x02;
	regs.h.bh = 0;
	regs.h.dl = x;
	regs.h.dh = y;
	INTR(0x10, &regs, &regs);
}
