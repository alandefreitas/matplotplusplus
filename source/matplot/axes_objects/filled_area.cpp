//
// Created by Alan Freitas on 09/07/20.
//

#include <cmath>
#include <matplot/axes_objects/filled_area.h>
#include <matplot/core/axes_type.h>
#include <sstream>

namespace matplot {
    filled_area::filled_area(class axes_type *parent) : line(parent) {}

    filled_area::filled_area(class axes_type *parent,
                             const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &base_values,
                             bool stacked, std::string_view line_spec)
        : line(parent, x, y, line_spec), stacked_(stacked),
          base_data_(base_values) {}

    void filled_area::maybe_update_face_color() {
        // if user has not defined the color yet, get it from the xlim
        if (!fill_user_color_) {
            // get it from the xlim
            // the line spec is not altered from black
            auto c = parent_->get_color_and_bump();
            face_color_ = c;
            // if not stacked
            if (!stacked_) {
                // visualization is better with some alpha
                face_color_[0] *= 0.5;
            }
            fill_user_color_ = true;
        }
    }

    std::string filled_area::plot_string() {
        // we get the fill color and not the line spec from the xlim
        maybe_update_face_color();

        // plot fill curve
        std::string res = "'-' with filledcurve linecolor rgb \"" +
                          to_string(face_color_) + "\"";

        // plot base line
        if (plot_base_line_ && line_spec_.has_line()) {
            // base line has no markers
            auto previous_marker = line_spec_.marker();
            line_spec_.marker(line_spec::marker_style::none);
            res += ", " + line::plot_string();
            line_spec_.marker(previous_marker);
        }

        // plot line as usual
        res += ", " + line::plot_string();
        return res;
    }

    std::string filled_area::data_string() {
        std::stringstream ss;

        std::vector<double> stacked_data;
        if (!stacked_) {
            // send data for filled curve
            for (size_t i = 0; i < y_data_.size(); ++i) {
                if (std::isfinite(y_data_[i])) {
                    double base_value = base_data_.empty() ? 0.0
                                        : base_data_.size() == 1
                                            ? base_data_[0]
                                            : base_data_[i];
                    ss << "    " << x_data_[i] << " " << base_value << " "
                       << y_data_[i] << "\n";
                } else {
                    ss << "    \n";
                }
            }
            ss << "    e\n";
        } else {
            // If the area is stacked, the data for filled curve needs
            // to consider previous area plots in the xlim. Also,
            // the sooner it comes in the xlim, the more in the background.
            // So we stack the y_values with all y_values that come *after*
            // the area in the xlim
            stacked_data = y_data_;
            for (auto children_it = parent_->children().rbegin();
                 children_it != parent_->children().rend(); ++children_it) {
                const auto &child = *children_it;
                auto ptr = dynamic_cast<filled_area *>(child.get());
                if (ptr != nullptr) {
                    if (ptr != this) {
                        size_t n =
                            std::min(stacked_data.size(), ptr->y_data_.size());
                        for (size_t i = 0; i < n; ++i) {
                            stacked_data[i] += ptr->y_data_[i];
                        }
                    } else {
                        break;
                    }
                }
            }
            // send data
            for (size_t i = 0; i < stacked_data.size(); ++i) {
                double base_value = base_data_.empty()       ? 0.0
                                    : base_data_.size() == 1 ? base_data_[0]
                                                             : base_data_[i];
                ss << "    " << x_data_[i] << " " << base_value << " "
                   << stacked_data[i] << "\n";
            }
            ss << "    e\n";
        }

        // send data for base line
        for (size_t i = 0; i < y_data_.size(); ++i) {
            double base_value = base_data_.empty()       ? 0.0
                                : base_data_.size() == 1 ? base_data_[0]
                                                         : base_data_[i];
            if (std::isfinite(base_value) && std::isfinite(x_data_[i])) {
                ss << "    " << x_data_[i] << " " << base_value << "\n";
            } else {
                ss << "    \n";
            }
        }
        ss << "    e\n";

        // send data for foreground line as usual
        if (!stacked_) {
            ss << line::data_string();
        } else {
            std::swap(y_data_, stacked_data);
            ss << line::data_string();
            std::swap(y_data_, stacked_data);
        }
        return ss.str();
    }

    enum axes_object::axes_category filled_area::axes_category() {
        // filled area is always 2d
        return axes_object::axes_category::two_dimensional;
    }

    bool filled_area::stacked() const { return stacked_; }

    class filled_area &filled_area::stacked(bool stacked) {
        stacked_ = stacked;
        return *this;
    }

    const std::vector<double> &filled_area::base_data() const {
        return base_data_;
    }

    class filled_area &
    filled_area::base_data(const std::vector<double> &base_data) {
        base_data_ = base_data;
        return *this;
    }

    bool filled_area::plot_base_line() const { return plot_base_line_; }

    class filled_area &filled_area::plot_base_line(bool plot_base_line) {
        plot_base_line_ = plot_base_line;
        return *this;
    }

    const color_array &filled_area::face_color() const { return face_color_; }

    class filled_area &filled_area::face_color(const color_array &face_color) {
        face_color_ = face_color;
        fill_user_color_ = true;
        return *this;
    }

    bool filled_area::fill_user_color() const { return fill_user_color_; }

    class filled_area &filled_area::fill_user_color(bool fill_user_color) {
        fill_user_color_ = fill_user_color;
        return *this;
    }

} // namespace matplot