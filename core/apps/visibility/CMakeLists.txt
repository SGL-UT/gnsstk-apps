add_executable(wheresat WhereSat.cpp)
linkum(wheresat)
install (TARGETS wheresat DESTINATION "${CMAKE_INSTALL_BINDIR}")

add_executable(findMoreThan12 findMoreThan12.cpp)
linkum(findMoreThan12)
install (TARGETS findMoreThan12 DESTINATION "${CMAKE_INSTALL_BINDIR}")
