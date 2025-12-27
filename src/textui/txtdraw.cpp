#include "txtdraw.hpp"
#include "txtbuffer.hpp"
#include "macros.h"

#include <cstdint>
#include <string.h>

#include <cp437.h>

TextDraw::TextDraw(TextBuffer &txtBuffer)
{
    m_txtBuffer = txtBuffer;
}

uint8_t TextDraw::GetChar(const int x, const int y)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return 0;
    }
    return m_txtBuffer.Ptr()[(y * m_txtBuffer.Width() + x) * 2];
}

uint8_t TextDraw::GetColor(const int x, const int y)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return 0;
    }
    return m_txtBuffer.Ptr()[(y * m_txtBuffer.Width() + x) * 2 + 1];
}

uint16_t TextDraw::GetCharColor(const int x, const int y)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return 0;
    }
    const uint16_t *ptr = (uint16_t*)m_txtBuffer.Ptr();
    return ptr[(y * m_txtBuffer.Width() + x)];
}

void TextDraw::PutChar(const int x, const int y, const uint8_t ch)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return;
    }
    m_txtBuffer.Ptr()[(y * m_txtBuffer.Width() + x) * 2] = ch;
}

void TextDraw::PutColor(const int x, const int y, const uint8_t color)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return;
    }
    m_txtBuffer.Ptr()[(y * m_txtBuffer.Width() + x) * 2 + 1] = color;
}

void TextDraw::PutCharColor(const int x, const int y, const uint8_t ch, const uint8_t color)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return;
    }
    uint16_t *ptr = (uint16_t*)m_txtBuffer.Ptr();
    ptr[(y * m_txtBuffer.Width() + x)] = (uint16_t)(color<<8) + (uint16_t)(ch);
}

void TextDraw::HLine(const int x, const int y, const int width, const uint8_t ch)
{
    int x0 = (uint8_t)(MAX(0, x));
	int y0 = (uint8_t)(MAX(0, y));
	if ((x0 >= m_txtBuffer.Width()) || (y0 >= m_txtBuffer.Height())) {
		return;
	}
	int w = MIN((uint8_t)(m_txtBuffer.Width() - x0), width);
	int offs = (x0 + y0 * m_txtBuffer.Width()) * 2;
	for (int i = 0; i < w; i++) {
        m_txtBuffer.Ptr()[offs + i * 2] = ch;
    }
}

void TextDraw::HLineColor(const int x, const int y, const int width, const uint8_t ch, const uint8_t color)
{
    int x0 = (uint8_t)(MAX(0, x));
	int y0 = (uint8_t)(MAX(0, y));
	if ((x0 >= m_txtBuffer.Width()) || (y0 >= m_txtBuffer.Height())) {
		return;
	}
	int w = MIN((uint8_t)(m_txtBuffer.Width() - x0), width);
	int offs = (x0 + y0 * m_txtBuffer.Width()) * 2;
	for (int i = 0; i < w; i++) {
        m_txtBuffer.Ptr()[offs + i * 2 + 1] = ch;
        m_txtBuffer.Ptr()[offs + i * 2 + 1] = color;
    }
}

void TextDraw::PutArea(const TextBuffer &sourceBuffer, const int destX, const int destY)
{
    if (destX >= m_txtBuffer.Width() || destY >= m_txtBuffer.Height()) {
        // outside bounds
        return;
    }

    int clamped_x = MAX(destX, 0);
	int clamped_y = MAX(destY, 0);
    uint8_t clamped_width = MIN(sourceBuffer.Width(), m_txtBuffer.Width()) + MIN(destX, 0);
	uint8_t clamped_height = MIN(sourceBuffer.Height(), m_txtBuffer.Height()) + MIN(destY, 0);

	uint8_t *dest = m_txtBuffer.Ptr() +
		(clamped_x + clamped_y * m_txtBuffer.Width()) * 2;
	uint8_t *src = sourceBuffer.Ptr();

	for (uint8_t yy = 0; yy < clamped_height; yy++) {
		memcpy(dest, src, clamped_width * 2);
		dest += (m_txtBuffer.Width() * 2);
		src += (clamped_width * 2);
	}
}

void TextDraw::VLineColor(
		const int x,
		const int y,
		const uint8_t height,
		const uint8_t character,
		const uint8_t color)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((x0 >= m_txtBuffer.Width()) || (y0 >= m_txtBuffer.Height())) {
		return;
	}
	int h = MIN(m_txtBuffer.Height() - x0, height);
	int offs = x0 * 2 + y0 * m_txtBuffer.Width() * 2;

	for (int i = 0; i < h; i++)
	{
        m_txtBuffer.Ptr()[offs + i * m_txtBuffer.Width() * 2] = character;
		m_txtBuffer.Ptr()[offs + i * m_txtBuffer.Width() * 2 + 1] = color;
	}
}

void TextDraw::VLine(
		const int x,
		const int y,
		const uint8_t height,
		const uint8_t character)
{
	int x0 = MAX(0, x);
	int y0 = MAX(0, y);
	if ((x0 >= m_txtBuffer.Width()) || (y0 >= m_txtBuffer.Height())) {
		return;
	}
	int h = MIN(m_txtBuffer.Height() - x0, height);
	int offs = x0 * 2 + y0 * m_txtBuffer.Width() * 2;

	for (int i = 0; i < h; i++)
	{
        m_txtBuffer.Ptr()[offs + i * m_txtBuffer.Width() * 2] = character;
	}
}

void TextDraw::Print(const char *str, const int x, const int y)
{
	int len = strlen(str);
	if ((y < 0) || (y >= m_txtBuffer.Height()))
	{
		return;
	}
	int offs = (uint16_t)((y*m_txtBuffer.Width()+x)*2);
	for (int i = 0; i < len; i++)
	{
		if (x + i < 0)
		{
			continue;
		}
        if (x + i >= m_txtBuffer.Width()) {
            break;
        }
        m_txtBuffer.Ptr()[offs + i * 2] = str[i];
	}
}

void TextDraw::PrintColor(const char *str, const int x, const int y, const uint8_t color)
{
	int len = strlen(str);
	if ((y < 0) || (y >= m_txtBuffer.Height()))
	{
		return;
	}
	int offs = (uint16_t)((y*m_txtBuffer.Width()+x)*2);
	for (int i = 0; i < len; i++)
	{
		if (x + i < 0)
		{
			continue;
		}
        if (x + i >= m_txtBuffer.Width()) {
            break;
        }
        m_txtBuffer.Ptr()[offs + i * 2] = str[i];
        m_txtBuffer.Ptr()[offs + i * 2 + 1] = color;
	}
}

