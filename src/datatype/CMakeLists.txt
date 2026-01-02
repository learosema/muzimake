add_library(datatype STATIC list.c instrmnt.c ui_event.h)
target_include_directories(datatype PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
