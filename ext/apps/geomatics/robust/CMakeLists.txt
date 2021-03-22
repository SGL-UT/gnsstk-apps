# apps/geomatics/robust/CMakeLists.txt

add_executable(lsfilt lsfilt.cpp)
target_link_libraries(lsfilt ${SUPPORTLIBS})
install (TARGETS lsfilt DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rstats rstats.cpp)
target_link_libraries(rstats ${SUPPORTLIBS})
install (TARGETS rstats DESTINATION "${CMAKE_INSTALL_BINDIR}")

