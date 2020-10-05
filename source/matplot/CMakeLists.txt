find_package(Filesystem REQUIRED)

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

target_include_directories(matplot
    PUBLIC $<BUILD_INTERFACE:${MATPLOT_ROOT_DIR}/source>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(matplot
    PRIVATE cimg nodesoup std::filesystem)

# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(matplot PUBLIC cxx_std_17)

# Definition to find the proper filesystem library
if (CXX_FILESYSTEM_IS_EXPERIMENTAL)
    target_compile_definitions(matplot PRIVATE CXX_FILESYSTEM_IS_EXPERIMENTAL)
endif()

# Hacks to support MSVC
if(MSVC)
    # World maps require this option because there is so much in the file
    target_compile_options(matplot PRIVATE /bigobj)
    target_compile_options(matplot PUBLIC /wd4305)
    # Fix compile error caused by utf8 character in line_spec.cpp
    target_compile_options(matplot PUBLIC /utf-8)
    # Allow exceptions
    target_compile_options(matplot PRIVATE /EHsc)
endif()

include(CheckSymbolExists)

# Some hack to not depend on FILE* internals
# https://github.com/alandefreitas/matplotplusplus/issues/4
check_symbol_exists(__fbufsize "stdio_ext.h" HAVE_FBUFSIZE)
if (HAVE_FBUFSIZE)
    target_compile_definitions(matplot PRIVATE MATPLOT_HAS_FBUFSIZE)
endif()

# Another hack to check for min in Windows.h
# http://www.suodenjoki.dk/us/archive/2010/min-max.htm
check_symbol_exists(min "Windows.h" HAVE_WINDOWS_MINMAX)
if (HAVE_WINDOWS_MINMAX)
    target_compile_definitions(matplot PUBLIC NOMINMAX)
endif()

if (BUILD_FOR_DOCUMENTATION_IMAGES)
    message("Building matplot for documentation images. wait() commands will be ignored. ~figure will save the files.")
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES)
endif ()

if (BUILD_HIGH_RESOLUTION_WORLD_MAP)
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_HIGH_RESOLUTION_WORLD_MAP)
else ()
    message("Not including the high resolution maps for geoplots")
endif ()

if (BUILD_WITH_PEDANTIC_WARNINGS)
    if (MSVC)
        target_compile_options(matplot PRIVATE /W4 /WX)
    else ()
        target_compile_options(matplot PRIVATE -Wall -Wextra -pedantic -Werror)
        # Allow the warnings related to the bundled CImg
        if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            target_compile_options(matplot PRIVATE -Wno-null-pointer-arithmetic -Wno-char-subscripts)
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(matplot PRIVATE -Wno-error=class-memaccess -Wno-class-memaccess)
        else ()
            message(ERROR "Cannot disable the relevant warnings for ${CMAKE_CXX_COMPILER_ID}")
        endif ()
    endif ()
endif ()

if (BUILD_EXPERIMENTAL_OPENGL_BACKEND)
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
    find_package(OpenGL REQUIRED)

    # https://github.com/Dav1dde/glad
    find_package(GLAD QUIET)
    if (NOT GLAD_FOUND AND NOT TARGET glad)
        # Use CPM only if not found, to avoid ODR violations
        # find_package(GLAD REQUIRE) would suffice if it worked well
        CPMAddPackage(NAME glad GIT_REPOSITORY https://github.com/Dav1dde/glad GIT_TAG df8e9e16110b305479a875399cee13daa0ccadd9 VERSION 0.1.33)
    else ()
        # FindGLAD does not usually create a target, so we create an interface target
        if (NOT TARGET glad)
            add_library(glad INTERFACE)
            target_include_directories(glad INTERFACE ${GLAD_INCLUDE_PATH})
            target_link_libraries(glad INTERFACE ${GLAD_LIBRARIES})
        endif ()
    endif ()

    # https://github.com/glfw/glfw
    find_package(glfw3 QUIET)
    if (NOT GLFW3_FOUND AND NOT TARGET glfw)
        # Use CPM only if not found, to avoid ODR violations
        # find_package(glfw3 REQUIRE) would suffice if it worked well
        CPMAddPackage(NAME glfw3 GIT_REPOSITORY https://github.com/glfw/glfw VERSION 3.3.2 GIT_TAG 3.3.2 OPTIONS "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "GLFW_INSTALL OFF")
    endif ()

    add_library(matplot_opengl
            backend/opengl_embed.h
            backend/opengl_embed.cpp
            backend/opengl.h
            backend/opengl.cpp
            )
    target_include_directories(matplot_opengl PUBLIC matplot)
    target_link_libraries(matplot_opengl PUBLIC matplot glad glfw ${CMAKE_DL_LIBS})
endif()


# Install
if (BUILD_INSTALLER)
    # Install targets
    install(TARGETS matplot
            EXPORT Matplot++Targets
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