# apps/difftools/CMakeLists.txt

add_executable(rowdiff rowdiff.cpp)
linkum(rowdiff)
install (TARGETS rowdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rnwdiff rnwdiff.cpp)
linkum(rnwdiff)
install (TARGETS rnwdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rmwdiff rmwdiff.cpp)
linkum(rmwdiff)
install (TARGETS rmwdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rinheaddiff rinheaddiff.cpp)
linkum(rinheaddiff)
install (TARGETS rinheaddiff DESTINATION "${CMAKE_INSTALL_BINDIR}")
