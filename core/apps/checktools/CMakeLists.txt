# apps/checktools/CMakeLists.txt

add_executable(rowcheck rowcheck.cpp)
target_link_libraries(rowcheck ${SUPPORTLIBS})
install (TARGETS rowcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rmwcheck rmwcheck.cpp)
target_link_libraries(rmwcheck ${SUPPORTLIBS})
install (TARGETS rmwcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rnwcheck rnwcheck.cpp)
target_link_libraries(rnwcheck ${SUPPORTLIBS})
install (TARGETS rnwcheck DESTINATION "${CMAKE_INSTALL_BINDIR}")


