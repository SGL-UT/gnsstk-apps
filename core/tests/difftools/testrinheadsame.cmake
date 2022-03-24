# tests for rinheaddiff

message(STATUS "running ${TEST_PROG} ${IXOPTS} ${SOURCEDIR}/${FILE1} ${SOURCEDIR}/${FILE2}")

# Make sure windows knows where to find the DLLs
if ( WIN32 )
  set(ENV{PATH} "$ENV{PATH};${EXTPATH}")
endif ( WIN32 )

execute_process(COMMAND ${TEST_PROG} ${IXOPTS} ${SOURCEDIR}/${FILE1} ${SOURCEDIR}/${FILE2}
                OUTPUT_FILE ${TARGETDIR}/${TESTBASE}.out
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR)
    message(FATAL_ERROR "Test failed: ${HAD_ERROR}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files
    ${SOURCEDIR}/${TESTBASE}.exp ${TARGETDIR}/${TESTBASE}.out
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ: ${DIFFERENT}")
endif()
