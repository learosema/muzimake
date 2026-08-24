set(NAME "filefmts")
add_library(${NAME} STATIC
	  dir.c
    fileio.c
    file.c
    bnkfile.c
    rolfile.c
    pathutil.c
)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${NAME} PRIVATE datatype)

add_executable(testffmt testinst.c)
target_link_libraries(testffmt PRIVATE datatype filefmts greatest)
add_test(NAME testffmt COMMAND testffmt)

