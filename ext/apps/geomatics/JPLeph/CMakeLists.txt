# apps/geomatics/JPLeph/CMakeLists.txt

add_executable(convertSSEph convertSSEph.cpp)
linkum(convertSSEph)
install (TARGETS convertSSEph DESTINATION "${CMAKE_INSTALL_BINDIR}")
