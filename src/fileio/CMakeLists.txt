add_library(fileio STATIC 
    fileio.cpp
    file.cpp
    bnkfile.cpp
    rolfile.cpp
    pathutil.cpp
)
target_include_directories(fileio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(fileio PRIVATE datatype)
