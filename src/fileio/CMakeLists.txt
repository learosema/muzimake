add_library(fileio STATIC
    fileio.c
    file.c
    bnkfile.c
    rolfile.c
    pathutil.c
)
target_include_directories(fileio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fileio PRIVATE datatype)
