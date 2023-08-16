aux_source_directory(. SRC_LIST)

add_executable(circuit_test ${SRC_LIST})

target_link_libraries(circuit_test PRIVATE ${GTEST_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(circuit_test PRIVATE ../include)

gtest_discover_tests(circuit_test)