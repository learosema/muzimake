set(NAME "Core")

add_library(${NAME} OBJECT 
    dpmiutil.cpp
    textmode.cpp
    keyboard.cpp
    mouse.cpp
    vga.cpp   
)

target_include_directories(${NAME} PUBLIC textmode.h)