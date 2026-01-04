#include "txtbuffer.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <macros.h>
#include <cp437.h>

void txtbuffer_init(textbuffer_t * const buffer, const uint16_t width, const uint16_t height)
{
	buffer->data = malloc(sizeof(uint8_t) * width * height * 2);
	if (buffer->data) {
		buffer->width = width;
		buffer->height = height;
	} else {
		buffer->width = 0;
		buffer->height = 0;
	}
}

void txtbuffer_dispose(textbuffer_t * const buffer)
{
	if (buffer->data != NULL) {
		free(buffer->data);
	}
	buffer->data = NULL;
	buffer->width = 0;
	buffer->height = 0;
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
	int x0 = (MAX(0, x));
	int len = strlen(str);
	if ((buffer == NULL) || (buffer->data == NULL) || (y < 0) || (y >= buffer->height))
	{
		return;
	}
	int start_idx = y*buffer->width*2+x*2;
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
int txtbuffer_printn_color(textbuffer_t * const buffer, const char * const str, const int len, const int x, const int y, uint8_t color)
{
	int effective_len = MIN(len, strlen(str));
	if ((buffer == NULL) || (buffer->data == NULL) || (y < 0) || (y >= buffer->height))
	{
		return 0;
	}
	int start_idx = (uint16_t)(y*buffer->width*2+x*2);
	int i = 0;
	for (i = 0; i < len; i++)
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
	return (i + 1);
}

/**
 * write string onto buffer but at most n chars
 */
int txtbuffer_printn(textbuffer_t * const buffer, const char * const str, const int len, const int x, const int y)
{
	int effective_len = MIN(len, strlen(str));
	if ((buffer == NULL) || (buffer->data == NULL) || (y < 0) || (y >= buffer->height))
	{
		return 0;
	}
	int start_idx = (uint16_t)(y*buffer->width*2+x*2);
	int i = 0;
	for (i = 0; i < len; i++)
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
	return (i + 1);
}

void txtbuffer_putchar(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch)
{
	if ((buffer == NULL) || (buffer->data == NULL) || (x < 0) || (x >= buffer->width) || (y < 0) || (y >= buffer->height))
	{
		return;
	}
	buffer->data[(y*buffer->width+x)*2] = ch;
}

void txtbuffer_putchar_color(textbuffer_t * const buffer, const int x, const int y, const uint8_t ch, const uint8_t color)
{
	if ((buffer == NULL) || (buffer->data == NULL) || (x < 0) || (x >= buffer->width) || (y < 0) || (y >= buffer->height))
	{
		return;
	}
	int idx = (y * buffer->width + x) * 2;
	buffer->data[idx] = ch;
	buffer->data[idx + 1] = color;
}

void txtbuffer_putcolor(textbuffer_t * const buffer, const int x, const int y, const uint8_t color)
{
	if ((x < 0) || (x >= buffer->width) || (y < 0) || (y >= buffer->height))
	{
		return;
	}
	buffer->data[(y * buffer->width + x) * 2 + 1] = color;
}

uint8_t txtbuffer_getchar(const textbuffer_t * const buffer, const int x, const int y) {
	if ((x < 0) || (x >= buffer->width) || (y < 0) || (y >= buffer->height))
	{
		return 0;
	}
	return buffer->data[(y * buffer->width + x) * 2];
}

uint8_t txtbuffer_getcolor(const textbuffer_t * const buffer, const int x, const int y) {
	if ((x < 0) || (x >= buffer->width) || (y < 0) || (y >= buffer->height))
	{
		return 0;
	}
	return buffer->data[(y * buffer->width + x) * 2 + 1];
}

void txtbuffer_clear(textbuffer_t * const buffer, const uint8_t color)
{
  size_t size = 2 * buffer->width * buffer->height;
  for (size_t i = 0; i < size; i += 2) {
    buffer->data[i] = 32;
    buffer->data[i + 1] = color;
  }
}

void txtbuffer_hline(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const uint8_t character)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((buffer == NULL) || (x0 >= buffer->width) || (y0 >= buffer->height)) {
		return;
	}
	int clamped_width = MIN((buffer->width - x0), width);
	int idx = x0 * 2 + y0 * buffer->width * 2;
	for (int i = 0; i < clamped_width; i++)
	{
		buffer->data[idx + i * 2] = character;
	}
}

void txtbuffer_hline_color(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const uint8_t character,
	const uint8_t color)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((buffer == NULL) || (x0 >= buffer->width) || (y0 >= buffer->height)) {
		return;
	}
	int clamped_width = MIN((buffer->width - x0), width);
	int idx = x0 * 2 + y0 * buffer->width * 2;
	for (int i = 0; i < clamped_width; i++)
	{
		buffer->data[idx + i * 2] = character;
		buffer->data[idx + i * 2+ 1] = color;
	}
}

void txtbuffer_vline(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int height,
	const uint8_t character)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((buffer == NULL) || (x0 >= buffer->width) || (y0 >= buffer->height)) {
		return;
	}
	int clamped_height = MIN((buffer->height - x0), height);
	int idx = x0 * 2 + y0 * buffer->width * 2;
	for (int i = 0; i < clamped_height; i++)
	{
		buffer->data[idx] = character;
		idx+= buffer->width * 2;
	}
}

void txtbuffer_vline_color(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int height,
	const uint8_t character,
	const uint8_t color)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((buffer == NULL) || (x0 >= buffer->width) || (y0 >= buffer->height)) {
		return;
	}
	int clamped_height = MIN((buffer->height - x0), height);
	int idx = x0 * 2 + y0 * buffer->width * 2;
	for (int i = 0; i < clamped_height; i++)
	{
		buffer->data[idx] = character;
		buffer->data[idx + 1] = color;
		idx+= buffer->width * 2;
	}
}

void txtbuffer_colorize_line(
	textbuffer_t * const buffer,
	const int x, const int y,
	const int width, const uint8_t color
)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((buffer == NULL) || (x0 >= buffer->width) || (y0 >= buffer->height)) {
		return;
	}
	uint8_t clamped_width = MIN((buffer->width - x0), width);
	int idx = x0 * 2 + y0 * buffer->width * 2;
	for (int i = 0; i < clamped_width; i++)
	{
		buffer->data[idx + i * 2 + 1] = color;
	}
}

void txtbuffer_fill_area(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const int height,
	const uint8_t character,
	const uint8_t color
) {
	int y0 = MAX(0, y);
	int y1 = MIN(y0 + height - 1, buffer->width - 1);

	for (int yi = y0; yi <= y1; yi++)
	{
		txtbuffer_hline_color(buffer, x, yi, width, character, color);
	}
}

void txtbuffer_colorize_area(
	textbuffer_t * const buffer,
	const int x,
	const int y,
	const int width,
	const int height,
	const uint8_t color
) {
	int y0 = MAX(0, y);
	int y1 = MIN(y0 + height - 1, buffer->width - 1);

	for (int y = y0; y <= y1; y++)
	{
		txtbuffer_colorize_line(buffer, x, y, width, color);
	}
}

void txtbuffer_rect(textbuffer_t * const buffer,
	const int x, const int y,
	const int width, const int height,
	const uint8_t color
) {
	txtbuffer_putchar_color(buffer, x + width - 1, y, CP_THIN_LEFT_THIN_DOWN, color);
	txtbuffer_putchar_color(buffer, x, y, CP_THIN_RIGHT_THIN_DOWN, color);
	txtbuffer_putchar_color(buffer, x, y + height - 1, CP_THIN_RIGHT_THIN_UP, color);
	txtbuffer_putchar_color(buffer, x + width - 1, y + height - 1, CP_THIN_LEFT_THIN_UP, color);
	txtbuffer_hline_color(buffer, x + 1, y, width - 2, CP_THIN_HORIZONTAL, color);
	txtbuffer_vline_color(buffer, x, y+1, height - 2, CP_THIN_VERTICAL, color);
	txtbuffer_vline_color(buffer, x + width -1, y+1, height - 2, CP_THIN_VERTICAL, color);
	txtbuffer_hline_color(buffer, x + 1, y + height - 1, width - 2,CP_THIN_HORIZONTAL, color);
}

void txtbuffer_dblrect(textbuffer_t * const buffer,
	const int x, const int y, const int width, const int height,
	const uint8_t color
) {
	txtbuffer_putchar_color(buffer, x + width - 1, y, CP_THIN_LEFT_THIN_DOWN, color);
	txtbuffer_putchar_color(buffer, x, y, CP_THIN_RIGHT_THIN_DOWN, color);
	txtbuffer_putchar_color(buffer, x, y + height - 1, CP_THIN_RIGHT_THIN_UP, color);
	txtbuffer_putchar_color(buffer, x + width - 1, y + height - 1, CP_THIN_LEFT_THIN_UP, color);
	txtbuffer_hline_color(buffer, x + 1, y, width - 2, CP_THIN_HORIZONTAL, color);
	txtbuffer_vline_color(buffer, x, y+1, height - 2, CP_THIN_VERTICAL, color);
	txtbuffer_vline_color(buffer, x + width -1, y+1, height - 2, CP_THIN_VERTICAL, color);
	txtbuffer_hline_color(buffer, x + 1, y + height - 1, width - 2,CP_THIN_HORIZONTAL, color);
}

void txtbuffer_box(textbuffer_t * const buffer, const int x, const int y, const int width, const int height, const uint8_t color)
{
	txtbuffer_rect(buffer, x, y, width, height, color);

	if (width > 2 && height > 2) {
		txtbuffer_fill_area(buffer, x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}

void txtbuffer_dblbox(textbuffer_t * const buffer, const int x, const int y, const int width, const int height, const uint8_t color)
{
	txtbuffer_dblrect(buffer, x, y, width, height, color);

	if (width > 2 && height > 2) {
		txtbuffer_fill_area(buffer, x + 1, y + 1,
			width - 2, height - 2, ' ', color);
	}
}

bool txtbuffer_check_box(const textbuffer_t * const buffer, const int x, const int y, const int width, const int height)
{
	uint8_t topleft_corner, topright_corner, btmleft_corner, btmright_corner;

	topleft_corner = txtbuffer_getchar(buffer, x, y);
	if (CP_THIN_RIGHT_THIN_DOWN != topleft_corner) return false;

	topright_corner = txtbuffer_getchar(buffer, x + width - 1, 0);
	if (CP_THIN_LEFT_THIN_DOWN != topright_corner) return false;

	btmleft_corner = txtbuffer_getchar(buffer, y, y + height - 1);
	if (CP_THIN_RIGHT_THIN_UP != btmleft_corner) return false;

	btmright_corner = txtbuffer_getchar(buffer, x + width - 1, y + height - 1);
	if (CP_THIN_LEFT_THIN_UP != btmright_corner) return false;

	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THIN_HORIZONTAL != txtbuffer_getchar(buffer, x + i, y)) return false;
		if (CP_THIN_HORIZONTAL != txtbuffer_getchar(buffer, x + i, y + height - 1)) return false;
	}

	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THIN_HORIZONTAL != txtbuffer_getchar(buffer, x, y + i)) return false;
		if (CP_THIN_HORIZONTAL != txtbuffer_getchar(buffer, x + width - 1, y + i)) return false;
	}

	return true;
}

bool txtbuffer_check_dblbox(const textbuffer_t * const buffer, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	uint8_t topleft_corner, topright_corner, btmleft_corner, btmright_corner;

	topleft_corner = txtbuffer_getchar(buffer, x, y);
	if (CP_THICK_RIGHT_THICK_DOWN != topleft_corner) return false;

	topright_corner = txtbuffer_getchar(buffer, x + width - 1, 0);
	if (CP_THICK_LEFT_THICK_DOWN != topright_corner) return false;

	btmleft_corner = txtbuffer_getchar(buffer, y, y + height - 1);
	if (CP_THICK_RIGHT_THICK_UP != btmleft_corner) return false;

	btmright_corner = txtbuffer_getchar(buffer, x + width - 1, y + height - 1);
	if (CP_THICK_LEFT_THICK_UP != btmright_corner) return false;

	for (uint8_t i = 1; i < width - 2; i++) {
		if (CP_THICK_HORIZONTAL != txtbuffer_getchar(buffer, x + i, y)) return false;
		if (CP_THICK_HORIZONTAL != txtbuffer_getchar(buffer, x + i, y + height - 1)) return false;
	}

	for (uint8_t i = 1; i < height - 2; i++) {
		if (CP_THICK_HORIZONTAL != txtbuffer_getchar(buffer, x, y + i)) return false;
		if (CP_THICK_HORIZONTAL != txtbuffer_getchar(buffer, x + width - 1, y + i)) return false;
	}

	return true;
}

textbuffer_t txtbuffer_get_area(
	const textbuffer_t * const src_buffer,
	const int x, const int y, const int width, const int height)
{
	textbuffer_t result = {NULL, 0, 0};

	if (width < 0 || height < 0 || x >= src_buffer->width || y >= src_buffer->height) {
		// negative width or outside bounds
		return result;
	}

	int clamped_x = MAX(x, 0);
	int clamped_y = MAX(y, 0);
	int clamped_width = MIN(width, src_buffer->width) + MIN(x, 0);
	int clamped_height = MIN(height, src_buffer->height) + MIN(y, 0);
	txtbuffer_init(&result, clamped_width, clamped_height);

	uint8_t *src = src_buffer->data + (clamped_x + clamped_y * src_buffer->width) * 2;
	uint8_t *dest = result.data;

	for (uint8_t yy = 0; yy < clamped_height; yy++) {
		memcpy(dest, src, clamped_width * 2);
		src += src_buffer->width * 2;
		dest += clamped_width * 2;
	}

	return result;
}

void txtbuffer_put_area(textbuffer_t * const dest_buffer, const textbuffer_t * const src_buffer, const int x, const int y)
{
		if (src_buffer->width < 0 || src_buffer->height < 0 || x >= dest_buffer->width || y >= dest_buffer->height) {
		// neggative width or outside bounds
		return;
	}

	int clamped_x = MAX(x, 0);
	int clamped_y = MAX(y, 0);
	int clamped_width = MIN(src_buffer->width, dest_buffer->width) + MIN(x, 0);
	int clamped_height = MIN(src_buffer->height, dest_buffer->height) + MIN(y, 0);

	uint8_t *dest = (uint8_t *)dest_buffer->data +
		(clamped_x + clamped_y * dest_buffer->width) * 2;
	uint8_t *src = (uint8_t *)src_buffer->data;

	for (uint8_t yy = 0; yy < clamped_height; yy++) {
		memcpy(dest, src, clamped_width * 2);
		dest += (dest_buffer->width * 2);
		src += (clamped_width * 2);
	}
}
