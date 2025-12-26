#include "txtbuffer.hpp"
#include "macros.h"
#include <stddef.h>
#include <string.h>
#define MAX_ROWS 1000
#define MAX_COLS 1000



TextBuffer::TextBuffer()
{
    m_buffer = NULL;
    m_width = 0;
    m_height = 0;
}

TextBuffer::TextBuffer(const int rows, const int cols)
{
    m_width = CLAMP(rows, 0, MAX_ROWS);
    m_height = CLAMP(cols, 0, MAX_COLS);
    m_buffer = new unsigned char[m_width * m_height * 2];
}

TextBuffer::~TextBuffer()
{
    delete m_buffer;
}

TextBuffer TextBuffer::Clone()
{
    return CopyArea(0, 0, m_width, m_height);
}

TextBuffer TextBuffer::CopyArea(const int x, const int y, const int width, const int height)
{
	int clamped_x = MAX(x, 0);
	int clamped_y = MAX(y, 0);
	int clamped_width = MIN(width, m_width) + MIN(x, 0);
	int clamped_height = MIN(height, m_height) + MIN(y, 0);
    uint8_t* buffer = NULL;
    
    if ((clamped_width == 0) || (clamped_height == 0)) {
        return TextBuffer(buffer, 0, 0);
    }
    buffer = new uint8_t[clamped_width * clamped_height * 2];
    uint8_t *src = m_buffer + (clamped_x + clamped_y * m_width) * 2;
    uint8_t *dest = buffer;

    for (uint8_t yy = 0; yy < clamped_height; yy++) {
		memcpy(dest, src, clamped_width * 2);
		src += m_width * 2;
		dest += clamped_width * 2;
	}

    return TextBuffer(buffer, clamped_width, clamped_height);
}

void TextBuffer::Resize(const int rows, const int cols) {
    int new_width = CLAMP(rows, 0, MAX_ROWS);
    int new_height = CLAMP(cols, 0, MAX_COLS);
    uint8_t *new_buffer = new unsigned char[m_width * m_height * 2];
    for (int y = 0; y < m_height; y++) {
        memcpy(new_buffer + new_width * y * 2, m_buffer + m_width * y * 2, m_width * 2);
    }
    delete m_buffer;
    m_buffer = new_buffer;
    m_width = new_width;
    m_height = new_height;
}