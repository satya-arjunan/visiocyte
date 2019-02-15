
add_executable(TestPositiveControl testPositiveControl.c)
add_test(TestPositiveControl ${EXECUTABLE_OUTPUT_PATH}/TestPositiveControl)

add_executable(TestNegativeControl testNegativeControl.c)
add_test(TestNegativeControl ${EXECUTABLE_OUTPUT_PATH}/TestNegativeControl)
set_tests_properties(TestNegativeControl PROPERTIES WILL_FAIL TRUE)

get_target_property(VISIOCYTE_EXE_DIR visiocyte RUNTIME_OUTPUT_DIRECTORY)
if(NOT VISIOCYTE_EXE_DIR)
    set(VISIOCYTE_EXE_DIR ${EXECUTABLE_OUTPUT_PATH})
endif()
get_target_property(VISIOCYTE_EXE_NAME visiocyte OUTPUT_NAME)
if(NOT VISIOCYTE_EXE_NAME)
    set(VISIOCYTE_EXE_NAME visiocyte)
endif()
# message("${VISIOCYTE_EXE_DIR} ${VISIOCYTE_EXE_NAME}")
set(VISIOCYTE_EXECUTABLE "${VISIOCYTE_EXE_DIR}/${VISIOCYTE_EXE_NAME}")
add_test(TestVISIOCYTEHelp ${VISIOCYTE_EXECUTABLE} -h)
set_tests_properties(TestVISIOCYTEHelp PROPERTIES PASS_REGULAR_EXPRESSION "analysis platform")

# add_test(read001 ${VISIOCYTE_EXECUTABLE})

