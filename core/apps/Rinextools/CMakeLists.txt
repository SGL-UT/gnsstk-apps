# apps/Rinextools/CMakeLists.txt

add_executable(RinDump RinDump.cpp)
target_link_libraries(RinDump ${SUPPORTLIBS})
install (TARGETS RinDump DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(RinEdit RinEdit.cpp)
target_link_libraries(RinEdit ${SUPPORTLIBS})
install (TARGETS RinEdit DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(RinSum RinSum.cpp)
target_link_libraries(RinSum ${SUPPORTLIBS})
install (TARGETS RinSum DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(scanBrdcFile scanBrdcFile.cpp)
target_link_libraries(scanBrdcFile ${SUPPORTLIBS})
install (TARGETS scanBrdcFile DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(youwillfail youwillfail.cpp)
target_link_libraries(youwillfail ${SUPPORTLIBS})
install (TARGETS youwillfail DESTINATION "${CMAKE_INSTALL_BINDIR}")
