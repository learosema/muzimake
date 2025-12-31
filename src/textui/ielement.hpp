#pragma once
#include "geometry.h"

class IElement {
public:
    virtual int Id() = 0;
    virtual char *Name() = 0;
    virtual bool NeedsRepaint() = 0;
    virtual rect_t ClientRect() = 0;
    virtual void Render() = 0;
};

class IFocusable {
public:
    virtual bool Focused() = 0;
    virtual void Focus();
    virtual void Blur();
};
