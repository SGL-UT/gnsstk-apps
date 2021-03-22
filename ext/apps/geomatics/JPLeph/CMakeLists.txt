# apps/geomatics/JPLeph/CMakeLists.txt

add_executable(convertSSEph convertSSEph.cpp)
target_link_libraries(convertSSEph ${SUPPORTLIBS})
install (TARGETS convertSSEph DESTINATION "${CMAKE_INSTALL_BINDIR}")
