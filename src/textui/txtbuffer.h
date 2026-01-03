#ifndef __TXTBUFFER_H__
#define __TXTBUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct textbuffer_s
{
	uint8_t *data;
	uint16_t width;
	uint16_t height;
} textbuffer_t;

/**
 * Init textbuffer
 */
void txtbuffer_init(textbuffer_t * const buffer, const uint16_t width, const uint16_t height);
void txtbuffer_dispose(textbuffer_t * const buffer);

void txtbuffer_print(textbuffer_t * const buffer, const char * const str, const int x, const int y);
void txtbuffer_print_color(textbuffer_t * const buffer, const char * const str, const int x, const int y, const uint8_t color);

uint8_t txtbuffer_printn_color(textbuffer_t * const buffer, const char * const str, uint8_t len, int x, int y, uint8_t color);
uint8_t txtbuffer_printn(textbuffer_t * const buffer, const char * const str, uint8_t len, int x, int y);
void txtbuffer_putchar(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch);

void txtbuffer_putchar_color(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch, const uint8_t color);

void txtbuffer_putcolor(textbuffer_t * const buffer, const int x, const int y, const uint8_t color);

char txtbuffer_getchar(textbuffer_t * const buffer, const int x, const int y);

uint8_t txtbuffer_getcolor(textbuffer_t * const buffer, const int x, const int y);

void txtbuffer_box(textbuffer_t * const buffer, const int x, const int y, const uint8_t width, const uint8_t height, const uint8_t color);

void txtbuffer_dblbox(textbuffer_t * const buffer, const int x, const int y, const uint8_t width,
	const uint8_t height,
	const uint8_t color
);

void txtbuffer_rect(const textbuffer_t * const buffer,
	const int x, const int y,
	const uint8_t width, const uint8_t height,
	const uint8_t color
);

void txtbuffer_dblrect(const textbuffer_t * const buffer,
	const int x, const int y, const uint8_t width, const uint8_t height,
	const uint8_t color
);

bool txtbuffer_check_box(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
bool txtbuffer_check_dblbox(const textbuffer_t * const buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height);
textbuffer_t txtbuffer_get_area(const textbuffer_t * const src_buffer, const uint8_t x, const uint8_t y, const uint8_t width, const uint8_t height);
void txtbuffer_put_area(const textbuffer_t * const src_buffer, textbuffer_t * const dest_buffer, const uint8_t x, const uint8_t y);

#ifdef __cplusplus
}
#endif
#endif
