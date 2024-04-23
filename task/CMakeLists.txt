aux_source_directory(./ SRC_LIST)
add_executable(currents ${SRC_LIST})
target_link_libraries(currents PRIVATE ${PROJECT_NAME} Matrix)
target_include_directories(currents PRIVATE ../include/)

