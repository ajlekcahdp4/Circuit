aux_source_directory(. SRC_LIST)
add_executable(circuit_test ${SRC_LIST})
target_link_libraries(circuit_test PRIVATE ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME})
gtest_discover_tests(circuit_test)
