//
// Created by Alan Freitas on 15/07/20.
//

#include <iomanip>
#include <matplot/axes_objects/parallel_lines.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <regex>
#include <sstream>

namespace matplot {
    parallel_lines::parallel_lines(class axes_type *parent)
        : axes_object(parent) {}

    parallel_lines::parallel_lines(class axes_type *parent,
                                   const std::vector<std::vector<double>> &data,
                                   std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), data_(data) {
        for (size_t i = 0; i < data.size(); ++i) {
            axis_.emplace_back(parent_, true);
        }
    }

    std::string parallel_lines::set_variables_string() {
        // borders are not useful
        std::string res = "set border 0\n";
        // replace the previous background rectangle
        if (parent_->color() != parent_->parent()->color()) {
            res += "    unset object 2\n";
            // auto [min_x1, max_x1] = std::minmax_element(data_[0].begin(),
            // data_[0].end()); double r = *max_x1 - *min_x1;
            res += "    set object 2 rectangle from 1,0 to " +
                   num2str(data_.size()) + ",1 behind fillcolor rgb \"" +
                   to_string(parent_->color()) +
                   "\" fillstyle solid 1.0 noborder\n";
        }

        // allow x-axis go to as far as needed
        // res += "    set xrange [] noextend\n";
        // there can be no y-axis in a parallel plot
        // res += "    unset ytics\n";
        // use x-axis tic positions to label the xlim
        const bool main_x_axis_uses_default_format =
            parent_->x_axis().tick_label_format() == "%g" &&
            parent_->x_axis().tick_values_automatic();
        if (main_x_axis_uses_default_format) {
            res += "    set xtics 1 format \"x%g\" scale 0,0\n";
        }
        // turn on axis tics for the parallel xlim (not anymore because we were
        // forced to use line plots now)
        //        for (size_t i = 0; i < axis_.size(); ++i) {
        //            auto& ax = axis_[i];
        //            std::string axis_name = "paxis " + num2str(i+1) + " ";
        //            if (ax.visible() && !ax.tick_values().empty()) {
        //                res += "    set " + axis_name + "tics textcolor \"" +
        //                to_string(ax.color()) + "\" font \"" + parent_->font()
        //                + "," + num2str(unsigned(parent_->font_size())) + "\"
        //                format \"" + escape(ax.tick_label_format()) + "\"
        //                nomirror in scale 0.75 \n"; if
        //                (ax.tick_values_manual()) {
        //                    res += "    set " + axis_name + "tics " +
        //                    ax.tick_values_string() + ax.tick_rotate_string()
        //                    + "\n";
        //                }
        //            } else {
        //                res += "    unset " + axis_name + "tics\n";
        //            }
        //        }
        return res;
    }

    std::string parallel_lines::plot_string() {
        maybe_update_line_spec();
        /*
         * The ideal would be to use "with parallel",
         * but it just does not work from one version
         * to the other. Every time gnuplot updates,
         * our code with "with parallel" stopped working.
         * Thus, we will need to create lines for each
         * point as if this was a line plot. The commands
         * for line plots should be more stable.
         */

        // The parallel lines representing points
        const bool color_is_variable = !line_colors_.empty();
        std::string res =
            " '-' " + line_spec_.plot_string(
                          line_spec::style_to_plot::plot_line_only, true);
        if (color_is_variable) {
            res = std::regex_replace(res, std::regex(" linecolor rgb +[^ ]+ "),
                                     " linecolor palette ");
        }

        // The fake axes with their fake ticks
        res += ", '-' with lines linecolor 'black'";

        // Labels for the fake ticks
        // Have a look at:
        // https://www.mathworks.com/help/matlab/ref/parallelplot.html
        res += ", '-' with labels right";

        // Labels for last dimension
        res += ", '-' with labels left";

        return res;
    }

    void parallel_lines::maybe_update_line_spec() {
        // parallel coordinates work with lines only
        if (!line_spec_.has_line()) {
            line_spec_.line_style(line_spec::line_style::solid_line);
        }
        if (!line_colors_.empty()) {
            // already have colors from somewhere else
            return;
        }
        if (!line_spec_.user_color()) {
            auto c = parent_->get_color_and_bump();
            line_spec_.color(c);
        }
    }

    std::string parallel_lines::legend_string(std::string_view title) {
        return " keyentry " +
               line_spec_.plot_string(
                   line_spec::style_to_plot::plot_line_only) +
               " title \"" + escape(title) + "\"";
    }

    std::string parallel_lines::data_string() {
        // calculate data for jitter
        std::vector<double> X_min;
        std::vector<double> X_max;
        std::vector<double> X_range;
        constexpr double expand_factor = 0.03;
        std::vector<double> expanded_X_min;
        std::vector<double> expanded_X_max;
        std::vector<double> expanded_X_range;
        for (size_t i = 0; i < data_.size(); ++i) {
            auto [min_it, max_it] =
                std::minmax_element(data_[i].begin(), data_[i].end());
            X_min.emplace_back(*min_it);
            X_max.emplace_back(*max_it);
            X_range.emplace_back(X_max.back() - X_min.back());
            expanded_X_min.emplace_back(*min_it -
                                        expand_factor * X_range.back());
            expanded_X_max.emplace_back(*max_it +
                                        expand_factor * X_range.back());
            expanded_X_range.emplace_back(expanded_X_max.back() -
                                          expanded_X_min.back());
        }

        auto jitter = [&](double y, size_t dimension) {
            double y2 =
                y + randn(0, (X_max[dimension] - X_min[dimension]) / 100);
            // reflect
            if (y2 < X_min[dimension]) {
                y2 += 2 * (X_min[dimension] - y2);
            } else if (y2 > X_max[dimension]) {
                y2 -= 2 * (y2 - X_max[dimension]);
            }
            return y2;
        };

        auto normalize = [&](double y, size_t dimension) {
            return (y - expanded_X_min[dimension]) /
                   expanded_X_range[dimension];
        };

        const bool color_is_variable = !line_colors_.empty();
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        // for each point
        for (size_t i = 0; i < data_[0].size(); ++i) {
            // for each dimension
            for (size_t dimension = 0; dimension < data_.size(); ++dimension) {
                ss << "    " << dimension + 1 << "  ";
                if (!jitter_) {
                    ss << normalize(data_[dimension][i], dimension) << "  ";
                } else {
                    ss << normalize(jitter(data_[dimension][i], dimension),
                                    dimension)
                       << "  ";
                }
                if (color_is_variable) {
                    ss << line_colors_[i] << "  ";
                }
                // new line
                ss << "\n";
            }
            ss << "\n";
        }
        ss << "e\n";

        // Calculate tics for the multidimensional axes
        std::vector<ticks_results> ticks(data_.size());
        for (size_t i = 0; i < ticks.size(); ++i) {
            if (axis_[i].tick_values_manual()) {
                ticks[i].ticks = axis_[i].tick_values();
                ticks[i].tickLabels = axis_[i].ticklabels();
                for (size_t j = 0; j < ticks[i].ticks.size(); ++j) {
                    if (ticks[i].tickLabels.size() <= j) {
                        ticks[i].tickLabels.emplace_back(num2str(
                            ticks[i].ticks[j], axis_[i].tick_label_format()));
                    }
                }
            } else {
                ticks[i] = calcticks(X_min[i], X_max[i], false,
                                     parent_->font_size() * 0.125);
            }
        }

        // The fake axes with fake ticks
        for (size_t dimension = 0; dimension < data_.size(); ++dimension) {
            ss << "    " << dimension + 1 << "  " << 0 << "\n";
            ss << "    " << dimension + 1 << "  " << 1 << "\n\n";

            const size_t n_ticks = ticks[dimension].ticks.size();
            constexpr double tick_size = 0.03;
            for (size_t i = 0; i < n_ticks; ++i) {
                double y = normalize(ticks[dimension].ticks[i], dimension);
                ss << "    " << dimension + 1 << "  " << y << "\n";
                const bool is_last_dimension = dimension == data_.size() - 1;
                ss << "    "
                   << dimension + 1 +
                          (!is_last_dimension ? -tick_size : +tick_size)
                   << "  " << y << "\n\n";
            }
        }
        ss << "e\n";

        // Put labels on the fake tics
        for (size_t dimension = 0; dimension < data_.size(); ++dimension) {
            const size_t n_ticks = ticks[dimension].ticks.size();
            constexpr double tick_size = 0.03;
            for (size_t i = 0; i < n_ticks; ++i) {
                double tic_value = ticks[dimension].ticks[i];
                double y = normalize(tic_value, dimension);
                const bool is_last_dimension = dimension == data_.size() - 1;
                ss << "    "
                   << dimension + 1 +
                          (!is_last_dimension ? -tick_size - 0.01
                                              : +tick_size + 0.01)
                   << "  " << y << "  "
                   << std::quoted(ticks[dimension].tickLabels[i]) << "\n\n";
            }
            if (dimension == data_.size() - 2) {
                ss << "e\n";
            }
        }
        ss << "e\n";

        return ss.str();
    }

    std::string parallel_lines::unset_variables_string() { return ""; }

    enum axes_object::axes_category parallel_lines::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    const line_spec &parallel_lines::line_spec() const { return line_spec_; }

    class parallel_lines &
    parallel_lines::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const std::vector<std::vector<double>> &parallel_lines::data() const {
        return data_;
    }

    class parallel_lines &
    parallel_lines::data(const std::vector<std::vector<double>> &data) {
        data_ = data;
        touch();
        return *this;
    }

    const std::vector<class axis_type> &parallel_lines::axis() const {
        return axis_;
    }

    std::vector<class axis_type> &parallel_lines::axis() { return axis_; }

    class parallel_lines &
    parallel_lines::axis(const std::vector<class axis_type> &axis) {
        axis_ = axis;
        touch();
        return *this;
    }

    bool parallel_lines::visible() const { return visible_; }

    class parallel_lines &parallel_lines::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    bool parallel_lines::jitter() const { return jitter_; }

    class parallel_lines &parallel_lines::jitter(bool jitter) {
        jitter_ = jitter;
        touch();
        return *this;
    }

    const std::vector<float> &parallel_lines::line_widths() const {
        return line_widths_;
    }

    class parallel_lines &
    parallel_lines::line_widths(const std::vector<float> &line_widths) {
        line_widths_ = line_widths;
        return *this;
    }

    const std::vector<double> &parallel_lines::line_colors() const {
        return line_colors_;
    }

    class parallel_lines &
    parallel_lines::line_colors(const std::vector<double> &line_colors) {
        line_colors_ = line_colors;
        return *this;
    }

} // namespace matplot