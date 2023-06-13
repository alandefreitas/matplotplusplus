//
// Created by Alan Freitas on 09/07/20.
//

#include <matplot/axes_objects/error_bar.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {
    error_bar::error_bar(class axes_type *parent) : line(parent) {}

    error_bar::error_bar(class axes_type *parent, const std::vector<double> &x,
                         const std::vector<double> &y,
                         const std::vector<double> &y_neg_delta,
                         const std::vector<double> &y_pos_delta,
                         const std::vector<double> &x_neg_delta,
                         const std::vector<double> &x_pos_delta,
                         std::string_view line_spec)
        : line(parent, x, y, line_spec), x_negative_delta_(x_neg_delta),
          x_positive_delta_(x_pos_delta), y_negative_delta_(y_neg_delta),
          y_positive_delta_(y_pos_delta) {}

    error_bar::error_bar(class axes_type *parent, const std::vector<double> &x,
                         const std::vector<double> &y,
                         const std::vector<double> &error, error_bar::type type,
                         std::string_view line_spec)
        : line(parent, x, y, line_spec),
          x_negative_delta_(type != error_bar::type::vertical
                                ? error
                                : std::vector<double>({})),
          x_positive_delta_(type != error_bar::type::vertical
                                ? error
                                : std::vector<double>({})),
          y_negative_delta_(type != error_bar::type::horizontal
                                ? error
                                : std::vector<double>({})),
          y_positive_delta_(type != error_bar::type::horizontal
                                ? error
                                : std::vector<double>({})) {}

    std::string error_bar::plot_string() {
        // plot errorbar behind the line
        std::string res;
        const bool has_y_bar = !y_negative_delta_.empty();
        const bool has_x_bar = !x_negative_delta_.empty();
        const bool has_xy_bar = has_y_bar && has_x_bar;
        if (has_xy_bar) {
            res += "'-' with xyerrorbars linecolor rgb \"" +
                   to_string(line_spec_.color()) + "\" pointsize 0";
        } else if (has_y_bar) {
            // filled curve error bar is only relevant in yerrorbars
            if (!filled_curve_) {
                res += "'-' with yerrorbars linecolor rgb \"" +
                       to_string(line_spec_.color()) + "\" pointsize 0";
            } else {
                color_array c = line_spec_.color();
                c[0] = filled_curve_alpha_;
                res += "'-' with filledcurve linecolor rgb \"" + to_string(c) +
                       "\"";
            }
        } else if (has_x_bar) {
            res += "'-' with xerrorbars linecolor rgb \"" +
                   to_string(line_spec_.color()) + "\" pointsize 0";
        }
        // plot line over the errorbar
        res += ", " + line::plot_string();
        return res;
    }

    enum axes_object::axes_category error_bar::axes_category() {
        // errorbar is always 2d
        return axes_object::axes_category::two_dimensional;
    }

    std::string error_bar::data_string() {
        // send data once more for the bar
        const bool has_y_bar = !y_negative_delta_.empty();
        const bool has_x_bar = !x_negative_delta_.empty();
        const bool has_xy_bar = has_y_bar && has_x_bar;
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        if (has_xy_bar) {
            for (size_t i = 0; i < y_data_.size(); ++i) {
                ss << "    " << x_data_[i] << " " << y_data_[i] << " "
                   << x_data_[i] - x_negative_delta_[i] << " "
                   << x_data_[i] + x_positive_delta_[i] << " "
                   << y_data_[i] - y_negative_delta_[i] << " "
                   << y_data_[i] + y_positive_delta_[i] << "\n";
            }
            ss << "    e\n";
        } else if (has_x_bar) {
            for (size_t i = 0; i < y_data_.size(); ++i) {
                ss << "    " << x_data_[i] << " " << y_data_[i] << " "
                   << x_data_[i] - x_negative_delta_[i] << " "
                   << x_data_[i] + x_positive_delta_[i] << "\n";
            }
            ss << "    e\n";
        } else if (has_y_bar) {
            if (!filled_curve_) {
                for (size_t i = 0; i < y_data_.size(); ++i) {
                    ss << "    " << x_data_[i] << " " << y_data_[i] << " "
                       << y_data_[i] - y_negative_delta_[i] << " "
                       << y_data_[i] + y_positive_delta_[i] << "\n";
                }
                ss << "    e\n";
            } else {
                for (size_t i = 0; i < y_data_.size(); ++i) {
                    ss << "    " << x_data_[i] << " "
                       << y_data_[i] + y_positive_delta_[i] << " "
                       << y_data_[i] - y_negative_delta_[i] << "\n";
                }
                ss << "    e\n";
            }
        }
        // send data for foreground line as usual
        ss << line::data_string();
        return ss.str();
    }

    bool error_bar::requires_colormap() { return line::requires_colormap(); }

    std::string error_bar::set_variables_string() {
        //     set bars {small | large | fullwidth | <size>}
        //     unset bars
        //     show bars
        std::string res = line::set_variables_string();
        if (cap_size_ / 3. != 1.) {
            res += "set bars " + num2str(cap_size_ / 3.) + "\n";
        }
        return res;
    }

    std::string error_bar::unset_variables_string() {
        std::string res = line::unset_variables_string();
        if (cap_size_ / 3. != 1.) {
            res += "unset bars\n";
        }
        return res;
    }

    bool error_bar::filled_curve() const { return filled_curve_; }

    class error_bar &error_bar::filled_curve(bool filled_curve) {
        filled_curve_ = filled_curve;
        touch();
        return *this;
    }

    const std::vector<double> &error_bar::x_negative_delta() const {
        return x_negative_delta_;
    }

    class error_bar &
    error_bar::x_negative_delta(const std::vector<double> &x_negative_delta) {
        x_negative_delta_ = x_negative_delta;
        touch();
        return *this;
    }

    const std::vector<double> &error_bar::x_positive_delta() const {
        return x_positive_delta_;
    }

    class error_bar &
    error_bar::x_positive_delta(const std::vector<double> &x_positive_delta) {
        x_positive_delta_ = x_positive_delta;
        touch();
        return *this;
    }

    const std::vector<double> &error_bar::y_negative_delta() const {
        return y_negative_delta_;
    }

    class error_bar &
    error_bar::y_negative_delta(const std::vector<double> &y_negative_delta) {
        y_negative_delta_ = y_negative_delta;
        touch();
        return *this;
    }

    const std::vector<double> &error_bar::y_positive_delta() const {
        return y_positive_delta_;
    }

    class error_bar &
    error_bar::y_positive_delta(const std::vector<double> &y_positive_delta) {
        y_positive_delta_ = y_positive_delta;
        touch();
        return *this;
    }

    float error_bar::filled_curve_alpha() const { return filled_curve_alpha_; }

    class error_bar &error_bar::filled_curve_alpha(float filled_curve_alpha) {
        filled_curve_alpha_ = filled_curve_alpha;
        touch();
        return *this;
    }

    float error_bar::cap_size() const { return cap_size_; }

    class error_bar &error_bar::cap_size(float cap_size) {
        cap_size_ = cap_size;
        touch();
        return *this;
    }

} // namespace matplot