
add_executable(test_rol test_rol.c)
target_link_libraries(test_rol PRIVATE fileio)
add_custom_command(TARGET test_rol POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/STARTREK.ROL ${CMAKE_CURRENT_SOURCE_DIR}/../build/tests/STARTREK.ROL)
add_test(NAME test_rol COMMAND test_rol)

add_executable(test_lst test_lst.c)
target_link_libraries(test_lst PRIVATE datatype greatest)
add_test(NAME test_lst COMMAND test_lst)

add_executable(test_pth test_pth.c)
target_link_libraries(test_pth PRIVATE fileio greatest)
add_test(NAME test_pth COMMAND test_pth)

