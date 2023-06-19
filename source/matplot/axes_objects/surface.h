//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_SURFACE_H
#define MATPLOTPLUSPLUS_SURFACE_H

#include <matplot/detail/config.h>
#include <matplot/core/figure_type.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>
#include <matplot/core/axes_object.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/common.h>
#include <array>
#include <optional>

namespace matplot {
    class axes_type;

    /// Surfaces might include data for contours but, if you only want contours,
    /// it's best to use the contour, contourf, fcontour functions to plot it in
    /// 2d
    class MATPLOT_EXPORTS surface : public axes_object {
      public:
        explicit surface(class axes_type *parent);

        /// Grid surface
        surface(class axes_type *parent, const vector_2d &X, const vector_2d &Y,
                const vector_2d &Z, const vector_2d &C,
                std::string_view line_spec = "");

        /// Parametric surface
        //        surface(class xlim* parent, const vector_1d& x, const
        //        vector_1d& y, const vector_1d& z, const vector_1d& c, const
        //        std::string& line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        surface(const axes_handle &parent, Args&&... args)
            : surface(parent.get(), std::forward<Args>(args)...) {}

        virtual ~surface() = default;
      public /* mandatory virtual functions */:
        std::string set_variables_string() override;
        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        class surface &line_style(std::string_view line_spec);

        const matplot::line_spec &line_spec() const;
        matplot::line_spec &line_spec();
        class surface &line_spec(const class line_spec &line_spec);

        const vector_2d &Y_data() const;
        class surface &Y_data(const vector_2d &Y_data);

        const vector_2d &X_data() const;
        class surface &X_data(const vector_2d &X_data);

        const vector_2d &Z_data() const;
        class surface &Z_data(const vector_2d &Z_data);

        size_t norm() const;
        class surface &norm(size_t norm);

        const vector_2d &x_data() const;
        class surface &x_data(const vector_2d &x_data);

        const vector_2d &y_data() const;
        class surface &y_data(const vector_2d &y_data);

        const vector_2d &z_data() const;
        class surface &z_data(const vector_2d &z_data);

        bool hidden_3d() const;
        class surface &hidden_3d(bool hidden_3_d);

        bool surface_visible() const;
        class surface &surface_visible(bool surface_visible);

        bool surface_in_2d() const;
        class surface &surface_in_2d(bool surface_in_2d);

        bool palette_map_at_bottom() const;
        class surface &palette_map_at_bottom(bool palette_map_at_bottom);

        bool palette_map_at_surface() const;
        class surface &palette_map_at_surface(bool palette_map_at_surface);

        bool palette_map_at_top() const;
        class surface &palette_map_at_top(bool palette_map_at_top);

        bool contour_base() const;
        class surface &contour_base(bool contour_base);

        bool contour_surface() const;
        class surface &contour_surface(bool contour_surface);

        size_t contour_levels() const;
        class surface &contour_levels(size_t contour_levels);

        const std::vector<double> &contour_values() const;
        class surface &
        contour_values(const std::vector<double> &contour_values);

        bool contour_text() const;
        class surface &contour_text(bool contour_text);

        float font_size() const;
        class surface &font_size(const float &font_size);

        const std::string font() const;
        class surface &font(std::string_view font);

        const std::string &font_weight() const;
        class surface &font_weight(std::string_view font_weight);

        const color_array &font_color() const;
        class surface &font_color(const color_array &font_color);
        class surface &font_color(std::string_view font_color);

        bool depthorder() const;

        class surface &depthorder(bool depthorder);

        float face_alpha() const;
        class surface &face_alpha(float face_alpha);

        bool lighting() const;
        class surface &lighting(bool lighting);

        float primary() const;
        class surface &primary(float amount);

        float specular() const;
        class surface &specular(float amount);

        const class line_spec &contour_line_spec() const;
        class line_spec &contour_line_spec();
        class surface &
        contour_line_spec(const class line_spec &contour_line_spec);

        bool curtain() const;
        class surface &curtain(bool curtain);

        bool waterfall() const;
        class surface &waterfall(bool waterfall);

        bool fences() const;
        class surface &fences(bool fences);

        bool ribbons() const;
        class surface &ribbons(bool ribbons);

        double ribbon_width() const;
        class surface &ribbon_width(double ribbon_width);

        bool visible() const;
        class surface &visible(bool visible);

      public /* getters and setters bypassing the line_spec */:
        float line_width() const;
        class surface &line_width(float line_width);

        const std::array<float, 4> &edge_color() const;

        template <class T> surface &edge_color(T c) {
            line_spec().color(c);
            touch();
            return *this;
        }

        inline class surface &edge_color(std::initializer_list<float> c) {
            line_spec().color(c);
            touch();
            return *this;
        }

        double zmin() override;
        double zmax() override;

      protected:
        void maybe_update_line_spec();
        std::string grid_data_string();
        std::string ribbon_data_string();
        size_t create_line_index();

      protected:
        /// Data in the xlim
        vector_2d X_data_{};
        vector_2d Y_data_{};
        vector_2d Z_data_{};
        vector_2d C_data_{};

        /// Interpret data as a flat array of parametrics values
        /// If false, data needs to represent a grid
        /// If true, data represents a free form
        bool is_parametric_{false};

        /// Preprocess zmin / zmax
        double zmin_{NaN};
        double zmax_{NaN};

        /// Style
        size_t norm_{2};
        bool hidden3d_{false};
        bool depthorder_{false};
        float face_alpha_{.95f};
        class line_spec line_spec_;
        bool lighting_{false};
        float primary_{-1.};
        float specular_{-1.};
        bool curtain_{false};
        bool waterfall_{false};
        bool fences_{false};
        bool ribbons_{false};
        double ribbon_width_{0.75};

        // Line surface
        bool surface_visible_{true};
        bool surface_in_2d_{false};

        // Solid surface
        bool palette_map_at_bottom_{false};
        bool palette_map_at_surface_{true};
        bool palette_map_at_top_{false};

        class line_spec contour_line_spec_;
        bool contour_base_{false};
        bool contour_surface_{false};
        size_t contour_levels_{9};
        std::vector<double> contour_values_{};

        bool contour_text_{false};
        std::optional<float> font_size_{std::nullopt};
        std::optional<std::string> font_{std::nullopt};
        std::string font_weight_{"normal"};
        color_array font_color_{0, 0, 0, 0};

        /// True if visible
        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_SURFACE_H
