#######################################################
### Matplot++                                       ###
#######################################################
# Project information
cmake_minimum_required(VERSION 3.15)
include(cmake/functions/ensure_build_type.cmake)
project(Matplot++ VERSION 1.2.0)
set(MATPLOT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MATPLOT_VERSION ${CMAKE_PROJECT_VERSION})

#######################################################
### CMake Functions                                 ###
#######################################################
# CMake dependencies for installer
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# Append ./cmake directory to our include paths for the find_package scripts
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Functions to find or download packages if we can't find_package
include(FetchContent)

# Our custom cmake functions
include(cmake/functions.cmake)

#######################################################
### Declare options                                 ###
#######################################################
# Set variables with project properties
set_master_project_booleans() # detect if master project / dev mode
set_debug_booleans() # detect if debug
set_compiler_booleans() # detect compiler

# What to build
option(MATPLOTPP_BUILD_EXAMPLES "Build examples" ${MASTER_PROJECT})
if (${MASTER_PROJECT} AND BUILD_TESTING)
    option(MATPLOTPP_BUILD_TESTS "Build tests" ON)
else()
    option(MATPLOTPP_BUILD_TESTS "Build tests" OFF)
endif()
option(MATPLOTPP_BUILD_INSTALLER "Build installer target" ${MASTER_PROJECT})
option(MATPLOTPP_BUILD_PACKAGE "Build package" ${MASTER_PROJECT})

# How to build
option(MATPLOTPP_BUILD_WITH_PEDANTIC_WARNINGS "Use pedantic warnings. This is useful for developers because many of these warnings will be in continuous integration anyway." ${DEBUG_MODE})
option(MATPLOTPP_BUILD_SHARED_LIBS "Build shared libraries" ${BUILD_SHARED_LIBS})
option(MATPLOTPP_BUILD_WITH_SANITIZERS "Use pedantic warnings." ${DEBUG_MODE})

# MSVC hacks
option(MATPLOTPP_BUILD_WITH_MSVC_HACKS "Accept utf-8 in MSVC by default." ON)
option(MATPLOTPP_BUILD_WITH_UTF8 "Accept utf-8 in MSVC by default." ON)
option(MATPLOTPP_BUILD_WITH_EXCEPTIONS "Add compiler flags to use exceptions." ON)

# Features
option(MATPLOTPP_BUILD_HIGH_RESOLUTION_WORLD_MAP "Compile the high resolution maps for geoplots" ON)
option(MATPLOTPP_BUILD_FOR_DOCUMENTATION_IMAGES "Bypass show() commands and save figures as .svg at destruction" OFF)
option(MATPLOTPP_BUILD_EXPERIMENTAL_OPENGL_BACKEND "Compile target with the experimental OpenGL backend" OFF)

# Where to find dependencies
option(MATPLOTPP_WITH_SYSTEM_CIMG "Use system-provided CImg.h instead of bundled" OFF)
option(MATPLOTPP_WITH_SYSTEM_NODESOUP "Use system-provided nodesoup instead of bundled" OFF)

#######################################################
### Apply global options                            ###
#######################################################
# In development, we can set some options for all targets
if (MASTER_PROJECT)
    message("Setting global options")
    # Maybe add sanitizers to all targets
    if (MATPLOTPP_BUILD_WITH_SANITIZERS AND NOT EMSCRIPTEN)
        add_sanitizers()
    endif ()

    # Allow exceptions in MSVC
    if (MSVC AND MATPLOTPP_BUILD_WITH_EXCEPTIONS)
        add_compile_options(/EHsc)
    endif ()

    # Allow utf-8 in MSVC
    if (MATPLOTPP_BUILD_WITH_UTF8 AND MSVC)
        set(CMAKE_CXX_FLAGS "/utf-8")
    endif ()

    # MSVC hack to disable windows min/max
    # http://www.suodenjoki.dk/us/archive/2010/min-max.htm
    if (MSVC AND MATPLOTPP_BUILD_WITH_MSVC_HACKS)
        # Check for min in Windows.h
        # include(CheckSymbolExists)
        # check_symbol_exists(min "WinDef.h" HAVE_WINDOWS_MINMAX)
        # if (NOT HAVE_WINDOWS_MINMAX)
        #     check_symbol_exists(min "Windows.h" HAVE_WINDOWS_MINMAX)
        # endif ()
        # if (HAVE_WINDOWS_MINMAX)
        add_compile_definitions(NOMINMAX)
        # endif ()
    endif ()
endif()

#######################################################
### Libraries                                       ###
#######################################################
add_subdirectory(source)

#######################################################
### Tests                                           ###
#######################################################
if (MATPLOTPP_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif ()

#######################################################
### Examples                                        ###
#######################################################
if (MATPLOTPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

#######################################################
### Installer                                       ###
#######################################################
if (MATPLOTPP_BUILD_INSTALLER)
    # https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html
    # Set variable where the cmake config is
    set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)
    message("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    message("CMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}")

    # Create Matplot++ConfigVersion.cmake and install it
    write_basic_package_version_file(
            Matplot++ConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY SameMajorVersion
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Matplot++ConfigVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)

    # Create Matplot++Config.cmake from Matplot++Config.cmake.in
    set(INCLUDE_INSTALL_DIR include/)
    set(LIB_INSTALL_DIR lib/)
    configure_package_config_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/Matplot++Config.cmake.in # input file
            ${CMAKE_CURRENT_BINARY_DIR}/Matplot++Config.cmake    # output file
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++
            PATH_VARS CMAKE_INSTALL_LIBDIR INCLUDE_INSTALL_DIR LIB_INSTALL_DIR
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Matplot++Config.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)
endif ()

#######################################################
### Packages                                        ###
#######################################################
if (MATPLOTPP_BUILD_INSTALLER AND MATPLOTPP_BUILD_PACKAGE)
    # Set the cpack variables
    # https://cliutils.gitlab.io/modern-cmake/chapters/install/packaging.html

    # The most common cpack variables
    set(CPACK_PACKAGE_VENDOR "Matplot++")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Matplot++: A C++ Graphics Library for Data Visualization")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${MATPLOT_ROOT_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${MATPLOT_ROOT_DIR}/README.md")

    # Set CPACK_SOURCE_IGNORE_FILES with files source packages shouldn't install
    # We get these from .gitignore to avoid redundancy
    FILE(READ .gitignore GITIGNORE_CONTENTS)
    STRING(REGEX REPLACE ";" "\\\\;" GITIGNORE_CONTENTS "${GITIGNORE_CONTENTS}")
    STRING(REGEX REPLACE "\n" ";" GITIGNORE_CONTENTS "${GITIGNORE_CONTENTS}")
    set(CPACK_SOURCE_IGNORE_FILES ${GITIGNORE_CONTENTS})

    # Always include CPack at last
    include(CPack)
endif ()
