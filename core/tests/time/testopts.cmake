# test that output is correct and unchanged

# Make sure windows knows where to find the DLLs
if ( WIN32 )
  set(ENV{PATH} "$ENV{PATH};${EXTPATH}")
endif ( WIN32 )

execute_process(COMMAND ${TEST_PROG} ${PROG_OPTS} ${PROG_OPTS_2}
                OUTPUT_FILE ${TARGETDIR}/${TESTNAME}.out
                RESULT_VARIABLE HAD_ERROR)
# files are expected to be the same
if(HAD_ERROR)
    message(FATAL_ERROR "Test failed: ${HAD_ERROR}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files
    ${TARGETDIR}/${TESTNAME}.out ${SOURCEDIR}/${TESTBASE}.exp
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ: ${DIFFERENT}")
endif()
