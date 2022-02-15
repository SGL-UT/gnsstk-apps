# apps/Rinextools/CMakeLists.txt

add_executable(RinDump RinDump.cpp)
linkum(RinDump)
install (TARGETS RinDump DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(RinEdit RinEdit.cpp)
linkum(RinEdit)
install (TARGETS RinEdit DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rineditnav RinEditNav.cpp)
linkum(rineditnav)
install (TARGETS rineditnav DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(RinSum RinSum.cpp)
linkum(RinSum)
install (TARGETS RinSum DESTINATION "${CMAKE_INSTALL_BINDIR}")
