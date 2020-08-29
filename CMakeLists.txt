cmake_minimum_required(VERSION 3.14)
project(matplotplusplus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/CPM.cmake)
message("CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_FOR_DOCUMENTATION_IMAGES "Bypass wait() commands and save figures as .svg at destruction" OFF)
option(BUILD_HIGH_RESOLUTION_WORLD_MAP "Compile the high resolution maps for geoplots" ON)

if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(MASTER_PROJECT ON)
else()
    set(MASTER_PROJECT OFF)
endif()
set(MATPLOT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(source)

if (MASTER_PROJECT)
    if (BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()
    if (BUILD_TESTS)
        add_subdirectory(test)
    endif()
endif()