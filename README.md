# MUZIMAKE

Just a plain C project exploring Sound Programming.
Especially the AdLib Sound card.

- Contains some code from the <https://github.com/learosema/asciijoy> repository for basic text mode functions.
- OPL2 lib used from <https://github.com/Superilu/lib16>, but changed the instrument_t struct to also contain a `drumType`

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

Type `wmake`. It will create an MUZIMAKE.EXE and copy
DOS4GW.EXE into the current dir.

### "Unit tests"

There are a couple tests in this project. Well, without using a test framework, just tinier programs testing the functionalities. I didn't get a proper test framework working yet, but there is:

```sh
wmake test     # test bank files (play every instrument, press ESC to quit, R for silence)

wmake test_rol # test rol files (just the file structure)
```

## Real mode

Type `wmake /f makefile.16`

### Is DJGPP supported?

Not yet. I need to understand how video ram access works and put together the makefile.
I guess this is a good place to start: <https://www.delorie.com/djgpp/doc/ug/compiling/port16.html>

## Why?

Yeah, there are perfect textmode libraries out there. I still wanted to code
something like that myself, for educational purposes.

Like the hard mode in Monkey Island.

## Assets

`STANDARD.BNK` is the default instrument bank library shipped with
[AdLib Visual Composer](https://vgmpf.com/Wiki/index.php?title=AdLib_Visual_Composer), (C) 1987 Adlib Inc.

`STARTREK.ROL` contains a minimalistic soundtrack of the Startrek TNG TV series,
created by an unknown artist, originally composed by Dennis McCarthy
