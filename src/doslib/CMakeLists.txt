set(NAME "DOSLib")

add_library(${NAME} STATIC 
    dpmiutil.cpp
    textmode.cpp
    keyboard.cpp
    mouse.cpp
    vga.cpp   
    timer.cpp
    opl2.cpp
)
target_link_libraries(${NAME} PRIVATE cp437)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})