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
add_test(NAME testffmt COMMAND testffmt ${PROJECT_SOURCE_DIR}/assets)

# target_compile_options(testffmt PRIVATE -fsanitize=address -fno-omit-frame-pointer -g -O0)
# target_link_options(testffmt PRIVATE -fsanitize=address)
