//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_MATPLOT_H
#define MATPLOTPLUSPLUS_MATPLOT_H

// Common / util
#include <matplot/util/common.h>
#include <matplot/util/concepts.h>
#include <matplot/util/geodata.h>
#include <matplot/util/handle_types.h>
#include <matplot/util/type_traits.h>

// Backends
#include <matplot/backend/backend_interface.h>
#include <matplot/backend/backend_registry.h>
#include <matplot/backend/gnuplot.h>
// #include <matplot/backend/opengl_3.h> // Don't include opengl by default

// Figure and axes
#include <matplot/core/axes_type.h>
#include <matplot/core/axis_type.h>
#include <matplot/core/figure_type.h>

// Axes objects
#include <matplot/axes_objects/bars.h>
#include <matplot/axes_objects/box_chart.h>
#include <matplot/axes_objects/circles.h>
#include <matplot/axes_objects/contours.h>
#include <matplot/axes_objects/error_bar.h>
#include <matplot/axes_objects/filled_area.h>
#include <matplot/axes_objects/function_line.h>
#include <matplot/axes_objects/histogram.h>
#include <matplot/axes_objects/labels.h>
#include <matplot/axes_objects/line.h>
#include <matplot/axes_objects/matrix.h>
#include <matplot/axes_objects/network.h>
#include <matplot/axes_objects/parallel_lines.h>
#include <matplot/axes_objects/stair.h>
#include <matplot/axes_objects/string_function.h>
#include <matplot/axes_objects/surface.h>
#include <matplot/axes_objects/vectors.h>

// Free-standing functions
#include <matplot/freestanding/axes_functions.h>
#include <matplot/freestanding/axes_lim.h>
#include <matplot/freestanding/histcounts.h>
#include <matplot/freestanding/plot.h>

#endif // MATPLOTPLUSPLUS_MATPLOT_H
