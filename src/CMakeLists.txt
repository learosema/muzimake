set(LIB_CORE_SRCS
	helper/dpmiutil.cpp
	textui/vga.cpp
	textui/mouse.cpp
	textui/keyboard.cpp
	textui/textmode.cpp
	helper/list.cpp
	helper/log.cpp
	filefmt/fileio.cpp
	helper/pathutil.cpp
)

set(LIB_UI_SRCS
	textui/ui_event.cpp
	textui/ui_common.cpp
	textui/ui_label.cpp
	textui/ui_btn.cpp
	textui/ui_input.cpp
	textui/ui_lsbox.cpp
	textui/ui_piano.cpp
	textui/ui_range.cpp
	textui/ui_sheet.cpp
	textui/ui_load.cpp
	textui/cmponent.cpp
)

set(LIB_OPL_SRCS
	filefmt/bnkfile.cpp
	filefmt/rolfile.cpp
	filefmt/file.cpp
	hal/instrmnt.cpp
	hal/opl2.cpp
)

## Choose app backend by platform: DOS uses `dosapp`, others use `sdlapp`
if(CMAKE_C_COMPILER_ID STREQUAL "OpenWatcom" OR CMAKE_SYSTEM_NAME STREQUAL "DOS")
	set(LIB_APP_SRCS
		app/dosapp.cpp
	)
else()
	set(LIB_APP_SRCS
		app/sdlapp.cpp
	)
endif()

# mainui is application code used by the executable; include it unconditionally
list(APPEND LIB_APP_SRCS app/mainui.cpp)

add_executable(MuziMake ${LIB_CORE_SRCS} ${LIB_UI_SRCS} ${LIB_OPL_SRCS} ${LIB_APP_SRCS} main.cpp)
target_include_directories(MuziMake PRIVATE
	${SRC_DIR}
	${SRC_DIR}/events
	${SRC_DIR}/fonts
	${SRC_DIR}/filefmt
	${SRC_DIR}/hal
	${SRC_DIR}/helper
	${SRC_DIR}/textui
	${SRC_DIR}/app
	${VENDOR_DIR}
	${FONTS_DIR}
)

## On non-DOS builds, find and link SDL2 so `SDLApp` can create a window.
if(NOT (CMAKE_C_COMPILER_ID STREQUAL "OpenWatcom" OR CMAKE_SYSTEM_NAME STREQUAL "DOS"))
	find_package(SDL2 REQUIRED)
	if(SDL2_FOUND)
		target_link_libraries(MuziMake PRIVATE SDL2::SDL2)
		target_include_directories(MuziMake PRIVATE ${SDL2_INCLUDE_DIRS})
	endif()
endif()

if(DEBUG_LOGS)
	target_compile_definitions(MuziMake PRIVATE DEBUG_LOGS=1)
endif()
