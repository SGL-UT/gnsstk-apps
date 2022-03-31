# apps/checktools/CMakeLists.txt

add_executable(rowcheck rowcheck.cpp)
linkum(rowcheck)
install (TARGETS rowcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rmwcheck rmwcheck.cpp)
linkum(rmwcheck)
install (TARGETS rmwcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rnwcheck rnwcheck.cpp)
linkum(rnwcheck)
install (TARGETS rnwcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")


