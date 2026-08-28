set(NAME textui)
add_library(${NAME} cmponent.c ui_btn.c ui_input.c ui_label.c ui_load.c ui_lsbox.c ui_sheet.c)
target_link_libraries(${NAME} PRIVATE cp437 datatype)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
