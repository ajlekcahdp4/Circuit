aux_source_directory(. SRC_LIST_1)
aux_source_directory(../src SRC_LIST_2)

add_executable(circuit_test ${SRC_LIST_1} ${SRC_LIST_2})

target_link_libraries(circuit_test PRIVATE ${GTEST_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(circuit_test PRIVATE ../include)

gtest_discover_tests(circuit_test)