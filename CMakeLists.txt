cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
project(MuziMake LANGUAGES CXX)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(VENDOR_DIR
	${CMAKE_SOURCE_DIR}/vendor/cp437
	${CMAKE_SOURCE_DIR}/vendor/greatest
)
set(FONTS_DIR ${CMAKE_SOURCE_DIR}/fonts)

add_subdirectory(src)
add_subdirectory(tests)
