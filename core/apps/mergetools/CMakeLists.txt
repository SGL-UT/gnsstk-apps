# apps/mergetools/CMakeLists.txt


add_executable(mergeRinObs mergeRinObs.cpp)
target_link_libraries(mergeRinObs ${SUPPORTLIBS})
install (TARGETS mergeRinObs DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(mergeRinNav mergeRinNav.cpp)
target_link_libraries(mergeRinNav ${SUPPORTLIBS})
install (TARGETS mergeRinNav DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(mergeRinMet mergeRinMet.cpp)
target_link_libraries(mergeRinMet ${SUPPORTLIBS})
install (TARGETS mergeRinMet DESTINATION "${CMAKE_INSTALL_BINDIR}")

if (CMAKE_SYSTEM_NAME MATCHES "SunOS")
  target_link_libraries(mergeRinObs gen)
  target_link_libraries(mergeRinMet gen)
  target_link_libraries(mergeRinNav gen)
endif()

