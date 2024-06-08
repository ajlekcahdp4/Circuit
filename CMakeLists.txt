cmake_minimum_required(VERSION 3.18)

project(Circuit LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
enable_testing()

set(END_TO_END_TESTING True CACHE STRING "")

if(${END_TO_END_TESTING} AND NOT EXISTS "end_to_end/test_runner")
    execute_process(COMMAND git -C end_to_end/ clone https:/github.com/LegendaryHog/test_runner.git --quiet RESULT_VARIABLE RES)
    if(${RES})
        message(WARNING "Cannot install end to end test runner lib")
    endif()
    execute_process(COMMAND rm -rf end_to_end/test_runner/.git OUTPUT_QUIET)
    message(STATUS "End to end test runner lib was installed - done")
endif()
if(${END_TO_END_TESTING})
    if(EXISTS "end_to_end/test_runner")
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

set (CIRCUIT_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
set (CIRCUIT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(lib/src/ LIB_SRC_LIST)
add_library(${PROJECT_NAME} ${LIB_SRC_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC ${CIRCUIT_LIB_INCLUDE_DIR})

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
