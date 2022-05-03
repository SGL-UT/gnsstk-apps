set( SD ${GNSSTK_APPS_TEST_DATA_DIR} )
set( TD ${GNSSTK_APPS_TEST_OUTPUT_DIR} )
set( ED ${GNSSTK_TEST_DATA_DIR} )
set( df_diff ${GNSSTK_BINDIR}/df_diff )

###############################################################################
# TEST findMoreThan12 - SLOW
###############################################################################

# Check that --help option is valid.
add_test(NAME findMoreThan12_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:findMoreThan12>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=findMoreThan12_CmdOpt_1
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

# Start time is based on a slight difference in 1.6 findMoreThan12
# default start time when compared with 2.7.  The time difference is
# immaterial for testing findMoreThan12.

# Check for consistency in results at 0 degrees elevation
add_test(NAME findMoreThan12_0
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:findMoreThan12>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=findMoreThan12_0
         -DTESTNAME=findMoreThan12_0
         -DARGS=-e\ ${SD}/glob200a.15n\ -m\ 0
         -DSPARG1=--position=-740289.9180\ -5457071.7340\ 3207245.5420
         -DSPARG2=--time=2015\ 199\ 86368
         -DSPARG3=--end-time=2015\ 200\ 79200
         -DDIFF_PROG=${df_diff}
         -DDIFF_ARGS=-e0.0001
         -DDIFF_UGLY=yes
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

# Check for consistency in results at 3 degrees elevation
add_test(NAME findMoreThan12_3
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:findMoreThan12>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=findMoreThan12_3
         -DTESTNAME=findMoreThan12_3
         -DARGS=-e\ ${SD}/glob200a.15n\ -m\ 3
         -DSPARG1=--position=-740289.9180\ -5457071.7340\ 3207245.5420
         -DSPARG2=--time=2015\ 199\ 86368
         -DSPARG3=--end-time=2015\ 200\ 79200
         -DDIFF_PROG=${df_diff}
         -DDIFF_ARGS=-e0.0001
         -DDIFF_UGLY=yes
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

# Check for consistency in results at 0 degrees elevation (SP3 Input)
add_test(NAME findMoreThan12_SP3
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:findMoreThan12>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=findMoreThan12_SP3
         -DTESTNAME=findMoreThan12_SP3
         -DARGS=-e\ ${SD}/anavsp315.200a\ -m\ 0
         -DSPARG1=--position=-740289.9180\ -5457071.7340\ 3207245.5420
         -DSPARG2=--time=2015\ 199\ 86368
         -DSPARG3=--end-time=2015\ 200\ 79200
         -DNODIFF=1
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

# Test with a non-existent ephemeris file
add_test(NAME findMoreThan12_MissingInput
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:findMoreThan12>
         -DARGS=-e\ ${SD}/notAFile.txt\ -m\ 0
         -DSPARG1=--position=-740289.9180\ -5457071.7340\ 3207245.5420
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

# Test with an invalid ephemeris file


###############################################################################
# TEST - wheresat Unit Test
###############################################################################

set (WSEPH "${SD}/glob199a.15n")
if (WIN32)
   # @todo use cmake_path once we've dropped support for everything
   # below cmake 3.20
   string(REPLACE "/" "\\" WSEPH "${WSEPH}")
endif()

set(tn wheresat_1)
add_test(NAME ${tn}
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=$<TARGET_FILE:wheresat>
    -DSOURCEDIR=${SD}
    -DTARGETDIR=${TD}
    -DTESTBASE=${tn}
    -DTESTNAME=${tn}
    -DSPARG1=--eph-files=${WSEPH}
    -DSPARG2=--start=07/18/2015\ 00:15:00
    -DSPARG3=--end=07/18/2015\ 01:00:00
    -DDIFF_PROG=${df_diff}
    -DEXTPATH=${EXTPATH}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

set(tn wheresat_2)
add_test(NAME ${tn}
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=$<TARGET_FILE:wheresat>
    -DSOURCEDIR=${SD}
    -DTARGETDIR=${TD}
    -DTESTBASE=${tn}
    -DTESTNAME=${tn}
    -DSPARG1=--eph-files=${WSEPH}
    -DSPARG2=--start=07/18/2015\ 00:15:00
    -DSPARG3=--end=07/18/2015\ 01:00:00
    -DARGS=-i
    -DDIFF_PROG=${df_diff}
    -DEXTPATH=${EXTPATH}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

set(tn wheresat_3)
add_test(NAME ${tn}
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=$<TARGET_FILE:wheresat>
    -DSOURCEDIR=${SD}
    -DTARGETDIR=${TD}
    -DTESTBASE=${tn}
    -DTESTNAME=${tn}
    -DSPARG1=--eph-files=${WSEPH}
    -DSPARG2=--start=07/18/2015\ 00:15:00
    -DSPARG3=--end=07/18/2015\ 01:00:00
    -DSPARG4=-u\ -740306.272\ -5457068.08\ 3207248.341
    -DARGS=-i
    -DDIFF_PROG=${df_diff}
    -DEXTPATH=${EXTPATH}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

set(tn wheresat_4)
add_test(NAME ${tn}
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=$<TARGET_FILE:wheresat>
    -DSOURCEDIR=${SD}
    -DTARGETDIR=${TD}
    -DTESTBASE=${tn}
    -DTESTNAME=${tn}
    -DSPARG1=--eph-files=${WSEPH}
    -DSPARG2=--start=07/18/2015\ 00:15:00
    -DSPARG3=--end=07/18/2015\ 01:00:00
    -DSPARG4=-u\ -740306.272\ -5457068.08\ 3207248.341
    -DARGS=-iV
    -DDIFF_PROG=${df_diff}
    -DEXTPATH=${EXTPATH}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
