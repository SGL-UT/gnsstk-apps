# variables:
# ARGS         extra arguments for rinexelvstrip
# TEST_PROG    path to rinexelvstrip binary
# INPUT_FILE   RINEX OBS file to process
# NAV_FILE     FIC (or RINEX NAV) file matching time of INPUT_FILE
# GNSSTK_APPS_BINDIR location of GNSSTK binaries
# TESTBASE     name of the test, also used for expected data and output file
# SOURCEDIR    path where INPUT_FILE and NAV_FILE are located
# EXPDIR       path where the reference data file is located
# TARGETDIR    path where the stripped RINEX OBS output file will be written
# RINDIFF      path to rowdiff application

# Make sure windows knows where to find the DLLs
if ( WIN32 )
  set(ENV{PATH} "$ENV{PATH};${EXTPATH}")
  message("PATH is $ENV{PATH}")
endif ( WIN32 )

# Convert ARGS into a cmake list
IF(DEFINED ARGS)
   string(REPLACE " " ";" ARG_LIST ${ARGS})
ENDIF(DEFINED ARGS)

message(STATUS "running ${TEST_PROG} -i ${INPUT_FILE} -o ${TARGETDIR}/${TESTBASE}.out -n ${NAV_FILE} ${ARGS}")

execute_process(COMMAND ${TEST_PROG} -i ${INPUT_FILE} -o ${TARGETDIR}/${TESTBASE}.out -n ${NAV_FILE} ${ARG_LIST}
                OUTPUT_QUIET
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR)
    message(FATAL_ERROR "Test failed, exit code: ${HAD_ERROR}")
endif()

# diff against reference

message(STATUS "running ${RINDIFF} ${EXPDIR}/${TESTBASE}.exp ${TARGETDIR}/${TESTBASE}.out")

execute_process(COMMAND ${RINDIFF} ${EXPDIR}/${TESTBASE}.exp ${TARGETDIR}/${TESTBASE}.out
    OUTPUT_QUIET
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ: ${DIFFERENT}")
endif()

