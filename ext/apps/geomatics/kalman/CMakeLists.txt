# apps/geomatics/kalman/CMakeLists.txt

add_executable(mergeSRI mergeSRI.cpp)
linkum(mergeSRI)
install (TARGETS mergeSRI DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(tkalm tkalm.cpp)
linkum(tkalm)
install (TARGETS tkalm DESTINATION "${CMAKE_INSTALL_BINDIR}")

