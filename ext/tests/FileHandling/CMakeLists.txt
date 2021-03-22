
add_executable(Yuma_T Yuma_T.cpp)
target_link_libraries(Yuma_T ${SUPPORTLIBS})
add_test(NAME YumaData
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:Yuma_T>
         -DDIFF_PROG=${df_diff}
         -DTESTBASE=test_output_Yuma_T
         -DTESTNAME=test_output_Yuma_T
         -DOWNOUTPUT=1
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)

add_executable(SEM_T SEM_T.cpp)
target_link_libraries(SEM_T ${SUPPORTLIBS})
add_test(NAME SEMData
         COMMAND ${CMAKE_COMMAND}
         -DTEST_PROG=$<TARGET_FILE:SEM_T>
         -DTESTBASE=test_output_SEM_T
         -DTESTNAME=test_output_SEM_T
         -DOWNOUTPUT=1
         -DSOURCEDIR=${NAVSAT_TOOLS_TEST_DATA_DIR}
         -DTARGETDIR=${NAVSAT_TOOLS_TEST_OUTPUT_DIR}
         -P ${CMAKE_CURRENT_SOURCE_DIR}/../testsuccexp.cmake)
