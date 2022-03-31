# apps/geomatics/cycleslips/CMakeLists.txt

add_executable(DiscFix DiscFix.cpp)
linkum(DiscFix)
install (TARGETS DiscFix DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(dfix dfix.cpp)
linkum(dfix)
install (TARGETS dfix DESTINATION "${CMAKE_INSTALL_BINDIR}")

