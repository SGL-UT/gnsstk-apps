# apps/mergetools/CMakeLists.txt


add_executable(mergeRinNav mergeRinNav.cpp)
linkum(mergeRinNav)
install (TARGETS mergeRinNav DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(mergeRinMet mergeRinMet.cpp)
linkum(mergeRinMet)
install (TARGETS mergeRinMet DESTINATION "${CMAKE_INSTALL_BINDIR}")

if (CMAKE_SYSTEM_NAME MATCHES "SunOS")
  target_link_libraries(mergeRinMet gen)
  target_link_libraries(mergeRinNav gen)
endif()

