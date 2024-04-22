cmake_minimum_required(VERSION 3.18)

project(Circuit LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
enable_testing()

find_package(Matrix QUIET)

if(NOT ${Matrix_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        Matrix
        GIT_REPOSITORY https://github.com/LegendaryHog/matrix
        GIT_TAG        master
        OVERRIDE_FIND_PACKAGE
    )
endif()

find_package(Matrix)
if(${Matrix_FOUND})
    message("-- Found Matrix: True")
else()
    message(FATAL_ERROR "-- Found Matrix: False")
endif()

set(END_TO_END_TESTING_ENABLED True)
if(${END_TO_END_TESTING_ENABLED})
    execute_process("git -C end_to_end/ clone https://github.com/LegendaryHog/test_runner.git" OUTPUT_QUIET)
endif()

if(NOT EXISTS "end_to_end/test_runner" AND ${END_TO_END_TESTING_ENABLED})
    message(WARNING "Python lib to end to end testing not found.")
    message(WARNING "If you don't need end to end tests set END_TO_END_TESTING_ENABLED in False")
endif()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

aux_source_directory(src/ PROJECT_SOURCE_DIRECTORY)
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIRECTORY})
target_link_libraries(${PROJECT_NAME} PRIVATE Matrix)
target_include_directories(${PROJECT_NAME} PUBLIC include/)

add_subdirectory(unit_tests)
add_subdirectory(task)

add_custom_target(
    unit_tests 
    DEPENDS
        vector_test
        matrix_test
        circuit_test
)

add_custom_target(
    whole_project
    DEPENDS
        unit_tests
        currents
)
