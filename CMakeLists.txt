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
    message(STATUS "Found Matrix - done")
else()
    message(FATAL_ERROR "Didn't find Matrix")
endif()

set(END_TO_END_TESTING_ENABLED True)
set(END_TO_END_PATH ${CMAKE_CURRENT_SOURCE_DIR}/end_to_end/)
if(${END_TO_END_TESTING_ENABLED} AND NOT EXISTS "${END_TO_END_PATH}/test_runner")
    execute_process(COMMAND git -C ${END_TO_END_PATH} clone https://github.com/LegendaryHog/test_runner.git --quiet RESULT_VARIABLE RES)
    if(${RES})
        message(WARNING "Cannot clone end to end test runner lib")
    endif()
    execute_process(COMMAND rm -rf ${END_TO_END_PATH}/test_runner/.git OUTPUT_QUIET)
    message(STATUS "End to end test runner lib was cloned - done")
endif()
if(${END_TO_END_TESTING_ENABLED})
    if(EXISTS "${END_TO_END_PATH}/test_runner")
        message(STATUS "Found test_runner - done")
    else()
        set(STR1 "Python lib to end to end testing not found.")
        set(STR2 "If you don't need end to end tests set END_TO_END_TESTING_ENABLED in False")
        message(WARNING "${STR1}\n${STR2}")
    endif()
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
