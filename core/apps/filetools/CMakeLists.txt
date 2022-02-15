# apps/filetools/CMakeLists.txt

add_executable(bc2sp3 bc2sp3.cpp)
linkum(bc2sp3)
install (TARGETS bc2sp3 DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(smdscheck smdscheck.cpp)
linkum(smdscheck)
install (TARGETS smdscheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(navdump navdump.cpp)
linkum(navdump)
install (TARGETS navdump DESTINATION "${CMAKE_INSTALL_BINDIR}")
