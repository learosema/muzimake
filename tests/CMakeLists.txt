set(LIB_CORE_SRCS
	../src/helper/dpmiutil.cpp
	../src/textui/vga.cpp
	../src/textui/mouse.cpp
	../src/textui/keyboard.cpp
	../src/textui/textmode.cpp
	../src/helper/list.cpp
	../src/filefmt/fileio.cpp
	../src/helper/pathutil.cpp
)

set(LIB_UI_SRCS
	../src/textui/ui_event.cpp
	../src/textui/ui_common.cpp
	../src/textui/ui_label.cpp
	../src/textui/ui_btn.cpp
	../src/textui/ui_input.cpp
	../src/textui/ui_lsbox.cpp
	../src/textui/ui_piano.cpp
	../src/textui/ui_range.cpp
	../src/textui/ui_sheet.cpp
	../src/textui/ui_load.cpp
	../src/textui/cmponent.cpp
)


set(INCLUDE_DIRS
	../vendor
	../src
	../src/textui
	../src/events
	../src/filefmt
	../fonts
	../src/hal
	../src/helper
)

add_executable(test_evt events.cpp test_evt.cpp)

add_executable(test_rol
	../src/filefmt/bnkfile.cpp
	../src/filefmt/rolfile.cpp
	../src/helper/list.cpp
	../src/filefmt/fileio.cpp
	../src/filefmt/file.cpp
	../src/hal/instrmnt.cpp
	../src/hal/opl2.cpp
	test_rol.cpp
)

add_executable(test_bnk
	../src/filefmt/bnkfile.cpp
	../src/filefmt/rolfile.cpp
	../src/helper/list.cpp
	../src/filefmt/fileio.cpp
	../src/filefmt/file.cpp
	../src/hal/instrmnt.cpp
	../src/hal/opl2.cpp
	test_bnk.cpp
)

add_executable(testinst
	../src/hal/instrmnt.cpp
	../src/filefmt/bnkfile.cpp
	../src/filefmt/rolfile.cpp
	../src/helper/list.cpp
	../src/filefmt/fileio.cpp
	../src/filefmt/file.cpp
	testinst.cpp
)

add_executable(test_tmr
	../src/helper/dpmiutil.cpp
	../src/textui/textmode.cpp
	../src/helper/timer.cpp
	test_tmr.cpp
)

add_executable(test_kbd
	../src/helper/dpmiutil.cpp
	../src/textui/textmode.cpp
	../src/textui/keyboard.cpp
	../src/textui/mouse.cpp
	test_kbd.cpp
)


add_executable(test_mou
	../src/helper/dpmiutil.cpp
	../src/textui/textmode.cpp
	../src/textui/mouse.cpp
	test_mou.cpp
)

add_executable(test_fnt test_fnt.cpp)

add_executable(test_lst ../src/helper/list.cpp test_lst.cpp)

add_executable(test_pth ../src/helper/pathutil.cpp test_pth.cpp)

add_executable(test_ui ${LIB_CORE_SRCS} ../src/helper/log.cpp test_ui.cpp)

add_executable(test_cmp ${LIB_CORE_SRCS} ../src/helper/log.cpp test_cmp.cpp)

add_executable(testfile
	../src/filefmt/bnkfile.cpp
	../src/filefmt/rolfile.cpp
	../src/helper/list.cpp
	../src/filefmt/fileio.cpp
	../src/filefmt/file.cpp
	../src/hal/instrmnt.cpp
	testfile.cpp
)

target_include_directories(test_evt INCLUDE_DIRS)
target_include_directories(test_rol INCLUDE_DIRS)
target_include_directories(test_bnk INCLUDE_DIRS)
target_include_directories(testinst INCLUDE_DIRS)
target_include_directories(test_tmr INCLUDE_DIRS)
target_include_directories(test_kbd INCLUDE_DIRS)
target_include_directories(test_mou INCLUDE_DIRS)
target_include_directories(test_fnt INCLUDE_DIRS)
target_include_directories(test_lst INCLUDE_DIRS)
target_include_directories(test_pth INCLUDE_DIRS)
target_include_directories(test_ui INCLUDE_DIRS)
target_include_directories(test_cmp INCLUDE_DIRS)
target_include_directories(testfile INCLUDE_DIRS)
