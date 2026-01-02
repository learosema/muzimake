set(NAME log)
add_library(${NAME} STATIC log.c)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
