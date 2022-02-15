# apps/geomatics/robust/CMakeLists.txt

add_executable(lsfilt lsfilt.cpp)
linkum(lsfilt)
install (TARGETS lsfilt DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(rstats rstats.cpp)
linkum(rstats)
install (TARGETS rstats DESTINATION "${CMAKE_INSTALL_BINDIR}")

