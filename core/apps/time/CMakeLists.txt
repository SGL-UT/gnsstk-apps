# apps/time/CMakeLists.txt

add_executable(timeconvert timeconvert.cpp)
linkum(timeconvert)
install (TARGETS timeconvert DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(timediff timediff.cpp)
linkum(timediff)
install (TARGETS timediff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(calgps calgps.cpp)
linkum(calgps)
install (TARGETS calgps DESTINATION "${CMAKE_INSTALL_BINDIR}")
