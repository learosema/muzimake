add_subdirectory(log)
add_subdirectory(datatype)
add_subdirectory(fileio)

if (CMAKE_SYSTEM_NAME STREQUAL "DOS")
	add_subdirectory(doslib)
endif()

add_subdirectory(textui)
