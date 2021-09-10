set( SD ${NAVSAT_TOOLS_TEST_DATA_DIR} )
set( TD ${NAVSAT_TOOLS_TEST_OUTPUT_DIR} )
###############################################################################
# TEST mergeRinMet
###############################################################################

# check that -h option is valid
add_test(NAME mergeRinMet_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinMet>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

add_test(NAME mergeRinMet_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinMet>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -DTESTBASE=mergeRinMet_1
         -DTESTNAME=mergeRinMet_1
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rmwdiff>
         -DINFILE1=arlm200a.15m
         -DINFILE2=arlm200b.15m
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrinmerge.cmake)

# Test that the output of mergeRinMet_1 is a valid Rinex Met file.
add_test(NAME mergeRinMet_ValidOutput
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rmwcheck>
         -DARGS=${TD}/mergeRinMet_1.out
         -DTESTBASE=mergeRinMet_ValidOutput
         -DTESTNAME=mergeRinMet_ValidOutput
         -DTARGETDIR=${TD}
         -DNODIFF=1
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_tests_properties(mergeRinMet_ValidOutput PROPERTIES DEPENDS mergeRinMet_1)

# Merge the same file twice. Should get the same file back.
add_test(NAME mergeRinMet_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinMet>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -DTESTBASE=mergeRinMet_2
         -DTESTNAME=mergeRinMet_2
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rmwdiff>
         -DINFILE1=mergeRinMet_2.exp
         -DINFILE2=mergeRinMet_2.exp
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrinmerge.cmake)

#add_test(NAME mergeRinMet_InvalidInput
#         COMMAND ${CMAKE_COMMAND}
#         -DTEST_PROG=$<TARGET_FILE:mergeRinMet>
#         -DARGS=-o\ ${TD}/mergeRinMet_InvalidInput.out\ ${SD}/timeconvert_2015_200_0.exp\ ${SD}/arlm200b.15m
#         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

#add_test(NAME mergeRinMet_MissingInput
#         COMMAND ${CMAKE_COMMAND}
#         -DTEST_PROG=$<TARGET_FILE:mergeRinMet>
#         -DARGS=-o\ ${TD}/mergeRinMet_MissingInput.out\ ${SD}/notAFile.txt\ ${SD}/arlm200b.15m
#         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)
###############################################################################
# TEST mergeRinNav
###############################################################################

# check that -h option is valid
add_test(NAME mergeRinNav_CmdOpt_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinNav>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testhelp.cmake)

add_test(NAME mergeRinNav_1
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinNav>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -DTESTBASE=mergeRinNav_1
         -DTESTNAME=mergeRinNav_1
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=arlm200a.15n
         -DINFILE2=arlm200b.15n
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrinmerge.cmake)

# Test that the output of mergeRinNav_1 is a valid Rinex Nav file.
add_test(NAME mergeRinNav_ValidOutput
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:rnwcheck>
         -DARGS=${TD}/mergeRinNav_1.out
         -DTESTBASE=mergeRinNav_ValidOutput
         -DTESTNAME=mergeRinNav_ValidOutput
         -DTARGETDIR=${TD}
         -DNODIFF=1
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
set_tests_properties(mergeRinNav_ValidOutput PROPERTIES DEPENDS mergeRinNav_1)


# Merge the same file twice. Should get the same file back.
add_test(NAME mergeRinNav_2
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:mergeRinNav>
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -DTESTBASE=mergeRinNav_2
         -DTESTNAME=mergeRinNav_2
         -DRINHEADDIFF=$<TARGET_FILE:rinheaddiff>
         -DRINDIFF=$<TARGET_FILE:rnwdiff>
         -DINFILE1=mergeRinNav_2.exp
         -DINFILE2=mergeRinNav_2.exp
         -P ${CMAKE_CURRENT_SOURCE_DIR}/testrinmerge.cmake)

#add_test(NAME mergeRinNav_InvalidInput
#         COMMAND ${CMAKE_COMMAND}
#         -DTEST_PROG=$<TARGET_FILE:mergeRinNav>
#         -DARGS=-o\ ${TD}/mergeRinNav_InvalidInput.out\ ${SD}/timeconvert_2015_200_0.exp\ ${SD}/arlm200b.15n
#         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)

#add_test(NAME mergeRinNav_MissingInput
#         COMMAND ${CMAKE_COMMAND}
#         -DTEST_PROG=$<TARGET_FILE:mergeRinNav>
#         -DARGS=-o\ ${TD}/mergeRinNav_MissingInput.out\ ${SD}/notAFile.txt\ ${SD}/arlm200b.15n
#         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testfailexp.cmake)
