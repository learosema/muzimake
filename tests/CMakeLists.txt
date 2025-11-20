set(LIB_CORE_SRCS
	${SRC_DIR}/helper/dpmiutil.cpp
	${SRC_DIR}/textui/vga.cpp
	${SRC_DIR}/textui/mouse.cpp
	${SRC_DIR}/textui/keyboard.cpp
	${SRC_DIR}/textui/textmode.cpp
	${SRC_DIR}/helper/list.cpp
	${SRC_DIR}/filefmt/fileio.cpp
	${SRC_DIR}/helper/pathutil.cpp
)

set(LIB_UI_SRCS
	${SRC_DIR}/textui/ui_event.cpp
	${SRC_DIR}/textui/ui_common.cpp
	${SRC_DIR}/textui/ui_label.cpp
	${SRC_DIR}/textui/ui_btn.cpp
	${SRC_DIR}/textui/ui_input.cpp
	${SRC_DIR}/textui/ui_lsbox.cpp
	${SRC_DIR}/textui/ui_piano.cpp
	${SRC_DIR}/textui/ui_range.cpp
	${SRC_DIR}/textui/ui_sheet.cpp
	${SRC_DIR}/textui/ui_load.cpp
	${SRC_DIR}/textui/cmponent.cpp
)


set(INCLUDE_DIRS
	${VENDOR_DIR}
	${SRC_DIR}
	${SRC_DIR}/textui
	${SRC_DIR}/events
	${SRC_DIR}/filefmt
	${FONTS_DIR}
	${SRC_DIR}/hal
	${SRC_DIR}/helper
)

add_executable(test_evt ${SRC_DIR}/events/events.cpp test_evt.cpp)

add_executable(test_rol
	${SRC_DIR}/filefmt/bnkfile.cpp
	${SRC_DIR}/filefmt/rolfile.cpp
	${SRC_DIR}/helper/list.cpp
	${SRC_DIR}/filefmt/fileio.cpp
	${SRC_DIR}/filefmt/file.cpp
	${SRC_DIR}/hal/instrmnt.cpp
	${SRC_DIR}/hal/opl2.cpp
	test_rol.cpp
)

add_executable(test_bnk
	${SRC_DIR}/filefmt/bnkfile.cpp
	${SRC_DIR}/filefmt/rolfile.cpp
	${SRC_DIR}/helper/list.cpp
	${SRC_DIR}/filefmt/fileio.cpp
	${SRC_DIR}/filefmt/file.cpp
	${SRC_DIR}/hal/instrmnt.cpp
	${SRC_DIR}/hal/opl2.cpp
	test_bnk.cpp
)

add_executable(testinst
	${SRC_DIR}/hal/instrmnt.cpp
	${SRC_DIR}/filefmt/bnkfile.cpp
	${SRC_DIR}/filefmt/rolfile.cpp
	${SRC_DIR}/helper/list.cpp
	${SRC_DIR}/filefmt/fileio.cpp
	${SRC_DIR}/filefmt/file.cpp
	testinst.cpp
)

add_executable(test_tmr
	${SRC_DIR}/helper/dpmiutil.cpp
	${SRC_DIR}/textui/textmode.cpp
	${SRC_DIR}/helper/timer.cpp
	test_tmr.cpp
)

add_executable(test_kbd
	${SRC_DIR}/helper/dpmiutil.cpp
	${SRC_DIR}/textui/textmode.cpp
	${SRC_DIR}/textui/keyboard.cpp
	${SRC_DIR}/textui/mouse.cpp
	test_kbd.cpp
)


add_executable(test_mou
	${SRC_DIR}/helper/dpmiutil.cpp
	${SRC_DIR}/textui/textmode.cpp
	${SRC_DIR}/textui/mouse.cpp
	test_mou.cpp
)

add_executable(test_fnt test_fnt.cpp)

add_executable(test_lst test_lst.cpp ${VENDOR_DIR}/greatest.h ${SRC_DIR}/helper/list.cpp)

add_executable(test_pth ${SRC_DIR}/helper/pathutil.cpp test_pth.cpp)

add_executable(test_ui ${LIB_CORE_SRCS} ${SRC_DIR}/helper/log.cpp test_ui.cpp)

add_executable(test_cmp ${LIB_CORE_SRCS} test_cmp.cpp ${VENDOR_DIR}/greatest.h ${SRC_DIR}/helper/log.cpp)

add_executable(testfile
	${SRC_DIR}/filefmt/bnkfile.cpp
	${SRC_DIR}/filefmt/rolfile.cpp
	${SRC_DIR}/helper/list.cpp
	${SRC_DIR}/filefmt/fileio.cpp
	${SRC_DIR}/filefmt/file.cpp
	${SRC_DIR}/hal/instrmnt.cpp
	testfile.cpp
)

target_include_directories(test_evt PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_rol PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_bnk PRIVATE ${INCLUDE_DIRS})
target_include_directories(testinst PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_tmr PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_kbd PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_mou PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_fnt PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_lst PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_pth PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_ui PRIVATE ${INCLUDE_DIRS})
target_include_directories(test_cmp PRIVATE ${INCLUDE_DIRS})
target_include_directories(testfile PRIVATE ${INCLUDE_DIRS})
