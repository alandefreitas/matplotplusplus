cmake_minimum_required(VERSION 3.14)
project(matplotplusplus)

# https://cliutils.gitlab.io/modern-cmake/chapters/basics/variables.html
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(cmake/CPM.cmake)
message("CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_FOR_DOCUMENTATION_IMAGES "Bypass wait() commands and save figures as .svg at destruction" OFF)
option(BUILD_HIGH_RESOLUTION_WORLD_MAP "Compile the high resolution maps for geoplots" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(WITH_SYSTEM_CIMG "Use system-provided CImg.h instead of bundled" OFF)
option(WITH_SYSTEM_NODESOUP "Use system-provided nodesoup instead of bundled" OFF)

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
