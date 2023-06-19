//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_BARS_H
#define MATPLOTPLUSPLUS_BARS_H

#include <matplot/detail/config.h>

#include <array>
#include <cmath>
#include <map>
#include <matplot/core/figure_type.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/common.h>

namespace matplot {
    class axes_type;
    class MATPLOT_EXPORTS bars : public axes_object {
      public:
        explicit bars(class axes_type *parent);
        bars(class axes_type *parent, const std::vector<double> &y);
        bars(class axes_type *parent,
             const std::vector<std::vector<double>> &Y);
        bars(class axes_type *parent, const std::vector<double> &x,
             const std::vector<double> &y);
        bars(class axes_type *parent, const std::vector<double> &x,
             const std::vector<std::vector<double>> &Y);

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        bars(const axes_handle &parent, Args &&... args)
            : bars(parent.get(), std::forward<Args>(args)...) {}

        virtual ~bars() = default;

      public /* xlim object virtual functions */:
        // std::string set_variables_string() override;
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        // std::string unset_variables_string() override;
        bool requires_colormap() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* useful functions for bars */:
        double x_minimum_difference();
        double x_end_point(size_t cluster_index, size_t index);
        double cluster_width();
        double range_for_cluster();

      public /* getters and setters */:
        const color_array &face_color() const;
        class bars &face_color(const color_array &face_color);
        class bars &face_color(const std::array<float, 3> &face_color);
        class bars &face_color(std::initializer_list<float> face_color);
        class bars &face_color(std::string_view color);

        const std::vector<color_array> &face_colors() const;
        std::vector<color_array> &face_colors();

        class bars &face_colors(const std::vector<color_array> &face_colors);

        bool manual_face_color() const;
        class bars &manual_face_color(bool manual_face_color);

        const color_array &edge_color() const;
        class bars &edge_color(const color_array &edge_color);
        class bars &edge_color(const std::array<float, 3> &edge_color);
        class bars &edge_color(std::initializer_list<float> face_color);
        class bars &edge_color(std::string_view edge_color);

        const line_spec &edge_style() const;
        class bars &edge_style(const line_spec &edge_style);

        float line_width() const;
        class bars &line_width(float line_width);

        bool vertical_orientation() const;
        class bars &vertical_orientation(bool vertical_orientation);

        bool visible() const;
        class bars &visible(bool visible);

        float bar_width() const;
        class bars &bar_width(float bar_width);

      public /* getters and setters bypassing the bars_spec */:
      private:
        /// If the user has not set the face color,
        /// we get a color from the xlim
        void maybe_update_face_colors();

      protected:
        // original data
        std::vector<double> x_;
        std::vector<std::vector<double>> ys_;

        // color and style
        std::vector<color_array> face_colors_{{0.4f, 0, 0, 0}};
        bool manual_face_color_{false};
        color_array edge_color_{0, 0, 0, 0};
        line_spec edge_style_{"-"};
        float line_width_{0.5};
        bool vertical_orientation_{true};
        float bar_width_{0.8f};
        float cluster_width_{0.8f};

        // True if visible
        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_BARS_H
