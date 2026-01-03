#include "txtbuffer.h"
#include <stdlib.h>
#include <string.h>
#include <macros.h>

void txtbuffer_init(textbuffer_t * const buffer, const uint16_t width, const uint16_t height)
{
	buffer->data = malloc(sizeof(uint8_t) * width * height * 2);
	buffer->width = width;
	buffer->height = height;
}

void txtbuffer_dispose(textbuffer_t * const buffer)
{
	free(buffer->data);
	buffer->data = NULL;
}


void txtbuffer_print(textbuffer_t * const buffer, const char * const str, const int x, const int y)
{
	uint16_t x0 = (uint8_t)(MAX(0, x));
	int len = strlen(str);
	if ((y < 0) || (y >= buffer->height))
	{
		return;
	}
	int start_idx = (uint16_t)(y*buffer->width*2+x*2);
	for (int i = 0; i < len; i++)
	{
		if (x + i < 0)
		{
			continue;
		}
		if (x + i >= buffer->width)
		{
			break;
		}
		buffer->data[start_idx + i * 2] = str[i];
	}
}

/**
 * write string onto buffer
 */
void txtbuffer_print_color(textbuffer_t * const buffer, const char * const str, const int x, const int y, const uint8_t color)
{
	uint16_t x0 = (uint8_t)(MAX(0, x));
	int len = strlen(str);
	if ((y < 0) || (y >= buffer->height))
	{
		return;
	}
	int start_idx = (uint16_t)(y*buffer->width*2+x*2);
	for (int i = 0; i < len; i++)
	{
		if (x + i < 0)
		{
			continue;
		}
		if (x + i >= buffer->width)
		{
			break;
		}
		buffer->data[start_idx + i * 2] = str[i];
		buffer->data[start_idx + i * 2 + 1] = color;
	}
}
/**
 * write string onto buffer but at most n chars
 */
uint8_t txtbuffer_printn_color(textbuffer_t * const buffer, const char * const str, uint8_t len, int x, int y, uint8_t color)
{
	int len = strlen(str); /* TODO .. now bedtime */
	if ((y < 0) || (y >= buffer->height))
	{
		return;
	}
	int start_idx = (uint16_t)(y*buffer->width*2+x*2);
	for (int i = 0; i < len; i++)
	{
		if (x + i < 0)
		{
			continue;
		}
		if (x + i >= buffer->width)
		{
			break;
		}
		buffer->data[start_idx + i * 2] = str[i];
		buffer->data[start_idx + i * 2 + 1] = color;
	}
}

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

