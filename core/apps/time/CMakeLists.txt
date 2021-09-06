# apps/time/CMakeLists.txt

add_executable(timeconvert timeconvert.cpp)
target_link_libraries(timeconvert ${SUPPORTLIBS})
install (TARGETS timeconvert DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(timediff timediff.cpp)
target_link_libraries(timediff ${SUPPORTLIBS})
install (TARGETS timediff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(calgps calgps.cpp)
target_link_libraries(calgps ${SUPPORTLIBS})
install (TARGETS calgps DESTINATION "${CMAKE_INSTALL_BINDIR}")
