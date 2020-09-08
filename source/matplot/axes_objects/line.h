//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_LINE_H
#define MATPLOTPLUSPLUS_LINE_H

#include <array>
#include <matplot/core/axes_object.h>
#include <matplot/core/figure_type.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

namespace matplot {
    class axes_type;
    class line : public axes_object {
      public:
        explicit line(class axes_type *parent);
        line(class axes_type *parent, const std::vector<double> &y_data,
             std::string_view line_spec = "");
        line(class axes_type *parent, const std::vector<double> &x_data,
             const std::vector<double> &y_data,
             std::string_view line_spec = "");
        line(class axes_type *parent, const std::vector<double> &x_data,
             const std::vector<double> &y_data,
             const std::vector<double> &z_data,
             std::string_view line_spec = "");

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        line(const axes_handle &parent, Args&&... args)
            : line(parent.get(), std::forward<Args>(args)...) {}

      public /* mandatory virtual functions */:
        void run_draw_commands() override;

        std::string plot_string() override;
        std::string legend_string(std::string_view title) override;
        std::string data_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;
        bool requires_colormap() override;

      public /* getters and setters */:
        class line &line_style(std::string_view line_spec);

        const matplot::line_spec &line_spec() const;
        matplot::line_spec &line_spec();
        class line &line_spec(const class line_spec &line_spec);

        const std::vector<double> &y_data() const;
        class line &y_data(const std::vector<double> &y_data);

        const std::vector<double> &x_data() const;
        class line &x_data(const std::vector<double> &x_data);

        const std::vector<double> &z_data() const;
        class line &z_data(const std::vector<double> &z_data);

        const std::vector<size_t> &marker_indices() const;
        class line &marker_indices(const std::vector<size_t> &marker_indices);

        bool use_y2() const;
        class line &use_y2(bool use_y_2);

        bool impulse() const;
        class line &impulse(bool impulse);

        bool fill() const;
        class line &fill(bool fill);

        bool use_y_2() const;
        class line &use_y_2(bool use_y_2);

        bool polar() const;
        class line &polar(bool polar);

        bool visible() const;
        class line &visible(bool visible);

      public /* getters and setters bypassing the line_spec */:
        float line_width() const;
        class line &line_width(float line_width);

        enum line_spec::marker_style marker_style() const;
        template <class T> line &marker_style(T marker_style) {
            line_spec_.marker_style(marker_style);
            return *this;
        }

        enum line_spec::marker_style marker() const;
        template <class T> line &marker(T marker) {
            line_spec_.marker(marker);
            return *this;
        }

        float marker_size() const;
        class line &marker_size(float size);
        class line &marker_size(const std::vector<float> &size_vector);
        class line &marker_size(const std::vector<double> &size_vector);

        bool marker_face() const;
        class line &marker_face(bool size);

        const std::array<float, 4> &color() const;
        template <class T> line &color(T c) {
            line_spec().color(c);
            return *this;
        }

        inline class line &color(std::initializer_list<float> c) {
            line_spec().color(c);
            return *this;
        }

        const std::array<float, 4> &marker_color() const;

        template <class T> line &marker_color(T c) {
            line_spec().marker_color(c);
            return *this;
        }

        inline class line &marker_color(std::initializer_list<float> c) {
            line_spec().marker_color(c);
            return *this;
        }

        inline class line &marker_colors(const std::vector<double> &cs) {
            marker_colors_ = cs;
            touch();
            return *this;
        }

        const std::array<float, 4> &marker_face_color() const;

        template <class T> line &marker_face_color(T c) {
            line_spec().marker_face_color(c);
            return *this;
        }

        inline class line &marker_face_color(std::initializer_list<float> c) {
            line_spec().marker_face_color(c);
            return *this;
        }

        inline float marker_face_alpha() {
            return line_spec().marker_face_alpha();
        }

        inline class line &marker_face_alpha(float a) {
            line_spec().marker_face_alpha(a);
            return *this;
        }

      protected:
        virtual std::vector<line_spec::style_to_plot> styles_to_plot();
        void maybe_update_line_spec();

      protected:
        /// Line style
        matplot::line_spec line_spec_;

        /// Data in the xlim
        std::vector<double> y_data_{};
        std::vector<double> x_data_{};
        std::vector<double> z_data_{};

        /// Positions at which we want markers to appear
        std::vector<size_t> marker_indices_{};
        std::vector<float> marker_sizes_{};
        std::vector<double> marker_colors_{};

        /// Draw line as impulse
        bool impulse_{false};

        /// Draw line as filled area
        bool fill_{false};

        /// Use the y2 xlim
        bool use_y2_{false};

        /// This line is meant for a polar plot
        bool polar_{false};

        /// True if visible
        bool visible_{true};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_LINE_H
