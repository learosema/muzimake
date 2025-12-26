set(NAME log)
add_library(log OBJECT log.cpp)
target_include_directories(log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
