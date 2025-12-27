#pragma once
#include <stdint.h>
#include "txtbuffer.hpp"

class TextDraw
{
public:
    TextDraw(TextBuffer &txtBuffer);
    virtual ~TextDraw();

    uint8_t GetChar(const int x, const int y);
    uint8_t GetColor(const int x, const int y);
    uint16_t GetCharColor(const int x, const int y);

    void PutChar(const int x, const int y, const uint8_t ch);
    void PutColor(const int x, const int y, const uint8_t ch);
    void PutCharColor(const int x, const int y, const uint8_t ch, const uint8_t color);
    
    void HLine(const int x, const int y, const int width, const uint8_t ch);
    void HLineColor(const int x, const int y, const int width, const uint8_t ch, const uint8_t color);

    void VLine(const int x, const int y, const uint8_t height, const uint8_t character);
    void VLineColor(const int x, const int y, const uint8_t height, const uint8_t character, const uint8_t color);

    void PutArea(const TextBuffer &sourceBuffer, const int destX, const int destY);

    void Print(const char *str, const int x, const int y);
    void PrintColor(const char *str, const int x, const int y, const uint8_t ch);

    

private:
    TextBuffer m_txtBuffer;
};