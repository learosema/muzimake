add_library(datatype STATIC list.c instrmnt.c)
target_include_directories(datatype PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

