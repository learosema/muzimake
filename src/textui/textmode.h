#ifndef TEXTMODE_HPP
#define TEXTMODE_HPP

#include <stdint.h>

#if defined __DOS__ && defined __WATCOMC__ && defined __386__
#include <dpmiutil.h>
#endif

#ifdef __DOS__
#ifdef __386__
#define INTR int386
#define VRAMPTR uint8_t *
#define TEXT_VRAM_BASE (uint8_t *)0xb8000
#define TEXT_VRAM_BASE_MONO (uint8_t *)0xb0000
#define BIOS_VIDEO_PORT_ADDRESS (uint16_t *)(0x0463)
#else
#define INTR int86
#define VRAMPTR uint8_t far *
#define BIOS_VIDEO_PORT_ADDRESS (uint16_t far *)(0x00400063)
#define TEXT_VRAM_BASE (uint8_t far *)(0xb8000000)
#define TEXT_VRAM_BASE_MONO (uint8_t far *)(0xb0000000)
#endif
#else
#include <intstubs.h>
#define INTR int86
#define VRAMPTR uint8_t *
#define TEXT_VRAM_BASE (uint8_t *)0x0
#define TEXT_VRAM_BASE_MONO (uint8_t *)0x0
#define BIOS_VIDEO_PORT_ADDRESS (uint16_t *)(0x0)
#endif


#define PAGE_SIZE_80X25 0x1000
#define PAGE_SIZE_80X50 0x2040

#define TEXT_VRAM (g_currentMode.vram+g_currentMode.page*g_currentMode.pageSize)

#define TEXT_GET_CHAR(x, y) (uint8_t) \
	((((x) < 0) || ((x) >= textmode_get_modeinfo()->numCols) || ((y) < 0) || ((y) >= textmode_get_modeinfo()->numRows)) ? 0 : \
	*(textmode_get_modeinfo()->vram + 2 * (textmode_get_modeinfo()->numCols * (y) + (x))))

#define TEXT_GET_COLOR(x, y) (uint8_t) \
	((((x) < 0) || ((x) >= info->numCols) || ((y) < 0) || ((y) >= textmode_get_modeinfo()->numRows)) ? 0 : \
	*(textmode_get_modeinfo()->vram + 1 + 2 * (textmode_get_modeinfo()->numCols * (y) + (x))))




typedef struct MODEINFO
{
	VRAMPTR vram;
	uint8_t mode;
	uint8_t numCols;
	uint8_t numRows;
	uint8_t page;
	uint16_t pageSize;
	bool  hasColors;
	uint16_t videoPortAddress;
} MODEINFO;

typedef struct RECT
{
	int x;
	int y;
	uint8_t width;
	uint8_t height;
} RECT;

typedef struct textbuffer_s
{
	uint8_t width;
	uint8_t height;
	uint8_t *buffer;
} textbuffer_t;

void textmode_setmode(uint8_t mode);

void textmode_init_headless(uint8_t mode);

void textmode_dispose();

MODEINFO *textmode_get_modeinfo();

void textmode_set_page(uint8_t page);

void textmode_font8();

void textmode_cursor(const uint8_t startRow, const uint8_t endRow);

void textmode_clear(uint8_t color);

void textmode_hline(
	int x,
	int y,
	uint8_t width,
	uint8_t character,
	uint8_t color
);

void textmode_vline(
		int x,
		int y,
		uint8_t height,
		uint8_t character,
		uint8_t color);

void textmode_colorize_line(
	int x,
	int y,
	uint8_t width,
	uint8_t color
);

void textmode_fill_area(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t character,
	uint8_t color
);

void textmode_colorize_area(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
);

void textmode_print(const char *str, const int x, const int y, const uint8_t color);

uint8_t textmode_printn_color(const char *str, uint8_t len, int x, int y, uint8_t color);

uint8_t textmode_printn(const char *str, uint8_t len, int x, int y);

void textmode_putchar(int x, int y, char ch);

void textmode_putchar_color(int x, int y, char ch, uint8_t color);

void textmode_putcolor(int x, int y, uint8_t color);

char textmode_getchar(int x, int y);

uint8_t textmode_getcolor(int x, int y);

void textmode_box(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
);

void textmode_dblbox(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
);

void textmode_rect(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
);

void textmode_dblrect(
	int x,
	int y,
	uint8_t width,
	uint8_t height,
	uint8_t color
);

void textmode_init_font(const uint8_t *charData, const uint16_t charHeight, const uint16_t offset, const uint16_t count);

void textmode_gotoxy(const uint8_t x, const uint8_t y);

bool textmode_check_box(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

bool textmode_check_dblbox(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

textbuffer_t textmode_get_area(const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height);

void textmode_put_area(const textbuffer_t * txt_buffer, const uint8_t x, const uint8_t y);

textbuffer_t textmode_get_screen();

void textmode_dispose_buffer(textbuffer_t * txt_buffer);

#endif
