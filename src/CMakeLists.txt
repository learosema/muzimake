set(LIB_CORE_SRCS "dpmiutil.cpp vga.cpp mouse.cpp keyboard.cpp textmode.cpp list.cpp fileio.cpp pathutil.cpp")

set(LIB_UI_SRCS "ui_event.cpp ui_common.cpp ui_label.cpp ui_btn.cpp ui_input.cpp ui_lsbox.cpp ui_piano.cpp ui_range.cpp ui_sheet.cpp ui_load.cpp cmponent.cpp")
LIB_OPL_SRCS=bnkfile.cpp rolfile.cpp file.cpp instrmnt.cpp opl2.cpp

add_executable(MuziMake PRIVATE )
