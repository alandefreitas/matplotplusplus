//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_BOX_CHART_H
#define MATPLOTPLUSPLUS_BOX_CHART_H

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
    class box_chart : public axes_object {
      public:
        enum class box_style_option {
            outline, // unfilled / dashed whiskers
            filled,  // filled / solid whiskers
            violin,  // show data distribution
        };

      public:
        explicit box_chart(class axes_type *parent);
        box_chart(class axes_type *parent, const std::vector<double> &y_data,
                  const std::vector<double> &groups = {});
        //        box_chart(class xlim* parent, const std::vector<double>& data,
        //        const std::vector<double>& edges, enum
        //        box_chart::normalization normalization_alg =
        //        box_chart::normalization::count); box_chart(class xlim*
        //        parent, const std::vector<double>& data, binning_algorithm
        //        algorithm = binning_algorithm::automatic,
        //        box_chart::normalization normalization_alg =
        //        box_chart::normalization::count);

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        box_chart(const axes_handle &parent, Args&&... args)
            : box_chart(parent.get(), std::forward<Args>(args)...) {}

        virtual ~box_chart() = default;
      public /* xlim object virtual functions */:
        std::string set_variables_string() override;
        std::string plot_string() override;
        // std::string legend_string(const std::string& title) override;
        std::string data_string() override;
        std::string unset_variables_string() override;
        bool requires_colormap() override;
        // double xmax() override;
        // double xmin() override;
        // double ymax() override;
        // double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* useful functions for box_charts */:
      public /* getters and setters */:
        const std::vector<double> &y_data() const;

        class box_chart &y_data(const std::vector<double> &y_data);

        const std::vector<double> &x_data() const;

        class box_chart &x_data(const std::vector<double> &x_data);

        const color_array &face_color() const;

        class box_chart &face_color(const color_array &face_color);

        bool manual_face_color() const;

        class box_chart &manual_face_color(bool manual_face_color);

        const color_array &edge_color() const;

        class box_chart &edge_color(const color_array &edge_color);

        float edge_width() const;

        class box_chart &edge_width(float edge_width);

        enum line_spec::marker_style whisker_style() const;

        class box_chart &
        whisker_style(enum line_spec::marker_style whisker_style);

        float whisker_size() const;

        class box_chart &whisker_size(float whisker_size);

        bool whisker_face() const;

        class box_chart &whisker_face(bool whisker_face);

        bool jitter_outliers() const;

        class box_chart &jitter_outliers(bool jitter_outliers);

        float box_width() const;

        class box_chart &box_width(float box_width);

        float cap_size() const;

        class box_chart &cap_size(float cap_size);

      public /* getters and setters bypassing the line_spec */:
        box_style_option box_style() const;
        class box_chart &box_style(box_style_option box_style);

      private /* helper functions to generate the plot */:
        void maybe_update_face_color();

      protected:
        // sample data
        std::vector<double> y_data_;

        // sample groups (same length as y_data_)
        // this describes the groups and the positions at the same time
        std::vector<double> x_data_;

        // color and style
        color_array face_color_{{0.4f, 0, 0, 0}};
        bool manual_face_color_{false};
        color_array edge_color_{0, 0, 0, 0};
        float edge_width_{0.5};
        enum line_spec::marker_style whisker_style_{
            line_spec::marker_style::circle};
        float whisker_size_{1.};
        bool whisker_face_{false};
        bool jitter_outliers_{false};
        float box_width_{0.5};
        box_style_option box_style_{box_style_option::filled};
        float cap_size_{3.};
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_BOX_CHART_H
