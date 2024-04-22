cmake_minimum_required(VERSION 3.18)

project(Circuit LANGUAGES CXX)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
enable_testing()

find_package(matrix QUIET)

if(NOT matrix_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        matrix
        GIT_REPOSITORY https://github.com/LegendaryHog/matrix
        GIT_TAG        master
        OVERRIDE_FIND_PACKAGE
    )
endif()

find_package(matrix REQUIRED)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

aux_source_directory(src/ PROJECT_SOURCE_DIR)
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC include/)

add_subdirectory(unit_tests)
add_subdirectory(task)

#add_custom_target(
#    unit_tests 
#    DEPENDS
#        vector_test
#        matrix_test
#        circuit_test
#)

#add_custom_target(
#    whole_project
#    DEPENDS
#        unit_tests
#        currents
#)
