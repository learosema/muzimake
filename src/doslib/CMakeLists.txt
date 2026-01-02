set(NAME doslib)

add_library(${NAME} STATIC
    dpmiutil.c
    textmode.c
    keyboard.c
    mouse.c
    vga.c
    timer.c
    opl2.c
		evt_poll.c
)

target_link_libraries(${NAME} PRIVATE cp437 datatype)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
