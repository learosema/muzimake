set(NAME textui)
add_library(${NAME} txtbuffer.cpp txtdraw.cpp)
target_link_libraries(${NAME} PRIVATE cp437)
