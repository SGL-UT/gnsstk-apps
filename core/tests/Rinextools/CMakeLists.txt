# core/tests/Rinextools

set(TD ${GNSSTK_APPS_TEST_OUTPUT_DIR})
set(SD ${GNSSTK_APPS_TEST_DATA_DIR})

###############################################################################
# TEST RinDump
###############################################################################

# check that -h option is valid
add_test(NAME RinDump_help
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinDump>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

# Test all satellites (default) with C1C data
set( ARGS1 ${SD}/test_input_rinex3_obs_RinexObsFile.15o\ C1C\ )
add_test(NAME RinDump_Default_C1C
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinDump>
         -DDIFF_PROG=${df_diff}
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinDump_sat_default_C1C
         -DTESTNAME=RinDump_sat_default_C1C
         -DARGS=${ARGS1}
         -DDIFF_ARGS=-l7\ -z2
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST RinDump_Default_C1C PROPERTY LABELS Rinex3)

# Test single satellite with C1C data
set( ARGS2 ${SD}/test_input_rinex3_obs_RinexObsFile.15o\ G05\ C1C\ )
add_test(NAME RinDump_Single_C1C
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinDump>
         -DDIFF_PROG=${df_diff}
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinDump_sat_single_C1C
         -DTESTNAME=RinDump_sat_single_C1C
         -DARGS=${ARGS2}
         -DDIFF_ARGS=-l7\ -z2
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST RinDump_Single_C1C PROPERTY LABELS Rinex3)

# Test Sat with C1C data with RINEX 2 file
set( ARGS2 ${SD}/arlm200a.15o\ G23\ C1C\ )
add_test(NAME RinDump_Rinex2Input
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinDump>
         -DDIFF_PROG=${df_diff}
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinDump_Rinex2Input
         -DTESTNAME=RinDump_Rinex2Input
         -DARGS=${ARGS2}
         -DDIFF_ARGS=-l7\ -z2
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_property(TEST RinDump_Rinex2Input PROPERTY LABELS Rinex2)

test_app_with_stdout(
    RinDump_211Mixed
    RinDump
    Rinex2
    ${SD}/inputs/igs/faa1170b00.16o\ GC1C\ RC1C\ EC1B\ SC5X
    "-l7 -z1")


###############################################################################
# TEST RinEdit
###############################################################################
add_test(NAME RinEdit_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinEdit>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

test_app_with_file(
    RinEdit_Header #test_name
    RinEdit # application name
    Rinex2  # Lables for reporting
    --IF\ ${SD}/arlm200a.15o\ --OF\ ${TD}/RinEdit_Header.out\ --HDr\ RIGTC\ --HDx\ 3979316.4389,1050312.2534,4857066.9036\ --HDn\ 11502M002\ --HDm\ GOPE\ --HDa\ RIGTC\ GO\ PECNY
    -l2 # diff_args
    )

# Test that the output of RinEdit_Header is a valid Rinex Obs file.
add_test(NAME RinEdit_ValidOutput
    COMMAND ${CMAKE_COMMAND}
    -DTEST_PROG=$<TARGET_FILE:rowcheck>
    -DARGS=${TD}/RinEdit_Header.out
    -DTESTBASE=RinEdit_ValidOutput
    -DTESTNAME=RinEdit_ValidOutput
    -DTARGETDIR=${TD}
    -DNODIFF=1
    -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_tests_properties(RinEdit_ValidOutput PROPERTIES DEPENDS RinEdit_Header)
set_property(TEST RinEdit_ValidOutput PROPERTY LABELS Rinex2)

# Convert 3->2 and keep as much data as possible
test_app_with_file(
    RinEdit_302to211Mixed # test name
    RinEdit # app name
    "Rinex2 Rinex3" # labels
    --ID\ ${SD}/inputs/igs\ --IF\ FAA100PYF_R_20161700100_15M_01S_MO\ --ver2\ --OD\ ${TD}\ --OF\ RinEdit_302to211Mixed.out
    -l2)

# Convert 3->2 and make a GPS only file, uses an input file to configure the editing
test_app_with_file(
    RinEdit_302to211GPS
    RinEdit
    "Rinex2 Rinex3"
    --ID\ ${SD}/inputs/igs\ --IF\ FAA100PYF_R_20161700100_15M_01S_MO\ --ver2\ --OD\ ${TD}\ --OF\ RinEdit_302to211GPS.out\ --file\ ${SD}/inputs/RinEdit_GPS_only
    -l2)

# Edit out the last part of a Rinex3 file
test_app_with_file(
    RinEdit_302split1
    RinEdit
    "Rinex3"
    --IF\ ${SD}/inputs/igs/FAA100PYF_R_20161700100_15M_01S_MO\ --OF\ ${TD}/RinEdit_302split1.out\ --TE\ 2016,6,18,1,0,2
    -l2)

# Edit out the first part of a Rinex3 file
test_app_with_file(
    RinEdit_302split2
    RinEdit
    "Rinex3"
    --IF\ ${SD}/inputs/igs/FAA100PYF_R_20161700100_15M_01S_MO\ --OF\ ${TD}/RinEdit_302split2.out\ --TB\ 2016,6,18,1,0,3
    -l2)

# Combine the previous two files together
test_app_with_file(
    RinEdit_302merge
    RinEdit
    "Rinex3"
    --ID\ ${TD}\ --IF\ RinEdit_302split1.out\ --IF\ RinEdit_302split2.out\ --OF\ ${TD}/RinEdit_302merge.out
    -l2)
set_tests_properties(RinEdit_302merge PROPERTIES DEPENDS "RinEdit_302split1;RinEdit_302split2")

# Test thinning
test_app_with_file(
  RinEdit_210thin
  RinEdit
  "Rinex2"
  --ID\ ${SD}\ --IF\ test_dfix_txau047.ed.12o\ --OF\ ${TD}/RinEdit_210thin.out\ --TT\ 0.005\ --TN\ 30.0
  "-l2 -v")

# test merge rinex 2 obs
add_test(NAME RinEdit_211_merge_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:RinEdit>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinEdit_211_merge_1
         -DTESTNAME=RinEdit_211_merge_1
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rowdiff>
         -DINFILE1=arlm200a.15o
         -DINFILE2=arlm200b.15o
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditmerge.cmake)

# Merge the same file twice. Should get the same file back.
# This test currently fails, suggesting that RinEdit does not remove duplicate data.
# add_test(NAME RinEdit_211_merge_2
#          COMMAND ${CMAKE_COMMAND}
#          -DTEST_PROG=$<TARGET_FILE:RinEdit>
#          -DSOURCEDIR=${SD}
#          -DTARGETDIR=${TD}
#          -DTESTBASE=RinEdit_211_merge_2
#          -DTESTNAME=RinEdit_211_merge_2
#          -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
#          -DRINDIFF=$<TARGET_FILE:rowdiff>
#          -DINFILE1=RinEdit_211_merge_2.exp
#          -DINFILE2=RinEdit_211_merge_2.exp
#          -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditmerge.cmake)

###############################################################################
# TEST RinEditNav
###############################################################################
add_test(NAME RinEditNav_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)


# merge 2x RINEX 2 to a single RINEX 3
add_test(NAME RinEditNav_merge_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinEditNav_merge_1
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=arlm200a.15n
         -DINFILE2=arlm200b.15n
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditnavmerge3.cmake)

# merge 2x RINEX 2 to a single RINEX 2
add_test(NAME RinEditNav_merge_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinEditNav_merge_2
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=arlm200a.15n
         -DINFILE2=arlm200b.15n
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditnavmerge2.cmake)

# Merge the same file twice. Should get the same file back.
add_test(NAME RinEditNav_merge_3
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE=RinEditNav_merge_3
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=RinEditNav_merge_3.exp
         -DINFILE2=RinEditNav_merge_3.exp
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditnavmerge2.cmake)

# merge 2x RINEX 2 to a RINEX 2 and a RINEX 3
add_test(NAME RinEditNav_merge_4
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DTESTBASE2=RinEditNav_merge_2
         -DTESTBASE3=RinEditNav_merge_1
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=arlm200a.15n
         -DINFILE2=arlm200b.15n
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrineditnavmerge23.cmake)

# Test multiple outputs with system specified
add_test(NAME RinEditNav_outputs_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_outputs_1.cmake)

# Test system exclusion with system specified
add_test(NAME RinEditNav_exclude_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_exclude_1.cmake)

# Test system exclusion with satellite specified
add_test(NAME RinEditNav_exclude_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_exclude_2.cmake)

# Test filtering via time (start time only)
add_test(NAME RinEditNav_time_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_time_1.cmake)

# Test filtering via time (end time only)
add_test(NAME RinEditNav_time_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_time_2.cmake)

# Test filtering via time (start and end time)
add_test(NAME RinEditNav_time_3
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rineditnav>
         -DSOURCEDIR=${SD}
         -DTARGETDIR=${TD}
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -P ${CMAKE_CURRENT_SOURCE_DIR}/RinEditNav_time_3.cmake)

###############################################################################
# TEST RinSum
# Run lots of different source files through RinSum to make sure the readers
# can at least handle the various variations 
###############################################################################

# Check RinSum with obspath and Rinex v2.11 file
test_app_with_stdout(RinSum_obspath_v211 RinSum Rinex2
    --obspath\ ${SD}/inputs/igs\ --obs\ faa1170b00.16o
    "-l2 -v")

# Check RinSum & Rinex v2.10 file
test_app_with_stdout(RinSum_v210_osn3 RinSum Rinex2
    --obs\ ${SD}/inputs/igs/osn31700.16o\ --ycode
    "-l2 -v")

# Check RinSum & Rinex v2.11 file
test_app_with_stdout(RinSum_v211_cags RinSum Rinex2
    --obs\ ${SD}/inputs/igs/cags1700.16o
    "-l2 -v")

# Check RinSum & Rinex v2.11 file
test_app_with_stdout(RinSum_v211_kerg RinSum Rinex2
    --obs\ ${SD}/inputs/igs/kerg1700.16o
    "-l2 -v")

# Check RinSum with Rinex v2.11 file with more obs types
test_app_with_stdout(RinSum_v211_nklg RinSum Rinex2
    --obs\ ${SD}/inputs/igs/nklg170b00.16o
    "-l2 -v")

# Check RinSum Rinex v3.02 input
test_app_with_stdout(RinSum_v302_FAA1 RinSum Rinex3
    --obs\ ${SD}/inputs/igs/FAA100PYF_R_20161700100_15M_01S_MO
    "-l2 -v")

# Check RinSum with Rinex v3.03 input
# Uncomment this when Nathanial's changes make it in
#test_app_with_stdout(RinSum_v302_FAA1 RinSum Rinex3
#    --obs\ ${SD}/inputs/igs/UCAL00CAN_S_20161700100_15M_01S_MO
#    "-l2 -v")


###############################################################################
# TEST scanBrdcFile
###############################################################################
set(test_name scanBrdcFile1)
add_test(NAME ${test_name}
    COMMAND ${CMAKE_COMMAND}
    -DTESTBASE=${test_name}
    -DTESTNAME=${test_name}
    -DTEST_PROG=$<TARGET_FILE:scanBrdcFile>
    -DDIFF_PROG=${df_diff}
    -DSOURCEDIR=${GNSSTK_APPS_TEST_DATA_DIR}
    -DTARGETDIR=${GNSSTK_APPS_TEST_OUTPUT_DIR}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/testScanBrdcFile.cmake)
set_property(TEST  ${test_name} PROPERTY LABELS Rinex2)
