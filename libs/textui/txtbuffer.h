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


/**
 * Dispose textbuffer
 */
void txtbuffer_dispose(textbuffer_t * const buffer);

/**
 * Clear textbuffer (fill entirely with color)
 */
void txtbuffer_clear(textbuffer_t * const buffer, const uint8_t color);

/**
 * Write a horizontal line of characters into the buffer
 */
void txtbuffer_hline(textbuffer_t * const buffer,
	const int x, const int y, const int width,
	const uint8_t character);

/**
 * Write a horizontal line of characters with a
 * given color into the buffer
 */
void txtbuffer_hline_color(textbuffer_t * const buffer,
	const int x, const int y, const int width,
	const uint8_t character, const uint8_t color);

/**
 * Write a vertical line of characters into the
 * buffer
 */
void txtbuffer_vline(
	textbuffer_t * const buffer,
	const int x, const int y,
	const int height, const uint8_t character);

/**
 * Write a vertical line of characters with a
 * given color into the buffer
 */
void txtbuffer_vline_color(
	textbuffer_t * const buffer,
	const int x, const int y,
	const int height, const uint8_t character,
	const uint8_t color);

/**
 * Colorize a horizontal line with a given color
 */
void txtbuffer_colorize_line(
	textbuffer_t * const buffer,
	const int x, const int y,
	const int width, const uint8_t color);

void txtbuffer_fill_area(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const int height,
	const uint8_t character,
	const uint8_t color);

void txtbuffer_colorize_area(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const int height,
	const uint8_t color
);

void txtbuffer_print(textbuffer_t * const buffer, const char * const str, const int x, const int y);
void txtbuffer_print_color(textbuffer_t * const buffer, const char * const str, const int x, const int y, const uint8_t color);

int txtbuffer_printn_color(textbuffer_t * const buffer, const char * const str, const int len, const int x, const int y, uint8_t color);
int txtbuffer_printn(textbuffer_t * const buffer, const char * const str, const int len, const int x, const int y);
void txtbuffer_putchar(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch);

void txtbuffer_putchar_color(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch, const uint8_t color);

void txtbuffer_putcolor(textbuffer_t * const buffer, const int x, const int y, const uint8_t color);

uint8_t txtbuffer_getchar(const textbuffer_t * const buffer, const int x, const int y);

uint8_t txtbuffer_getcolor(const textbuffer_t * const buffer, const int x, const int y);

void txtbuffer_rect(textbuffer_t * const buffer,
	const int x, const int y,
	const int width, const int height,
	const uint8_t color
);

void txtbuffer_dblrect(textbuffer_t * const buffer,
	const int x, const int y, const int width, const int height,
	const uint8_t color
);

void txtbuffer_box(textbuffer_t * const buffer, const int x, const int y, const int width, const int height, const uint8_t color);

void txtbuffer_dblbox(textbuffer_t * const buffer, const int x, const int y, const int width, const int height, const uint8_t color);

bool txtbuffer_check_box(const textbuffer_t * const buffer, const int x, const int y, const int width, const int height);
bool txtbuffer_check_dblbox(const textbuffer_t * const buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

/**
 * get a sub area of a source text buffer and write it into a new textbuffer.
 * The result needs to be disposed via txtbuffer_dispose.
 */
textbuffer_t txtbuffer_get_area(
	const textbuffer_t * const src_buffer,
	const int x, const int y, const int width, const int height);

/**
 * Put a buffer into a given destination buffer
 */
void txtbuffer_put_area(textbuffer_t * const dest_buffer,
	const textbuffer_t * const src_buffer,
	const int x, const int y);

#ifdef __cplusplus
}
#endif
#endif
