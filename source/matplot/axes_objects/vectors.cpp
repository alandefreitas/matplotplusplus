//
// Created by Alan Freitas on 2020-07-07.
//

#include <cmath>
#include <matplot/axes_objects/vectors.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <regex>
#include <sstream>

namespace matplot {
    vectors::vectors(class axes_type *parent) : axes_object(parent) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &v_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), v_data_(v_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), u_data_(u_data),
          v_data_(v_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     const std::vector<double> &w_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), u_data_(u_data),
          v_data_(v_data), w_data_(w_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &x_data,
                     const std::vector<double> &y_data,
                     const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data), u_data_(u_data), v_data_(v_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &x_data,
                     const std::vector<double> &y_data,
                     const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     const std::vector<double> &c_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data), u_data_(u_data), v_data_(v_data), c_data_(c_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &x_data,
                     const std::vector<double> &y_data,
                     const std::vector<double> &z_data,
                     const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     const std::vector<double> &w_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data), z_data_(z_data), u_data_(u_data), v_data_(v_data),
          w_data_(w_data) {}

    vectors::vectors(class axes_type *parent, const std::vector<double> &x_data,
                     const std::vector<double> &y_data,
                     const std::vector<double> &z_data,
                     const std::vector<double> &u_data,
                     const std::vector<double> &v_data,
                     const std::vector<double> &w_data,
                     const std::vector<double> &c_data,
                     std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data), z_data_(z_data), u_data_(u_data), v_data_(v_data),
          w_data_(w_data), c_data_(c_data) {}

    std::string vectors::plot_string() {
        maybe_update_line_spec();
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        ss << " '-' with vectors";
        if (!c_data_.empty()) {
            ss << " linecolor palette";
            ss << " linewidth " << line_spec_.line_width();

            switch (line_spec_.line_style()) {
            case line_spec::line_style::solid_line:
                ss << " dashtype 1";
                break;
            case line_spec::line_style::dashed_line:
                ss << " dashtype '--'";
                break;
            case line_spec::line_style::dotted_line:
                ss << " dashtype '.'";
                break;
            case line_spec::line_style::dash_dot_line:
                ss << " dashtype '-.'";
                break;
            default:
                break;
            }
        } else {
            ss << line_spec_.plot_string(
                line_spec::style_to_plot::plot_line_only, false);
        }

        if (use_y2_) {
            ss << " xlim x1y2";
        }
        return ss.str();
    }

    std::string vectors::legend_string(std::string_view title) {
        return " keyentry with vectors " +
               line_spec_.plot_string(line_spec::style_to_plot::plot_line_only,
                                      false) +
               " title \"" + escape(title) + "\"";
    }

    std::string vectors::data_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        if (visible_) {
            for (size_t i = 0; i < v_data_.size(); ++i) {
                double x_value = x_data_.size() > i ? x_data_[i]
                                 : is_polar()       ? 0
                                              : static_cast<double>(i + 1);
                double y_value = y_data_.size() > i ? y_data_[i] : 0;
                double z_value = z_data_.size() > i ? z_data_[i] : 0;
                double u_value = u_data_.size() > i ? u_data_[i] : 0;
                double v_value = v_data_.size() > i ? v_data_[i] : 0;
                double w_value = w_data_.size() > i ? w_data_[i] : 0;
                double c_value = c_data_.size() > i ? c_data_[i] : 0;

                bool is_end_of_series =
                    !std::isfinite(x_value) || !std::isfinite(y_value) ||
                    !std::isfinite(z_value) || !std::isfinite(u_value) ||
                    !std::isfinite(v_value) || !std::isfinite(w_value);
                if (is_end_of_series) {
                    ss << "    \n";
                    continue;
                }

                ss << "    " << x_value;
                ss << "  " << y_value;
                if (is_3d()) {
                    ss << "  " << z_value;
                } else if (parent_->is_3d()) {
                    ss << "  " << 0.0;
                }

                if (normalize_) {
                    double mag =
                        std::sqrt(u_value * u_value + v_value * v_value +
                                  w_value * w_value);
                    if (mag != 0.0) {
                        u_value /= mag;
                        v_value /= mag;
                        w_value /= mag;
                    }
                    if (scale_ != 0.0) {
                        u_value *= scale_;
                        v_value *= scale_;
                        w_value *= scale_;
                    }
                }

                ss << "  " << u_value;
                ss << "  " << v_value;
                if (is_3d()) {
                    ss << "  " << w_value;
                } else if (parent_->is_3d()) {
                    ss << "  " << 0.0;
                }
                if (!c_data_.empty()) {
                    ss << " " << c_value;
                }

                ss << "\n";
            }
            // if polar, gnuplot uses the vector origin to determine
            // the automatic range of the r-axis.
            // If they all have origin 0,0 as often common, there is
            // no r-axis.
            // So we create an extra vector of magnitude zero from
            // the polar max to make the r-range adapt.
            if (is_polar()) {
                ss << "    0  " << xmax() << "  0  0\n";
            }
        }
        ss << "    "
           << "e\n";
        return ss.str();
    }

    void vectors::maybe_update_line_spec() {
        if (!line_spec_.user_color() && c_data_.empty()) {
            auto c = parent_->get_color_and_bump();
            line_spec_.color(c);
        } else if (line_spec_.has_non_custom_marker() &&
                   !line_spec_.user_color() &&
                   !line_spec_.marker_user_color()) {
            auto c = parent_->get_color_and_bump();
            line_spec_.marker_color(c);
        }
    }

    double vectors::xmax() {
        if (!is_polar()) {
            if (x_data_.empty()) {
                if (!y_data_.empty()) {
                    return static_cast<double>(y_data_.size() - 1);
                } else {
                    return axes_object::xmax();
                }
            }
            return *std::max_element(x_data_.begin(), x_data_.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return +(parent_->r_axis().limits()[1] -
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] =
                std::minmax_element(y_data_.begin(), y_data_.end());
            if (max_rho != y_data_.end() && min_rho != y_data_.end()) {
                return +round_polar_max(std::abs(*max_rho));
            } else {
                return 1;
            }
        }
    }

    double vectors::xmin() {
        if (!is_polar()) {
            if (x_data_.empty()) {
                if (!y_data_.empty()) {
                    return 0;
                } else {
                    return axes_object::xmin();
                }
            }
            return *std::min_element(x_data_.begin(), x_data_.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return -(parent_->r_axis().limits()[1] -
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] =
                std::minmax_element(y_data_.begin(), y_data_.end());
            if (max_rho != y_data_.end() && min_rho != y_data_.end()) {
                return -round_polar_max(std::abs(*max_rho));
            } else {
                return -1;
            }
        }
    }

    double vectors::ymax() {
        if (!is_polar()) {
            if (y_data_.empty()) {
                return axes_object::ymax();
            }
            return *std::max_element(y_data_.begin(), y_data_.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return +(parent_->r_axis().limits()[1] -
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] =
                std::minmax_element(y_data_.begin(), y_data_.end());
            if (max_rho != y_data_.end() && min_rho != y_data_.end()) {
                return +round_polar_max(std::abs(*max_rho));
            } else {
                return 1;
            }
        }
    }

    double vectors::ymin() {
        if (!is_polar()) {
            if (y_data_.empty()) {
                return axes_object::ymin();
            }
            return *std::min_element(y_data_.begin(), y_data_.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return -(parent_->r_axis().limits()[1] -
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] =
                std::minmax_element(y_data_.begin(), y_data_.end());
            if (max_rho != y_data_.end() && min_rho != y_data_.end()) {
                return -round_polar_max(std::abs(*max_rho));
            } else {
                return -1;
            }
        }
    }

    enum axes_object::axes_category vectors::axes_category() {
        if (z_data_.empty()) {
            if (!polar_) {
                return axes_object::axes_category::two_dimensional;
            } else {
                return axes_object::axes_category::polar;
            }
        } else {
            return axes_object::axes_category::three_dimensional;
        }
    }

    class vectors &vectors::line_style(std::string_view str) {
        line_spec_.parse_string(str);
        touch();
        return *this;
    }

    const line_spec &vectors::line_spec() const { return line_spec_; }

    line_spec &vectors::line_spec() { return line_spec_; }

    class vectors &vectors::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const std::vector<double> &vectors::y_data() const { return y_data_; }

    class vectors &vectors::y_data(const std::vector<double> &y_data) {
        y_data_ = y_data;
        touch();
        return *this;
    }

    const std::vector<double> &vectors::x_data() const { return x_data_; }

    class vectors &vectors::x_data(const std::vector<double> &x_data) {
        x_data_ = x_data;
        touch();
        return *this;
    }

    const std::vector<double> &vectors::z_data() const { return z_data_; }

    class vectors &vectors::z_data(const std::vector<double> &z_data) {
        z_data_ = z_data;
        touch();
        return *this;
    }

    const std::vector<size_t> &vectors::marker_indices() const {
        return marker_indices_;
    }

    class vectors &
    vectors::marker_indices(const std::vector<size_t> &marker_indices) {
        marker_indices_ = marker_indices;
        touch();
        return *this;
    }

    bool vectors::visible() const { return visible_; }

    class vectors &vectors::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float vectors::line_width() const { return line_spec().line_width(); }

    class vectors &vectors::line_width(float line_width) {
        line_spec().line_width(line_width);
        return *this;
    }

    enum line_spec::marker_style vectors::marker_style() const {
        return line_spec().marker();
    }

    enum line_spec::marker_style vectors::marker() const {
        return line_spec().marker();
    }

    float vectors::marker_size() const { return line_spec().marker_size(); }

    class vectors &vectors::marker_size(float size) {
        line_spec().marker_size(size);
        return *this;
    }

    class vectors &vectors::marker_size(const std::vector<float> &size_vector) {
        marker_sizes_ = size_vector;
        touch();
        return *this;
    }

    class vectors &
    vectors::marker_size(const std::vector<double> &size_vector) {
        std::vector<float> size_vector_float(size_vector.size());
        std::transform(size_vector.begin(), size_vector.end(),
                       size_vector_float.begin(),
                       [](const double &x) { return static_cast<float>(x); });
        marker_size(size_vector_float);
        return *this;
    }

    bool vectors::marker_face() const { return line_spec().marker_face(); }

    class vectors &vectors::marker_face(bool marker_face) {
        line_spec().marker_face(marker_face);
        return *this;
    }

    const std::array<float, 4> &vectors::color() const {
        return line_spec().color();
    }

    const std::array<float, 4> &vectors::marker_color() const {
        return line_spec().marker_color();
    }

    const std::array<float, 4> &vectors::marker_face_color() const {
        return line_spec().marker_face_color();
    }

    bool vectors::use_y2() const { return use_y2_; }

    class vectors &vectors::use_y2(bool use_y_2) {
        use_y2_ = use_y_2;
        touch();
        return *this;
    }

    bool vectors::impulse() const { return impulse_; }

    class vectors &vectors::impulse(bool impulse) {
        impulse_ = impulse;
        touch();
        return *this;
    }

    bool vectors::fill() const { return fill_; }

    class vectors &vectors::fill(bool fill) {
        fill_ = fill;
        touch();
        return *this;
    }

    bool vectors::use_y_2() const { return use_y2_; }

    class vectors &vectors::use_y_2(bool use_y_2) {
        use_y2_ = use_y_2;
        touch();
        return *this;
    }

    bool vectors::polar() const { return polar_; }

    class vectors &vectors::polar(bool polar) {
        polar_ = polar;
        touch();
        return *this;
    }

    bool vectors::normalize() const { return normalize_; }

    class vectors &vectors::normalize(bool normalize) {
        normalize_ = normalize;
        touch();
        return *this;
    }

    double vectors::scale() const { return scale_; }

    class vectors &vectors::scale(double scale) {
        scale_ = scale;
        touch();
        return *this;
    }

} // namespace matplot
