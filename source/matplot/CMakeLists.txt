#######################################################
### Library                                         ###
#######################################################
add_library(matplot
        matplot.h

        backend/backend_interface.h
        backend/backend_interface.cpp
        backend/gnuplot.h
        backend/gnuplot.cpp
        backend/backend_registry.h
        backend/backend_registry.cpp

        core/axes_type.cpp
        core/axes_type.h
        core/axes_object.cpp
        core/axes_object.h
        core/axis_type.cpp
        core/axis_type.h
        core/figure_type.cpp
        core/figure_type.h
        core/figure_registry.cpp
        core/figure_registry.h
        core/legend.cpp
        core/legend.h
        core/line_spec.cpp
        core/line_spec.h

        util/colors.cpp
        util/colors.h
        util/common.cpp
        util/common.h
        util/concepts.h
        util/contourc.cpp
        util/contourc.h
        util/geodata.h
        util/handle_types.h
        util/keywords.h
        util/popen.h
        util/popen.cpp
        util/type_traits.h
        util/world_cities.cpp
        util/world_map_10m.cpp
        util/world_map_50m.cpp
        util/world_map_110m.cpp

        axes_objects/bars.cpp
        axes_objects/bars.h
        axes_objects/box_chart.cpp
        axes_objects/box_chart.h
        axes_objects/circles.cpp
        axes_objects/circles.h
        axes_objects/contours.cpp
        axes_objects/contours.h
        axes_objects/error_bar.cpp
        axes_objects/error_bar.h
        axes_objects/filled_area.cpp
        axes_objects/filled_area.h
        axes_objects/function_line.cpp
        axes_objects/function_line.h
        axes_objects/histogram.cpp
        axes_objects/histogram.h
        axes_objects/labels.cpp
        axes_objects/labels.h
        axes_objects/line.cpp
        axes_objects/line.h
        axes_objects/matrix.cpp
        axes_objects/matrix.h
        axes_objects/network.cpp
        axes_objects/network.h
        axes_objects/parallel_lines.cpp
        axes_objects/parallel_lines.h
        axes_objects/stair.cpp
        axes_objects/stair.h
        axes_objects/string_function.cpp
        axes_objects/string_function.h
        axes_objects/surface.cpp
        axes_objects/surface.h
        axes_objects/vectors.cpp
        axes_objects/vectors.h

        freestanding/axes_functions.cpp
        freestanding/axes_functions.h
        freestanding/axes_lim.h
        freestanding/histcounts.h
        freestanding/histcounts.cpp
        freestanding/plot.h
)

set(TARGETS matplot)

# Target aliases
add_library(Matplot++::matplot ALIAS matplot)
add_library(Matplot++::matplot++ ALIAS matplot)

# Include dirs
target_include_directories(matplot
    PUBLIC $<BUILD_INTERFACE:${MATPLOT_ROOT_DIR}/source>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(matplot PROPERTIES VERSION ${PROJECT_VERSION})

# Shared library symbol export
include(GenerateExportHeader)
generate_export_header(matplot
        BASE_NAME matplot
        EXPORT_MACRO_NAME MATPLOT_EXPORTS
        EXPORT_FILE_NAME matplot/detail/exports.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)
target_include_directories(matplot
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/matplot/detail/exports.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/matplot/detail)

# Dependencies
target_link_libraries_system(matplot PRIVATE cimg nodesoup)

# Required compiler features required
# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(matplot PUBLIC cxx_std_17)

#######################################################
### Compiler options                                ###
#######################################################
message("Setting matplotplusplus compiler options")

# Support MSVC
target_bigobj_options(matplot)
target_exception_options(matplot)
target_utf8_options(matplot)
target_nominmax_definition(matplot)

# Warnings
maybe_target_pedantic_warnings(matplot)

#######################################################
### Definitions                                     ###
#######################################################
# Some hack to not depend on FILE* internals
# https://github.com/alandefreitas/matplotplusplus/issues/4
include(CheckSymbolExists)
check_symbol_exists(__fbufsize "stdio_ext.h" HAVE_FBUFSIZE)
if (HAVE_FBUFSIZE)
    target_compile_definitions(matplot PRIVATE MATPLOT_HAS_FBUFSIZE)
endif()

# Build for documentation
if (MATPLOTPP_BUILD_FOR_DOCUMENTATION_IMAGES)
    message("Building matplot for documentation images. wait() commands will be ignored. ~figure will save the files.")
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES)
endif ()

# Include high-resolution world map in the binary
if (MATPLOTPP_BUILD_HIGH_RESOLUTION_WORLD_MAP)
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_HIGH_RESOLUTION_WORLD_MAP)
else ()
    message("Not including the high resolution maps for geoplots")
endif ()

#######################################################
### Library options                                 ###
#######################################################

# Maybe add pedantic warning


#if (MATPLOTPP_BUILD_WITH_PEDANTIC_WARNINGS)
#    if (MSVC)
#        target_compile_options(matplot PRIVATE /W4 /WX)
#    else ()
#        target_compile_options(matplot PRIVATE -Wall -Wextra -pedantic -Werror)
#        # Allow the warnings related to the bundled CImg
#        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
#            target_compile_options(matplot PRIVATE -Wno-null-pointer-arithmetic -Wno-char-subscripts)
#        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#            target_compile_options(matplot PRIVATE -Wno-error=class-memaccess -Wno-class-memaccess)
#        else ()
#            message(ERROR "Cannot disable the relevant warnings for ${CMAKE_CXX_COMPILER_ID}")
#        endif ()
#    endif ()
#endif ()

#######################################################
### Experimental OpenGL backend                     ###
#######################################################
if (MATPLOTPP_BUILD_EXPERIMENTAL_OPENGL_BACKEND)
    # Library for the OpenGL example
    # This is an example of what an OpenGL backend *could* look like.
    #     The opengl backend is currently incomplete.
    # There are two backends for OpenGL:
    #     - opengl_embed: you can put that inside your existing OpenGL
    #       application. You can then call draw() in your render loop
    #       to draw the plots on top of your application.
    #     - opengl: this backend will create it's own window and make
    #       plots there.
    # Qt: If you're using Qt, opengl_embed allows you to create plots in your
    #     application. See https://doc.qt.io/qt-5/qtgui-index.html
    # Web: If you're creating plots for the web, you can use emscripten to
    #     create efficient plots. See:
    #     https://emscripten.org/docs/porting/multimedia_and_graphics/OpenGL-support.html
    # We are continuously making plot categories compatible with these backends.
    # For each category, we need to adapt the axes objects to feed this backend
    #     with polygons and colors instead of commands.
    # Feeding these commands is not a problem at all. The data is all prepared
    #     in the axes objects. Unlike an gnuplot backend, we don't have to convert
    #     this data to strings and do not have to worry about an external syntax.
    # The biggest challenge is to actually create backends that can
    #     draw these polygons we feed them.
    # We discuss some of these difficulties in the documentation
    #     and the backend_interface.h.
    # The biggest pro of the OpenGL backend is that it is very efficient.
    #     Everything is happening on the GPU.
    # The biggest con of the OpenGL backend is that it cannot open a window
    #     in another thread. All it can do is get in the middle of the render
    #     loop and draw the plot.
    find_package(OpenGL)

    # https://github.com/Dav1dde/glad
    find_package(glad CONFIG)
    if (NOT glad_FOUND)
        find_package(GLAD QUIET)
    endif()
    if (NOT glad_FOUND AND NOT GLAD_FOUND)
        # Use CPM only if not found, to avoid ODR violations
        # find_package(GLAD REQUIRE) would suffice if it worked well
        FetchContent_Declare(glad GIT_REPOSITORY https://github.com/Dav1dde/glad.git GIT_TAG v0.1.36)
        FetchContent_MakeAvailable(glad)
    endif()
    if(TARGET glad AND NOT TARGET glad::glad)
        # Alias glad to glad::glad
        add_library(glad::glad ALIAS glad)
        list(APPEND TARGETS glad)
    endif()
    if(NOT TARGET glad::glad)
        # FindGLAD does not usually create a target, so we create an interface target
        add_library(glad::glad INTERFACE)
        target_include_directories(glad::glad INTERFACE ${GLAD_INCLUDE_PATH})
        target_link_libraries(glad::glad INTERFACE ${GLAD_LIBRARIES})
    endif()

    # https://github.com/glfw/glfw
    find_package(glfw3 QUIET)
    if ((NOT glfw3_FOUND AND NOT GLFW3_FOUND) OR NOT TARGET glfw)
        # Use CPM only if not found, to avoid ODR violations
        # find_package(glfw3 REQUIRE) would suffice if it worked well
        FetchContent_Declare(glfw3 GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3.8)
        FetchContent_MakeAvailable(glfw3)
    endif()

    add_library(matplot_opengl
            backend/opengl_embed.h
            backend/opengl_embed.cpp
            backend/opengl.h
            backend/opengl.cpp
            )
    target_link_libraries(matplot_opengl PUBLIC matplot glad::glad glfw ${CMAKE_DL_LIBS})

    list(APPEND TARGETS matplot_opengl)
endif()

#######################################################
### Installer                                       ###
#######################################################
if (MATPLOTPP_BUILD_INSTALLER)
    # Install targets
    install(TARGETS ${TARGETS}
            EXPORT Matplot++Targets
	    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            )

    # Install headers
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
            )

    # Install cmake script
    install(EXPORT Matplot++Targets
            FILE Matplot++Targets.cmake
            NAMESPACE Matplot++::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Matplot++
            )
endif()
