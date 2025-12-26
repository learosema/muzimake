#pragma once
#include <stdint.h>


class TextBuffer {

public:
    TextBuffer();
    TextBuffer(const int rows, const int cols);
    TextBuffer(TextBuffer &txtBuffer);
    virtual ~TextBuffer();

    TextBuffer Clone();
    
    TextBuffer CopyArea(const int x, const int y, const int width, const int height);
    
    void Resize(const int rows, const int cols);

    inline uint8_t *Ptr() const { return m_buffer; }
    inline int Width() const { return m_width; }
    inline int Height() const { return m_height; }
    
private:
    TextBuffer(const unsigned char *buffer, const unsigned int rows, const unsigned int cols);
    uint8_t *m_buffer;
    unsigned int m_width;
    unsigned int m_height;
};
