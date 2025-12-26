#include "txtdraw.hpp"
#include "txtbuffer.hpp"
#include "macros.h"

TextDraw::TextDraw(TextBuffer &txtBuffer) : m_txtBuffer(txtBuffer) {
}

uint8_t TextDraw::GetChar(const int x, const int y)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return 0;
    }
    return m_txtBuffer.Ptr()[x * y];
}

uint8_t TextDraw::GetColor(const int x, const int y)
{
    if ((x < 0) || (x >= m_txtBuffer.Width()) || (y < 0) || (y >= m_txtBuffer.Height())) {
        return 0;
    }
    return m_txtBuffer.Ptr()[x * y + 1];
}
