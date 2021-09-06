# apps/geomatics/cycleslips/CMakeLists.txt

add_executable(DiscFix DiscFix.cpp)
target_link_libraries(DiscFix ${SUPPORTLIBS})
install (TARGETS DiscFix DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(dfix dfix.cpp)
target_link_libraries(dfix ${SUPPORTLIBS})
install (TARGETS dfix DESTINATION "${CMAKE_INSTALL_BINDIR}")

