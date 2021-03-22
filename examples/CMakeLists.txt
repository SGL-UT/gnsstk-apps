# examples/CMakeLists.txt

add_executable(example1 example1.cpp)
target_link_libraries(example1 ${SUPPORTLIBS})

add_executable(example2 example2.cpp)
target_link_libraries(example2 ${SUPPORTLIBS})

add_executable(example3 example3.cpp)
target_link_libraries(example3 ${SUPPORTLIBS})

add_executable(example4 example4.cpp)
target_link_libraries(example4 ${SUPPORTLIBS})

add_executable(example12 example12.cpp)
target_link_libraries(example12 ${SUPPORTLIBS})

add_executable(example15 example15.cpp)
target_link_libraries(example15 ${SUPPORTLIBS})

add_executable(navfilterex navfilterex.cpp)
target_link_libraries(navfilterex ${SUPPORTLIBS})

add_executable(CommandOption1 CommandOption1.cpp)
target_link_libraries(CommandOption1 ${SUPPORTLIBS})

add_executable(CommandOption2 CommandOption2.cpp)
target_link_libraries(CommandOption2 ${SUPPORTLIBS})

add_executable(CommandOption3 CommandOption3.cpp)
target_link_libraries(CommandOption3 ${SUPPORTLIBS})

add_executable(CommandOption4 CommandOption4.cpp)
target_link_libraries(CommandOption4 ${SUPPORTLIBS})

add_executable(CommandOption5 CommandOption5.cpp)
target_link_libraries(CommandOption5 ${SUPPORTLIBS})
