//
// Created by Alan Freitas on 16/07/20.
//

#ifndef MATPLOTPLUSPLUS_LABELS_H
#define MATPLOTPLUSPLUS_LABELS_H

#include <matplot/core/figure_type.h>

#include <matplot/core/axes_object.h>
#include <matplot/core/axis_type.h>
#include <matplot/core/line_spec.h>
#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

namespace matplot {
    class axes_type;
    class labels : public axes_object {
      public:
        enum alignment { left, right, center, automatic };

      public:
        static constexpr double width_factor = 0.04;
        static constexpr double height_factor = 0.08;

      public:
        explicit labels(class axes_type *parent);
        labels(class axes_type *parent, const std::vector<double> &x,
               const std::vector<double> &y,
               const std::vector<std::string> &labels = {},
               const std::vector<double> &color = {},
               const std::vector<double> &sizes = {});

        /// If we receive an axes_handle, we can convert it to a raw
        /// pointer because there is no ownership involved here
        template <class... Args>
        labels(const axes_handle &parent, Args&&... args)
            : labels(parent.get(), std::forward<Args>(args)...) {}

      public /* mandatory virtual functions */:
        std::string set_variables_string() override;
        std::string plot_string() override;
        // std::string legend_string(const std::string& title) override;
        std::string data_string() override;
        std::string unset_variables_string() override;
        double xmax() override;
        double xmin() override;
        double ymax() override;
        double ymin() override;
        enum axes_object::axes_category axes_category() override;

      public /* getters and setters */:
        bool rectangles() const;
        class labels &rectangles(bool rectangles);

        bool absolute_size() const;
        class labels &absolute_size(bool absolute_size);

        enum alignment alignment() const;
        class labels &alignment(enum alignment alignment);

        const std::vector<double> &x() const;
        class labels &x(const std::vector<double> &x);

        const std::vector<double> &y() const;
        class labels &y(const std::vector<double> &y);

        const std::vector<std::string> &label_values() const;
        class labels &label_values(const std::vector<std::string> &labels);

        const std::vector<double> &colors() const;
        class labels &colors(const std::vector<double> &colors);

        const std::vector<double> &sizes() const;
        class labels &sizes(const std::vector<double> &sizes);

        bool visible() const;
        class labels &visible(bool visible);

        const color_array &color() const;
        class labels &color(const color_array &color);
        template <class T> labels &color(T c) {
            color(to_array(c));
            return *this;
        }

        const std::string &font() const;
        class labels &font(std::string_view font);

        float font_size() const;
        class labels &font_size(float font_size);

      public /* getters and setters bypassing the line_spec */:
      protected:
        class line_spec line_spec_;
        std::vector<double> x_{};           // x position
        std::vector<double> y_{};           // y position
        std::vector<std::string> labels_{}; // 1
        std::vector<double> colors_{};      // colormap
        std::vector<double> sizes_{};       // colormap
        bool rectangles_{false};            // plot rectangles behind the words
        bool absolute_size_{true};          // reduce font size for
        bool visible_{true};
        enum alignment alignment_ { alignment::automatic };
        std::string font_{"Helvetica"};
        float font_size_{10};
        color_array color_{0, 0, 0, 0};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_LABELS_H
