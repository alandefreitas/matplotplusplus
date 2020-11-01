#######################################################
### Matplot++                                       ###
#######################################################
# Project information
cmake_minimum_required(VERSION 3.14)
project(matplotplusplus VERSION 1.0.1)
set(CMAKE_CXX_STANDARD 17)

# CMake dependencies for installer
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# To find or download packages
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
message("CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
option(CPM_USE_LOCAL_PACKAGES "Try `find_package` before downloading dependencies" ON)
include(cmake/CPM.cmake)

# Check if this is a master project or a subdirectory of another project
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(MASTER_PROJECT ON)
else ()
    set(MASTER_PROJECT OFF)
endif ()
set(MATPLOT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Check if we are in debug mode
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(DEBUG_MODE ON)
    set(NOT_DEBUG_MODE OFF)
else()
    set(DEBUG_MODE OFF)
    set(NOT_DEBUG_MODE ON)
endif()

#######################################################
### Options                                         ###
#######################################################
# What to build
option(BUILD_EXAMPLES "Build examples" ${MASTER_PROJECT})
option(BUILD_TESTS "Build tests" ${MASTER_PROJECT})
option(BUILD_INSTALLER "Build installer target" ${MASTER_PROJECT})
option(BUILD_PACKAGE "Build package" ${MASTER_PROJECT})

# How to build
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_HIGH_RESOLUTION_WORLD_MAP "Compile the high resolution maps for geoplots" ON)
option(BUILD_FOR_DOCUMENTATION_IMAGES "Bypass show() commands and save figures as .svg at destruction" OFF)
option(BUILD_EXPERIMENTAL_OPENGL_BACKEND "Compile target with the experimental OpenGL backend" OFF)
option(BUILD_WITH_PEDANTIC_WARNINGS "Use pedantic warnings. This is useful for developers because many of these warnings will be in continuous integration anyway." ${DEBUG_MODE})
option(BUILD_WITH_UTF8 "Accept utf-8 in MSVC by default." ON)

# Where to find dependencies
option(WITH_SYSTEM_CIMG "Use system-provided CImg.h instead of bundled" OFF)
option(WITH_SYSTEM_NODESOUP "Use system-provided nodesoup instead of bundled" OFF)

#######################################################
### Libraries                                       ###
#######################################################
add_subdirectory(source)

#######################################################
### Examples and tests                              ###
#######################################################
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

#######################################################
### Installer                                       ###
#######################################################
if (BUILD_INSTALLER)
    # https://cliutils.gitlab.io/modern-cmake/chapters/install/installing.html
    # Set variable where the cmake config is
    set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)
    message("CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    message("CMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}")

    # Create Matplot++ConfigVersion.cmake and install it
    write_basic_package_version_file(
            Matplot++ConfigVersion.cmake
            VERSION ${PACKAGE_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Create matplot++-config.cmake from matplot++-config.cmake.in
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/matplot++-config.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/matplot++-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)

    # Install the file matplot++-config.cmake
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/matplot++-config.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)

    # Install cmake to find filesystem as a dependency
    if (NOT BUILD_SHARED_LIBS)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindFilesystem.cmake
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++)
    endif ()
endif ()

#######################################################
### Packages                                        ###
#######################################################
if (BUILD_INSTALLER AND BUILD_PACKAGE)
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