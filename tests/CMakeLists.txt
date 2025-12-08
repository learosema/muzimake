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
	${SRC_DIR}/app
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

add_executable(test_ui ${LIB_CORE_SRCS} ${LIB_UI_SRCS} ${SRC_DIR}/helper/log.cpp test_ui.cpp)

add_executable(test_cmp ${LIB_CORE_SRCS} ${LIB_UI_SRCS} test_cmp.cpp ${VENDOR_DIR}/greatest.h ${SRC_DIR}/helper/log.cpp)

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

add_executable(test_mainui
	${LIB_CORE_SRCS}
	${LIB_UI_SRCS}
	${SRC_DIR}/app/mainui.cpp
	test_mainui.cpp
	${VENDOR_DIR}/greatest.h
)

target_include_directories(test_mainui PRIVATE ${INCLUDE_DIRS})

add_executable(test_event_queue
    ${LIB_CORE_SRCS}
    ${LIB_UI_SRCS}
    ${SRC_DIR}/app/mainui.cpp
    test_event_queue.cpp
    ${VENDOR_DIR}/greatest.h
)

target_include_directories(test_event_queue PRIVATE ${INCLUDE_DIRS})
add_test(NAME test_event_queue COMMAND test_event_queue)

add_executable(test_event_queue_edge
	${LIB_CORE_SRCS}
	${LIB_UI_SRCS}
	${SRC_DIR}/app/mainui.cpp
	test_event_queue_edge.cpp
	${VENDOR_DIR}/greatest.h
)

target_include_directories(test_event_queue_edge PRIVATE ${INCLUDE_DIRS})
add_test(NAME test_event_queue_edge COMMAND test_event_queue_edge)

if(DEBUG_LOGS)
	set(_test_targets
		test_evt
		test_rol
		test_bnk
		testinst
		test_tmr
		test_kbd
		test_mou
		test_fnt
		test_lst
		test_pth
		test_ui
		test_cmp
		testfile
		test_mainui
		test_event_queue
		test_event_queue_edge
	)
	foreach(tgt IN LISTS _test_targets)
		if(TARGET ${tgt})
			target_compile_definitions(${tgt} PRIVATE DEBUG_LOGS=1)
		endif()
	endforeach()
endif()

# Register test executables with CTest so `ctest` will discover and run them.
# Use generator expressions to refer to the built target files.
if(TARGET test_evt)
	add_test(NAME test_evt COMMAND $<TARGET_FILE:test_evt>)
endif()
if(TARGET test_rol)
	add_test(NAME test_rol COMMAND $<TARGET_FILE:test_rol>)
endif()
if(TARGET test_bnk)
	add_test(NAME test_bnk COMMAND $<TARGET_FILE:test_bnk>)
	# Interactive / integration-style test — disable by default for ctest runs.
	set_tests_properties(test_bnk PROPERTIES LABELS "interactive" DISABLED "1")
endif()
if(TARGET testinst)
	add_test(NAME testinst COMMAND $<TARGET_FILE:testinst>)
endif()
if(TARGET test_tmr)
	add_test(NAME test_tmr COMMAND $<TARGET_FILE:test_tmr>)
	# Interactive keyboard test — disable by default so automated runs skip it.
	set_tests_properties(test_tmr PROPERTIES LABELS "interactive" DISABLED "1")
endif()
if(TARGET test_kbd)
	add_test(NAME test_kbd COMMAND $<TARGET_FILE:test_kbd>)
	# Interactive keyboard test — disable by default so automated runs skip it.
	set_tests_properties(test_kbd PROPERTIES LABELS "interactive" DISABLED "1")
endif()
if(TARGET test_mou)
	add_test(NAME test_mou COMMAND $<TARGET_FILE:test_mou>)
	# Interactive keyboard test — disable by default so automated runs skip it.
	set_tests_properties(test_mou PROPERTIES LABELS "interactive" DISABLED "1")
endif()
if(TARGET test_fnt)
	add_test(NAME test_fnt COMMAND $<TARGET_FILE:test_fnt>)
endif()
if(TARGET test_lst)
	add_test(NAME test_lst COMMAND $<TARGET_FILE:test_lst>)
endif()
if(TARGET test_pth)
	add_test(NAME test_pth COMMAND $<TARGET_FILE:test_pth>)
endif()
if(TARGET test_ui)
	add_test(NAME test_ui COMMAND $<TARGET_FILE:test_ui>)
	# Interactive UI test — disable by default so automated runs skip it.
	set_tests_properties(test_ui PROPERTIES LABELS "interactive" DISABLED "1")
endif()
if(TARGET test_cmp)
	add_test(NAME test_cmp COMMAND $<TARGET_FILE:test_cmp>)
endif()
if(TARGET testfile)
	add_test(NAME testfile COMMAND $<TARGET_FILE:testfile>)
endif()
if(TARGET test_mainui)
	add_test(NAME test_mainui COMMAND $<TARGET_FILE:test_mainui>)
endif()
# `test_event_queue` and `test_event_queue_edge` were already registered earlier.
