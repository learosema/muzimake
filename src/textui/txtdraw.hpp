#pragma once
#include <txtbuffer.hpp>
#include <stdint.h>

class TextDraw
{
public:
    TextDraw(TextBuffer &txtBuffer);
    virtual ~TextDraw();

    uint8_t GetChar(const int x, const int y);
    uint8_t GetColor(const int x, const int y);
    void PutChar(const int x, const int y, const uint8_t ch);
    void PutCharColor(const int x, const int y, const uint8_t ch, const uint8_t color);
    
    void HLine(const int x, const int width, const uint8_t ch);
    void HLineColor(const int x, const int width, const uint8_t ch);

private:
    TextBuffer m_txtBuffer;
};