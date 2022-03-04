# Test for RINEX merge tools.
#
# Expected variables (required unless otherwise noted):
# TEST_PROG: the program under test
# SOURCEDIR: the location of the reference file
# TARGETDIR: the directory to store stdout to compare with the reference output
# TESTBASE2: the name of the test, used to create output file and find reference
# TESTBASE3: the name of the test, used to create output file and find reference
# INFILE1: first input file
# INFILE2: second input file
# RINDIFF: location of RINEX diff tool for the format being tested
# RINHEADDIFF: location of rinheaddiff application
#
# TEST_PROG is expected to generate the output file
# ${TARGETDIR}/${TESTBASE3}.out (RINEX 3)
# ${TARGETDIR}/${TESTBASE2}.out (RINEX 2)
#
# Reference files are
# ${SOURCEDIR}/${TESTBASE3}.exp (RINEX 3)
# ${SOURCEDIR}/${TESTBASE2}.exp (RINEX 2)

# Make sure windows knows where to find the DLLs
if ( WIN32 )
  set(ENV{PATH} "$ENV{PATH};${EXTPATH}")
endif ( WIN32 )

# Generate the merged file

message(STATUS "running ${TEST_PROG} -o ${TARGETDIR}/${TESTBASE3}.out -2 ${TARGETDIR}/${TESTBASE2}.out ${SOURCEDIR}/${INFILE1} ${SOURCEDIR}/${INFILE2}")

execute_process(COMMAND ${TEST_PROG} -o ${TARGETDIR}/${TESTBASE3}.out -2 ${TARGETDIR}/${TESTBASE2}.out ${SOURCEDIR}/${INFILE1} ${SOURCEDIR}/${INFILE2}
                OUTPUT_QUIET
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR)
    message(FATAL_ERROR "Test failed, exit code: ${HAD_ERROR}")
endif()


# diff against reference

message(STATUS "running ${RINDIFF} ${SOURCEDIR}/${TESTBASE3}.exp ${TARGETDIR}/${TESTBASE3}.out")

execute_process(COMMAND ${RINDIFF} ${SOURCEDIR}/${TESTBASE3}.exp ${TARGETDIR}/${TESTBASE3}.out
    OUTPUT_QUIET
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ: ${DIFFERENT}")
endif()

message(STATUS "running ${RINDIFF} ${SOURCEDIR}/${TESTBASE2}.exp ${TARGETDIR}/${TESTBASE2}.out")

execute_process(COMMAND ${RINDIFF} ${SOURCEDIR}/${TESTBASE2}.exp ${TARGETDIR}/${TESTBASE2}.out
    OUTPUT_QUIET
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ: ${DIFFERENT}")
endif()


# Check for header differences

set( EXCL1 "PGM / RUN BY / DATE" )

message(STATUS "running ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/${TESTBASE3}.exp ${TARGETDIR}/${TESTBASE3}.out")

execute_process(COMMAND ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/${TESTBASE3}.exp ${TARGETDIR}/${TESTBASE3}.out
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - headers differ")
endif()

message(STATUS "running ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/${TESTBASE2}.exp ${TARGETDIR}/${TESTBASE2}.out")

execute_process(COMMAND ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/${TESTBASE2}.exp ${TARGETDIR}/${TESTBASE2}.out
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - headers differ")
endif()
