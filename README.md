# MUZIMAKE

Just a plain C project exploring Sound Programming.
Especially the AdLib Sound card.

Contains some code from the <https://github.com/learosema/asciijoy> repository for basic text mode functions.

I want to build a basic composing tool with it.

## AdLib Programming

Rabbit Hole:

- <https://codeberg.org/root42/dosadlib/>
- <https://www.youtube.com/watch?v=kP2B9a7Btpk&list=PLGJnX2KGgaw2L7Uv5NThlL48G9y4rJx1X&index=39>
- <https://cosmodoc.org/topics/adlib-functions/#anyway-heres-the-opl2>
- <https://www.vgmpf.com/Wiki/images/4/48/AdLib_-_Programming_Guide.pdf>
- OPL2 lib used from <https://github.com/Superilu/lib16>

## Building

### 32bit DOS4GW

The only 32 bit platform tested so far is 32Bit DOS with DOS4GW and Watcom C.

Type `wmake`. It will create an ASCIIJOY.EXE and copy
DOS4GW.EXE into the current dir.

## Real mode

Type `wmake /f makefile.16`

### Is DJGPP supported?

Not yet. I need to understand how video ram access works and put together the makefile.
I guess this is a good place to start: <https://www.delorie.com/djgpp/doc/ug/compiling/port16.html>

## Why?

Yeah, there are perfect textmode libraries out there. I still wanted to code
something like that myself, for educational purposes.

Like the hard mode in Monkey Island.
