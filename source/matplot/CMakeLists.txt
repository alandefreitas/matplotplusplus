find_package(Filesystem REQUIRED)

add_library(matplot
        matplot.h

        backend/backend_interface.h
        backend/backend_interface.cpp
        backend/gnuplot.h
        backend/gnuplot.cpp
        backend/backend_registry.h
        backend/backend_registry.cpp

        core/axes.cpp
        core/axes.h
        core/axes_object.cpp
        core/axes_object.h
        core/axis.cpp
        core/axis.h
        core/figure.cpp
        core/figure.h
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

target_include_directories(matplot PUBLIC ${MATPLOT_ROOT_DIR}/source)
target_link_libraries(matplot PUBLIC nodesoup cimg std::filesystem)

# https://cmake.org/cmake/help/v3.14/manual/cmake-compile-features.7.html#requiring-language-standards
target_compile_features(matplot PUBLIC cxx_std_17)

# Hacks to support MSVC
if(MSVC)
    # World maps require this option because there is so much in the file
    target_compile_options(matplot PRIVATE /bigobj)
    target_compile_options(matplot PUBLIC /wd4305)
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
endif()

if (BUILD_HIGH_RESOLUTION_WORLD_MAP)
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_HIGH_RESOLUTION_WORLD_MAP)
else()
    message("Not including the high resolution maps for geoplots")
endif()

# Library for the OpenGL example
# This is an example of what an OpenGL backend *could* look like.
# We would need to adapt the axes objects to feed this backend
#     with polygons and colors instead of commands, which is not
#     a problem at all. The data is all prepared in these objects.
#     We would not have to convert this data to strings and would
#     not have to worry about an external syntax.
# The biggest challenge is to actually create backends that can
#     draw these polygons we feed them.
# We discuss some of these difficulties in the documentation
#     and the backend_interface.h.
find_package(OpenGL)
find_package(GLAD)
find_package(glfw3 QUIET)
if (OpenGL_FOUND AND GLAD_FOUND AND glfw3_FOUND)
    add_library(matplot_opengl backend/opengl_3.h backend/opengl_3.cpp)
    target_include_directories(matplot_opengl PUBLIC matplot ${GLAD_INCLUDE_PATH})
    target_link_libraries(matplot_opengl PUBLIC matplot ${GLAD_LIBRARIES} glfw)
endif()
