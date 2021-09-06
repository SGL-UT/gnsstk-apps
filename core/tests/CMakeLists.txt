# tests/CMakeLists.txt


# This function automates some of the setup of testing an application that generates
# a single output file and differences that against a reference file
# See Rinextools RinEdit for examples of how to use it.
function(test_app_with_file test_name app_name labels args diff_args)
    add_test(NAME ${test_name}
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:${app_name}>
        -DTESTBASE=${test_name}
        -DTESTNAME=${test_name}
        -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}/outputs
        -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
        -DARGS=${args}
        -DOWNOUTPUT=1
        -DDIFF_PROG=${df_diff}
        -DDIFF_ARGS=${diff_args}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
    set_property(TEST ${test_name} PROPERTY LABELS ${labels})
endfunction()


# This function automates some of the setup of testing an application that generates
# output to standard output  and differences that against a reference file
# See Rinextools RinSum for examples of how to use it.
function(test_app_with_stdout test_name app_name labels args diff_args)
    add_test(NAME ${test_name}
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:${app_name}>
        -DTESTBASE=${test_name}
        -DTESTNAME=${test_name}
        -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}/outputs
        -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
        -DARGS=${args}
        -DDIFF_PROG=${df_diff}
        -DDIFF_ARGS=${diff_args}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
    set_property(TEST ${test_name} PROPERTY LABELS ${labels})
endfunction()

if (TEST_SWITCH)
    add_subdirectory( difftools )
    add_subdirectory( filetools )
    add_subdirectory( time )
    add_subdirectory( positioning )
    add_subdirectory( Rinextools )
    add_subdirectory( mergetools )
    add_subdirectory( checktools )
endif()
