# apps/filetools/CMakeLists.txt

add_executable(bc2sp3 bc2sp3.cpp)
target_link_libraries(bc2sp3 ${SUPPORTLIBS})
install (TARGETS bc2sp3 DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(smdscheck smdscheck.cpp)
target_link_libraries(smdscheck ${SUPPORTLIBS})
install (TARGETS smdscheck DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(navdump navdump.cpp)
target_link_libraries(navdump ${SUPPORTLIBS})
install (TARGETS navdump DESTINATION "${CMAKE_INSTALL_BINDIR}")
