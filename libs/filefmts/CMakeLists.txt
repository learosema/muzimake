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

# ------- testinst

add_executable(testinst testinst.c)
target_link_libraries(testinst PRIVATE datatype filefmts greatest)
add_test(NAME testinst COMMAND testffmt ${PROJECT_SOURCE_DIR}/assets)

# target_compile_options(testffmt PRIVATE -fsanitize=address -fno-omit-frame-pointer -g -O0)
# target_link_options(testffmt PRIVATE -fsanitize=address)

# ------ testpath

add_executable(testpath testpath.c)

target_link_libraries(testpath PRIVATE
	datatype filefmts greatest
)

set(DEEP_DIR_SEGMENT "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa") # 50 Zeichen
add_custom_command(TARGET testpath POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        "$<TARGET_FILE_DIR:testpath>/deep_dir_fixture/${DEEP_DIR_SEGMENT}/${DEEP_DIR_SEGMENT}/${DEEP_DIR_SEGMENT}/${DEEP_DIR_SEGMENT}/${DEEP_DIR_SEGMENT}/${DEEP_DIR_SEGMENT}"
)

add_test(NAME testpath COMMAND testpath)
set_tests_properties(testpath PROPERTIES
    WORKING_DIRECTORY $<TARGET_FILE_DIR:testpath>
)

# ------ testbnk

add_executable(testbnk testbnk.c)
target_link_libraries(testbnk PRIVATE datatype filefmts greatest)

add_custom_command(TARGET testbnk POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${PROJECT_SOURCE_DIR}/assets/STANDARD.BNK
		$<TARGET_FILE_DIR:testbnk>/STANDARD.BNK
)

add_test(NAME testbnk COMMAND testbnk)
set_tests_properties(testbnk PROPERTIES
  WORKING_DIRECTORY $<TARGET_FILE_DIR:testbnk>
)

# ------ testrol

add_executable(testrol testrol.c)
target_link_libraries(testrol PRIVATE datatype filefmts greatest)

add_custom_command(TARGET testrol POST_BUILD COMMAND
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${PROJECT_SOURCE_DIR}/assets/STARTREK.ROL
		$<TARGET_FILE_DIR:testrol>/STARTREK.ROL
)

add_test(NAME testrol COMMAND testrol)
set_tests_properties(testbnk PROPERTIES
  WORKING_DIRECTORY $<TARGET_FILE_DIR:testrol>
)


# target_compile_options(testbnk PRIVATE -fsanitize=address -fno-omit-frame-pointer -g -O0)
# target_link_options(testbnk PRIVATE -fsanitize=address)
