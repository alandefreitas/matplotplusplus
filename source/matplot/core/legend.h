//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_LEGEND_H
#define MATPLOTPLUSPLUS_LEGEND_H

#include <array>
#include <matplot/core/line_spec.h>
#include <matplot/util/handle_types.h>
#include <string>
#include <vector>

namespace matplot {
    class axes_type;

    class legend {
      public:
        enum class horizontal_alignment { left, center, right };

        enum class vertical_alignment { top, center, bottom };

        enum class general_alignment {
            topleft,
            top,
            topright,
            left,
            center,
            right,
            bottomleft,
            bottom,
            bottomright,
        };

      public:
        legend() = default;
        legend(class axes_type *parent);
        legend(class axes_type *parent,
               std::initializer_list<std::string> names);
        legend(class axes_type *parent, const std::vector<std::string> &names);

      public /* useful functions */:
        void touch();
        const std::string &operator[](size_t index) const;
        std::string &operator[](size_t index);
        bool empty() const;
        size_t size() const;
        std::vector<std::string>::const_iterator begin() const;
        std::vector<std::string>::iterator begin();
        std::vector<std::string>::const_iterator end() const;
        std::vector<std::string>::iterator end();

      public /* getter and setters */:
        std::vector<std::string> &strings();
        const std::vector<std::string> &strings() const;
        void strings(const std::vector<std::string> &strings);

        void inside(bool inside);
        bool inside() const;

        bool label_after_sample() const;
        void label_after_sample(bool label_after_sample);

        bool box() const;
        void box(bool box);

        const line_spec &box_line() const;
        line_spec &box_line();
        void box_line(const line_spec &box_line);

        bool vertical() const;
        void vertical(bool vertical);

        bool horizontal() const;
        void horizontal(bool horizontal);

        // Legend location given a reference position
        // If the position is automatic, the reference is the xlim
        // If the position is a manual point, the reference is this point
        general_alignment location() const;
        void location(general_alignment alignment);

        horizontal_alignment horizontal_location() const;
        void horizontal_location(horizontal_alignment horizontal_location);

        vertical_alignment vertical_location() const;
        void vertical_location(vertical_alignment vertical_location);

        bool manual_position() const;
        void manual_position(bool manual_position);

        const std::array<float, 2> &position() const;
        void position(const std::array<float, 2> &position);

        bool invert() const;
        void invert(bool invert);

        size_t num_columns() const;
        void num_columns(size_t num_columns);

        size_t num_rows() const;
        void num_rows(size_t num_rows);

        bool visible() const;
        void visible(bool visible);

        const std::string &title() const;
        void title(std::string_view title);

        const std::string &font_name() const;
        void font_name(std::string_view font_name);

        float font_size() const;
        void font_size(float font_size);

        const std::string &font_angle() const;
        void font_angle(std::string_view font_angle);

        const std::string &font_weight() const;
        void font_weight(std::string_view font_weight);

        const color_array &text_color() const;
        void text_color(const color_array &text_color);
        template <class T> void text_color(T c) { text_color(to_array(c)); }

      private:
        // The keys
        std::vector<std::string> strings_{};
        std::string title_{""};

        // Positioning
        bool inside_{true};
        bool manual_position_{false};
        std::array<float, 2> position_{0.0, 0.0};
        horizontal_alignment horizontal_location_{horizontal_alignment::right};
        vertical_alignment vertical_location_{vertical_alignment::top};

        // Font
        std::string font_name_{"Helvetica"};
        float font_size_{11};
        std::string font_angle_{"normal"};
        std::string font_weight_{"bold"};
        color_array text_color_{0., 0, 0, 0};

        // Style
        bool box_{true};
        line_spec box_line_{"k-"};
        // color_array color_{0., 1, 1, 1};
        bool vertical_{true};
        bool label_after_sample_{true};
        bool invert_{false};
        bool visible_{true};

        size_t num_columns_{0};
        size_t num_rows_{0};

        // Parent xlim
        class axes_type *parent_{nullptr};
    };
} // namespace matplot

#endif // MATPLOTPLUSPLUS_LEGEND_H
