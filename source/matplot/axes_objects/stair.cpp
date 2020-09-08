//
// Created by Alan Freitas on 2020-07-07.
//

#include <algorithm>
#include <matplot/axes_objects/stair.h>
#include <matplot/core/axes_type.h>

namespace matplot {
    stair::stair(class axes_type *parent) : line(parent) {}

    stair::stair(class axes_type *parent, const std::vector<double> &y_data,
                 std::string_view line_spec)
        : line(parent, y_data, line_spec) {}

    stair::stair(class axes_type *parent, const std::vector<double> &x_data,
                 const std::vector<double> &y_data,
                 std::string_view line_spec)
        : line(parent, x_data, y_data, line_spec) {}

    std::vector<line_spec::style_to_plot> stair::styles_to_plot() {
        // get styles we would plot if this were a normal line
        std::vector<line_spec::style_to_plot> result = line::styles_to_plot();

        // unlike in line, we cannot plot line and marker together
        // replace plot_line_and_marker with line and marker, if not there
        auto it = std::find(result.begin(), result.end(),
                            line_spec::style_to_plot::plot_line_and_marker);
        if (it != result.end()) {
            result.erase(it);
            result.emplace_back(line_spec::style_to_plot::plot_line_only);
            result.emplace_back(line_spec::style_to_plot::plot_marker_only);
            // we need to sort it to make sure line comes before marker and
            // marker before marker face
            std::sort(result.begin(), result.end());
        }
        return result;
    }

    std::string stair::plot_string() {
        line::maybe_update_line_spec();
        std::string res;
        bool first = true;
        for (const auto &style : styles_to_plot()) {
            if (!first) {
                res += ",";
            }
            if (style == line_spec::style_to_plot::plot_line_only) {
                // replace lines with steps, according to style
                switch (stair_style_) {
                case stair_style::trace_x_first:
                    res += " '-' with steps " +
                           line_spec_.plot_string(style, false);
                    break;
                case stair_style::trace_y_first:
                    res += " '-' with fsteps " +
                           line_spec_.plot_string(style, false);
                    break;
                case stair_style::histogram:
                    res += " '-' with histeps " +
                           line_spec_.plot_string(style, false);
                    break;
                case stair_style::fill:
                    res += " '-' with fillsteps fillstyle solid 0.25 fillcolor "
                           "\"" +
                           to_string(line_spec_.color()) + "\"";
                    break;
                }
            } else {
                // plot markers the same way
                res += " '-' " + line_spec_.plot_string(style);
            }
            if (first) {
                first = false;
            }
        }
        return res;
    }

    enum axes_object::axes_category stair::axes_category() {
        // always 2d
        return axes_object::axes_category::two_dimensional;
    }

    enum stair::stair_style stair::stair_style() const { return stair_style_; }

    class stair &stair::stair_style(enum stair::stair_style stair_style) {
        stair_style_ = stair_style;
        touch();
        return *this;
    }

    bool stair::fill() const { return fill_; }

    class stair &stair::fill(bool fill) {
        fill_ = fill;
        touch();
        return *this;
    }

} // namespace matplot