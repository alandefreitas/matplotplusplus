include(FindDependencies.cmake)

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
target_link_libraries(matplot PUBLIC nodesoup cimg)

if (BUILD_FOR_DOCUMENTATION_IMAGES)
    message("Building matplot for documentation images. wait() commands will be ignored. ~figure will save the files.")
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_FOR_DOCUMENTATION_IMAGES)
endif()

if (BUILD_HIGH_RESOLUTION_WORLD_MAP)
    target_compile_definitions(matplot PUBLIC MATPLOT_BUILD_HIGH_RESOLUTION_WORLD_MAP)
else()
    message("Not including the high resolution maps for geoplots")
endif()

find_package(OpenGL)
if (NOT OPENGL_FOUND)
    return()
endif()

if (NOT TARGET glad)
    CPMAddPackage(NAME glad GIT_REPOSITORY https://github.com/Dav1dde/glad GIT_TAG df8e9e16110b305479a875399cee13daa0ccadd9 VERSION 0.1.33)
endif()

if (NOT TARGET glfw)
    CPMAddPackage(NAME glfw GIT_REPOSITORY https://github.com/glfw/glfw VERSION 3.3.2 GIT_TAG 3.3.2 OPTIONS "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_TESTS OFF" "GLFW_INSTALL OFF")
endif()

# Library for the OpenGL example
add_library(matplot_opengl
        backend/opengl_3.h
        backend/opengl_3.cpp)
target_link_libraries(matplot_opengl PUBLIC matplot glad glfw)
