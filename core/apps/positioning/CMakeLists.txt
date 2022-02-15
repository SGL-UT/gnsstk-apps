# apps/positioning/CMakeListt.txt

add_executable(poscvt poscvt.cpp)
linkum(poscvt)
install (TARGETS poscvt DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(PRSolve PRSolve.cpp)
linkum(PRSolve)
install (TARGETS PRSolve DESTINATION "${CMAKE_INSTALL_BINDIR}")
