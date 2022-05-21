# Generic test where failure is expected
# TEST_PROG: the program under test
# ARGS: a space-separated argument list (optional)
# SPARG1: a single escaped argument (optional)
# SPARG2: a single escaped argument (optional)
# SPARG3: a single escaped argument (optional)
# SPARG4: a single escaped argument (optional)

# Make sure windows knows where to find the DLLs
if ( WIN32 )
  set(ENV{PATH} "$ENV{PATH};${EXTPATH}")
endif ( WIN32 )

# Convert ARGS into a cmake list
IF(DEFINED ARGS)
   string(REPLACE " " ";" ARG_LIST ${ARGS})
ENDIF(DEFINED ARGS)

message(STATUS "running ${TEST_PROG} ${ARG_LIST} ${SPARG1} ${SPARG2} ${SPARG3} ${SPARG4}")
execute_process(COMMAND ${TEST_PROG} ${ARG_LIST} ${SPARG1} ${SPARG2} ${SPARG3} ${SPARG4}
                OUTPUT_QUIET
                ERROR_QUIET
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR EQUAL 0)
    message(FATAL_ERROR "Test failed")
endif()

if (HAD_ERROR STREQUAL "Segmentation fault")
    message(FATAL_ERROR "Test had a seg fault")
endif()
