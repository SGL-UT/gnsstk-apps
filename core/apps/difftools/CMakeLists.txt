# apps/difftools/CMakeLists.txt

add_executable(rowdiff rowdiff.cpp)
target_link_libraries(rowdiff ${SUPPORTLIBS})
install (TARGETS rowdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rnwdiff rnwdiff.cpp)
target_link_libraries(rnwdiff ${SUPPORTLIBS})
install (TARGETS rnwdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rmwdiff rmwdiff.cpp)
target_link_libraries(rmwdiff ${SUPPORTLIBS})
install (TARGETS rmwdiff DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rinheaddiff rinheaddiff.cpp)
target_link_libraries(rinheaddiff ${SUPPORTLIBS})
install (TARGETS rinheaddiff DESTINATION "${CMAKE_INSTALL_BINDIR}")
