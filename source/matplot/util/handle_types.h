//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_HANDLE_TYPES_H
#define MATPLOTPLUSPLUS_HANDLE_TYPES_H

#include <array>
#include <memory>

namespace matplot {
    using color_array = std::array<float, 4>;

    class figure_type;
    using figure_handle = std::shared_ptr<class figure_type>;

    class axes_type;
    using axes_handle = std::shared_ptr<class axes_type>;

    class axes_object;
    using axes_object_handle = std::shared_ptr<class axes_object>;

    class legend;
    using legend_handle = std::shared_ptr<class legend>;

    class string_function;
    using string_function_handle = std::shared_ptr<class string_function>;

    class line;
    using line_handle = std::shared_ptr<class line>;
    using scatter_handle = std::shared_ptr<class line>;

    class stair;
    using stair_handle = std::shared_ptr<class stair>;

    class error_bar;
    using error_bar_handle = std::shared_ptr<class error_bar>;

    class filled_area;
    using filled_area_handle = std::shared_ptr<class filled_area>;

    class function_line;
    using function_line_handle = std::shared_ptr<class function_line>;

    class histogram;
    using histogram_handle = std::shared_ptr<class histogram>;

    class box_chart;
    using box_chart_handle = std::shared_ptr<class box_chart>;

    class parallel_lines;
    using parallel_lines_handle = std::shared_ptr<class parallel_lines>;

    class circles;
    using circles_handle = std::shared_ptr<class circles>;

    class labels;
    using labels_handle = std::shared_ptr<class labels>;

    class matrix;
    using matrix_handle = std::shared_ptr<class matrix>;

    class bars;
    using bars_handle = std::shared_ptr<class bars>;

    class vectors;
    using vectors_handle = std::shared_ptr<class vectors>;

    class contours;
    using contours_handle = std::shared_ptr<class contours>;

    class surface;
    using surface_handle = std::shared_ptr<class surface>;

    class network;
    using network_handle = std::shared_ptr<class network>;

    /// Bin scatter style
    enum class bin_scatter_style {
        automatic,   // decide automatically depending on the data
        point_size,  // size proportional to values in the bin
        point_alpha, // number of elements in bin represented by colormap where
                     // alpha varies with density
        jitter, // create a number of points in the bin proportional to the bin
                // value
        point_colormap, // number of elements in bin represented by default
                        // colormap
        heatmap // number of elements in each bin represented by a heatmap
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_HANDLE_TYPES_H
