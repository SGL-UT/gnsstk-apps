# apps/Rinextools/CMakeLists.txt

add_executable(RinNav RinNav.cpp)
target_link_libraries(RinNav ${SUPPORTLIBS})
install (TARGETS RinNav DESTINATION "${CMAKE_INSTALL_BINDIR}")

