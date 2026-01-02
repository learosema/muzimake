cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MuziMake LANGUAGES C)

option(DEBUG_LOGS "Enable debug logging (defines DEBUG_LOGS)" OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(VENDOR_DIR
	${CMAKE_SOURCE_DIR}/vendor/cp437
	${CMAKE_SOURCE_DIR}/vendor/greatest
)
set(FONTS_DIR ${CMAKE_SOURCE_DIR}/fonts)

include(cmake/StandardProjectSettings.cmake)
include(GNUInstallDirs)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

enable_testing()

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(tests)

