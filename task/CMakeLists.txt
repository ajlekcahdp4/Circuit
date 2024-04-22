add_executable(currents currents.cpp)

target_link_libraries(currents PRIVATE ${PROJECT_NAME} matrix)

