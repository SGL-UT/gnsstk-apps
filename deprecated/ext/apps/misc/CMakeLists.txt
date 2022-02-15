# apps/misc/CMakeLists.txt

add_executable(SVNumXRefDUMP  SVNumXRefDUMP.cpp)
linkum(SVNumXRefDUMP)
install (TARGETS SVNumXRefDUMP DESTINATION "${CMAKE_INSTALL_BINDIR}")
