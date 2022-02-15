# apps/filetools/CMakeLists.txt

add_executable(sp3version sp3version.cpp)
linkum(sp3version)
install (TARGETS sp3version DESTINATION "${CMAKE_INSTALL_BINDIR}")


