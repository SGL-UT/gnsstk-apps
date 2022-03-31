# apps/rfw/CMakeLists.txt

add_executable(rfw rfw.cpp FDStreamBuff.cpp TCPStreamBuff.cpp)
linkum(rfw)
install (TARGETS rfw DESTINATION "${CMAKE_INSTALL_BINDIR}")

if (CMAKE_SYSTEM_NAME MATCHES "SunOS")
  target_link_libraries(rfw socket nsl)
endif()
