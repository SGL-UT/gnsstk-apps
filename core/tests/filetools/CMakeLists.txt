set( SD ${GNSSTK_APPS_TEST_DATA_DIR} )
set( TD ${GNSSTK_APPS_TEST_OUTPUT_DIR} )

###############################################################################
# TEST bc2sp3
###############################################################################

# check that -h option is valid
add_test(NAME bc2sp3_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:bc2sp3>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testhelp_bc.cmake)

# check that bc2sp3 fails with missing files
add_test(NAME bc2sp3_CmdOpt_2
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:bc2sp3>
        -DSOURCEDIR=${SD}
        -DTARGETDIR=${TD}
        -DEXTPATH=${EXTPATH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/testmissing.cmake)

# check that bc2sp3 fails with invalid input
add_test(NAME bc2sp3_InvalidInput
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:bc2sp3>
        -DARGS=--in\ ${SD}/timeconvert_2015_200_0.exp\ --out\ ${TD}/bc2sp3_InvalidInput.out
        -DEXTPATH=${EXTPATH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

# check the output against prepared "truth" - SP3c case
set(ARGS_1 "--outputC --tb 1825,518400 --te 1825,604500")
add_test(NAME bc2sp3_Same_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:bc2sp3>
         -DFILE1=nga002.15n
         -DFILE2=nga003.15n
         -DTESTBASE=bc2sp3_Same_1
         -DTESTNAME=bc2sp3_Same_1
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DARGS=${ARGS_1}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testsame.cmake)

# check the output against prepared "truth" - SP3a case
set(ARGS_2 "--tb 1825,518400 --te 1825,604500 --msg Test_Case_File" )
add_test(NAME bc2sp3_Same_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:bc2sp3>
         -DFILE1=nga002.15n
         -DFILE2=nga003.15n
         -DTESTBASE=bc2sp3_Same_2
         -DTESTNAME=bc2sp3_Same_2
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DARGS=${ARGS_2}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testsame.cmake)

# check the output against prepared "truth" - SP3a case, 900s cadence
set(ARGS_2 "--tb 1825,518400 --te 1825,603900 --cs 900" )
add_test(NAME bc2sp3_Same_3
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:bc2sp3>
         -DFILE1=nga002.15n
         -DFILE2=nga003.15n
         -DTESTBASE=bc2sp3_Same_3
         -DTESTNAME=bc2sp3_Same_3
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DARGS=${ARGS_2}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testsame.cmake)


# tests for smdscheck

add_test(NAME smdscheck_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:smdscheck>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

set(SMDSCHECK_VALID_ARGS "-M ${SD}/sats.csv")
add_test(NAME smdscheck_valid
  COMMAND ${CMAKE_COMMAND}
  -DTEST_PROG=$<TARGET_FILE:smdscheck>
  -DSOURCEDIR=${SD}
  -DTARGETDIR=${TD}
  -DTESTNAME=smdscheck_valid
  -DTESTBASE=smdscheck_valid
  -DARGS=${SMDSCHECK_VALID_ARGS}
  -DNODIFF=1
  -DEXTPATH=${EXTPATH}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

set(SMDSCHECK_INVALID_ARGS "-M ${SD}/TropModel_Zero.exp")
add_test(NAME smdscheck_invalid
  COMMAND ${CMAKE_COMMAND}
  -DTEST_PROG=$<TARGET_FILE:smdscheck>
  -DARGS=${SMDSCHECK_INVALID_ARGS}
  -DEXTPATH=${EXTPATH}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

set(SMDSCHECK_MISSING_ARGS "-M ${SD}/not_a_real_file.what")
add_test(NAME smdscheck_missing
  COMMAND ${CMAKE_COMMAND}
  -DTEST_PROG=$<TARGET_FILE:smdscheck>
  -DARGS=${SMDSCHECK_MISSING_ARGS}
  -DEXTPATH=${EXTPATH}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

###############################################################################
# TEST navdump
###############################################################################

# check that -h option is valid
add_test(NAME navdump_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:navdump>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

# check that navdump fails with no files specified
add_test(NAME navdump_CmdOpt_2
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:navdump>
        -DSOURCEDIR=${SD}
        -DTARGETDIR=${TD}
        -DEXTPATH=${EXTPATH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

# check that navdump fails with invalid input
add_test(NAME navdump_InvalidInput
        COMMAND ${CMAKE_COMMAND}
        -DTEST_PROG=$<TARGET_FILE:navdump>
        -DARGS=${SD}/timeconvert_2015_200_0.exp
        -DEXTPATH=${EXTPATH}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

# note that the reference output doesn't need to be kept strictly consistent
# check the output against prepared "truth" - SP3c case
set(NAVDUMP_REGRESSION_1_ARGS "${SD}/test_input_SP3a.sp3")
add_test(NAME navdump_Regression_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:navdump>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=navdump_Regression_1
         -DTESTNAME=navdump_Regression_1
         -DARGS=${NAVDUMP_REGRESSION_1_ARGS}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

# note that the reference output doesn't need to be kept strictly consistent
# check the output against prepared "truth" - RINEX case
set(NAVDUMP_REGRESSION_2_ARGS "${SD}/arlm2000.15n")
add_test(NAME navdump_Regression_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:navdump>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=navdump_Regression_2
         -DTESTNAME=navdump_Regression_2
         -DARGS=${NAVDUMP_REGRESSION_2_ARGS}
         -DEXTPATH=${EXTPATH}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
