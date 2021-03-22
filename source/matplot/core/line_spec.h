//
// Created by Alan Freitas on 2020-07-06.
//

#ifndef MATPLOTPLUSPLUS_LINE_SPEC_H
#define MATPLOTPLUSPLUS_LINE_SPEC_H

#include <array>
#include <functional>
#include <matplot/util/colors.h>
#include <matplot/util/concepts.h>

namespace matplot {
    class line_spec {
      public:
        enum class line_style {
            none,          // no line
            solid_line,    // "-"
            dashed_line,   // "--"
            dotted_line,   // ":"
            dash_dot_line, // "-."
        };

        enum class marker_style {
            none,      // "" -> gnuplot linetype -1
            plus_sign, // "+" -> gnuplot linetype 1
            circle,    // "o" -> gnuplot linetype 6
            asterisk,  // "*" -> gnuplot linetype 3
            point,     // "." -> gnuplot linetype 7
            cross,     // "x" -> gnuplot linetype 2
            square,    // "s" / "square" -> gnuplot linetype 4 / 5
            diamond,   // "d" / "diamond" -> gnuplot linetype 12 / 13
            upward_pointing_triangle,   // "^" -> gnuplot linetype 8 / 9
            downward_pointing_triangle, // "v" -> gnuplot linetype 10 / 11
            right_pointing_triangle,    // ">" -> gnuplot linetype (doest not
                                        // exist)
            left_pointing_triangle, // "<" -> gnuplot linetype (does not exist)
            pentagram, // "p" / "pentagram" -> gnuplot linetype 14 / 15
            hexagram,  // "h" / "hexagram" -> gnuplot linetype (does not exist)
            custom, // https://stackoverflow.com/questions/16189187/gnuplot-using-custom-point-shapes-with-legend-entry
        };

        enum class style_to_plot {
            // plot line with markers together
            plot_line_and_marker,
            // plot only line
            plot_line_only,
            // plot only markers
            plot_marker_only,
            // plot only marker face
            plot_marker_face_only
        };

      public:
        line_spec();
        explicit line_spec(std::string_view expr);

        template <class T>
        line_spec(Pointer<T> parent, std::string_view expr)
            : line_spec(expr) {
            touch_function_ = [parent]() { parent->touch(); };
        }

      public:
        /// \brief Create string to apply this style in gnuplot
        /// This includes the style for the line and the marker
        /// It's only possible to set the color for one of them
        std::string
        plot_string(style_to_plot sty = style_to_plot::plot_line_and_marker,
                    bool include_style = true);

        /// Get line_spec properties from a string
        void parse_string(std::string_view expr);

        /// \brief True if we can plot line and marker with only one plot
        /// command We can plot them together "with linespoints" if:
        /// - They both are different from none
        /// - They have the same color
        bool can_plot_line_and_marker_together();

        /// \brief True if style includes a line
        bool has_line();

        /// \brief True if style includes a non custom marker
        bool has_non_custom_marker();

        /// \brief True if style includes a marker face
        bool has_marker_face();

        /// \brief True if line, marker, and marker face are the same color
        [[nodiscard]] bool line_and_marker_are_the_same_color() const;

        /// \brief True if marker and marker face are the same color
        [[nodiscard]] bool marker_and_face_are_the_same_color() const;

        /// Touch the parent element
        void touch();

        [[nodiscard]] const std::array<float, 4> &color() const;
        [[nodiscard]] float alpha() const;
        void color(const std::array<float, 3> &color);
        void color(const std::array<float, 4> &color);
        void color(std::initializer_list<float> color);
        void color(std::string_view color);
        void color(enum color marker_color);
        void alpha(float alpha);
        [[nodiscard]] bool user_color() const;
        void user_color(bool user_color);

        [[nodiscard]] line_style line_style() const;
        void line_style(enum line_style line_style);

        [[nodiscard]] float line_width() const;
        void line_width(float line_width);

        [[nodiscard]] enum marker_style marker_style() const;
        void marker_style(enum marker_style marker_style);
        void marker_style(std::string_view marker_style);
        [[nodiscard]] enum marker_style marker() const;
        template <class T> void marker(T marker) { marker_style(marker); }

        [[nodiscard]] const std::string &custom_marker() const;
        void custom_marker(std::string_view custom_marker);

        [[nodiscard]] float marker_size() const;
        void marker_size(float marker_size);

        const std::array<float, 4> &marker_color() const;
        float marker_alpha() const;
        void marker_color(const std::array<float, 3> &color);
        void marker_color(const std::array<float, 4> &color);
        void marker_color(std::initializer_list<float> color);
        void marker_color(std::string_view color);
        void marker_color(enum color marker_color);
        void marker_alpha(float alpha);
        bool marker_user_color() const;
        void marker_user_color(bool user_color);

        const std::array<float, 4> &marker_face_color() const;
        float marker_face_alpha() const;
        void marker_face_color(const std::array<float, 3> &color);
        void marker_face_color(const std::array<float, 4> &color);
        void marker_face_color(std::initializer_list<float> color);
        void marker_face_color(std::string_view color);
        void marker_face_color(enum color marker_face_color);
        void marker_face_alpha(float alpha);
        bool marker_face_user_color() const;
        void marker_face_user_color(bool user_color);

        bool marker_face() const;
        void marker_face(bool marker_face);

      private:
        // use labels for non-custom markers:
        // set label '▶︎' at 2,2
        // set label '◀' at 3,3
        // set label '✶' at 4,4
        // plot cos(x) with linespoints linecolor rgb "#000000" dashtype 3
        // linewidth 3 linetype 4 unset label
      private:
        // Line
        std::array<float, 4> color_{0, 0, 0.447f, 0.741f};
        // Did the user provide a color or is the color just a placeholder
        bool user_color_{false};
        enum line_style line_style_ { line_style::none };
        float line_width_{0.5000};

        // Marker
        enum marker_style marker_style_ { marker_style::none };
        std::array<float, 4> marker_color_{0, 0, 0.447f, 0.741f};
        bool marker_user_color_{false};
        std::string custom_marker_{""};
        float marker_size_{6};

        // Marker face
        std::array<float, 4> marker_face_color_{0, 0.84f, 1., 1.};
        bool marker_face_user_color_{false};
        bool marker_face_ = false;

        // Function to touch the parent element
        std::function<void()> touch_function_;
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_LINE_SPEC_H
