# Test RinEditNav's ability to use multiple outputs with systems specified
#
# Expected variables (required unless otherwise noted):
# TEST_PROG: the program under test
# SOURCEDIR: the location of the reference file
# TARGETDIR: the directory to store stdout to compare with the reference output
# RINDIFF: location of RINEX diff tool for the format being tested
# RINHEADDIFF: location of rinheaddiff application

message(STATUS "running ${TEST_PROG} -o gps:${TARGETDIR}/RinEditNav_outputs_1.gps.out -o gal:${TARGETDIR}/RinEditNav_outputs_1.gal.out ${SOURCEDIR}/test_input_rinex3_nav_gal.20n")

execute_process(COMMAND ${TEST_PROG} -o gps:${TARGETDIR}/RinEditNav_outputs_1.gps.out -o gal:${TARGETDIR}/RinEditNav_outputs_1.gal.out ${SOURCEDIR}/test_input_rinex3_nav_gal.20n
                OUTPUT_QUIET
                RESULT_VARIABLE HAD_ERROR)
if(HAD_ERROR)
    message(FATAL_ERROR "Test failed, exit code: ${HAD_ERROR}")
endif()


# diff against reference

message(STATUS "running ${RINDIFF} ${SOURCEDIR}/RinEditNav_outputs_1.gps.exp ${TARGETDIR}/RinEditNav_outputs_1.gps.out")

execute_process(COMMAND ${RINDIFF} ${SOURCEDIR}/RinEditNav_outputs_1.gps.exp ${TARGETDIR}/RinEditNav_outputs_1.gps.out
    OUTPUT_QUIET
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ")
endif()

message(STATUS "running ${RINDIFF} ${SOURCEDIR}/RinEditNav_outputs_1.gal.exp ${TARGETDIR}/RinEditNav_outputs_1.gal.out")

execute_process(COMMAND ${RINDIFF} ${SOURCEDIR}/RinEditNav_outputs_1.gal.exp ${TARGETDIR}/RinEditNav_outputs_1.gal.out
    OUTPUT_QUIET
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - files differ")
endif()


# Check for header differences

set( EXCL1 "PGM / RUN BY / DATE" )

message(STATUS "running ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/RinEditNav_outputs_1.gps.exp ${TARGETDIR}/RinEditNav_outputs_1.gps.out")

execute_process(COMMAND ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/RinEditNav_outputs_1.gps.exp ${TARGETDIR}/RinEditNav_outputs_1.gps.out
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - headers differ")
endif()

message(STATUS "running ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/RinEditNav_outputs_1.gal.exp ${TARGETDIR}/RinEditNav_outputs_1.gal.out")

execute_process(COMMAND ${RINHEADDIFF} -x ${EXCL1} ${SOURCEDIR}/RinEditNav_outputs_1.gal.exp ${TARGETDIR}/RinEditNav_outputs_1.gal.out
    RESULT_VARIABLE DIFFERENT)
if(DIFFERENT)
    message(FATAL_ERROR "Test failed - headers differ")
endif()
