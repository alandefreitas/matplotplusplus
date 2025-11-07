//
// Created by Alan Freitas on 2020-07-07.
//

#include <cmath>
#include <matplot/axes_objects/line.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <regex>
#include <sstream>

namespace matplot {
    line::line(class axes_type *parent) : axes_object(parent) {}

    line::line(class axes_type *parent, const std::vector<double> &y_data,
               std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data) {}

    line::line(class axes_type *parent, const std::vector<double> &x_data,
               const std::vector<double> &y_data, std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data) {}

    line::line(class axes_type *parent, const std::vector<double> &x_data,
               const std::vector<double> &y_data,
               const std::vector<double> &z_data, std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), y_data_(y_data),
          x_data_(x_data), z_data_(z_data) {}

    std::vector<line_spec::style_to_plot> line::styles_to_plot() {
        std::vector<line_spec::style_to_plot> result;
        // if it has a line...
        if (line_spec_.has_line()) {
            // ...and a marker
            if (line_spec_.has_non_custom_marker()) {
                if (line_spec_.line_and_marker_are_the_same_color()) {
                    if (marker_indices_.empty()) {
                        result.emplace_back(
                            line_spec::style_to_plot::plot_line_and_marker);
                    } else {
                        result.emplace_back(
                            line_spec::style_to_plot::plot_line_only);
                        result.emplace_back(
                            line_spec::style_to_plot::plot_marker_only);
                    }
                } else {
                    result.emplace_back(
                        line_spec::style_to_plot::plot_line_only);
                    result.emplace_back(
                        line_spec::style_to_plot::plot_marker_only);
                }
                if (line_spec_.marker_face()) {
                    if (!line_spec_.marker_and_face_are_the_same_color()) {
                        result.emplace_back(
                            line_spec::style_to_plot::plot_marker_face_only);
                    }
                }
            } else {
                // ...and no marker
                result.emplace_back(line_spec::style_to_plot::plot_line_only);
            }
        } else {
            // ...only a marker
            if (line_spec_.has_non_custom_marker()) {
                result.emplace_back(line_spec::style_to_plot::plot_marker_only);
                if (line_spec_.marker_face() &&
                    !line_spec_.marker_and_face_are_the_same_color()) {
                    result.emplace_back(
                        line_spec::style_to_plot::plot_marker_face_only);
                }
            } else {
                // has nothing, fallback to a solid line
                result.emplace_back(line_spec::style_to_plot::plot_line_only);
            }
        }
        // if we draw the line as impulse
        if (impulse_ || fill_) {
            // we cannot plot the line and marker together
            auto it = std::find(result.begin(), result.end(),
                                line_spec::style_to_plot::plot_line_and_marker);
            if (it != result.end()) {
                result.erase(it);
                result.emplace_back(line_spec::style_to_plot::plot_line_only);
                result.emplace_back(line_spec::style_to_plot::plot_marker_only);
            }
        }
        return result;
    }

    std::string line::plot_string() {
        maybe_update_line_spec();
        std::string res;
        bool first = true;
        for (const auto &style : styles_to_plot()) {
            if (!first) {
                res += ",";
            }
            const bool marker_size_is_variable = !marker_sizes_.empty();
            const bool we_are_plotting_marker =
                style != line_spec::style_to_plot::plot_line_only;

            std::string str;
            if (impulse_ && style == line_spec::style_to_plot::plot_line_only) {
                str =
                    " '-' with impulse " + line_spec_.plot_string(style, false);
            } else if (fill_ &&
                       style == line_spec::style_to_plot::plot_line_only) {
                str = " '-' with filledcurves " +
                      line_spec_.plot_string(style, false);
            } else {
                str = " '-' " + line_spec_.plot_string(style, true);
            }
            if (marker_size_is_variable && we_are_plotting_marker) {
                str = std::regex_replace(str,
                                         std::regex(" pointsize \\d*\\.?\\d* "),
                                         " pointsize variable ");
            }
            const bool color_is_variable = !marker_colors_.empty();
            if (color_is_variable && we_are_plotting_marker) {
                str = std::regex_replace(str,
                                         std::regex(" linecolor rgb +[^ ]+ "),
                                         " linecolor palette ");
            }
            if (use_y2_) {
                str += " axes x1y2";
            }
            res += str;
            if (first) {
                first = false;
            }
        }
        return res;
    }

    std::string line::legend_string(std::string_view title) {
        if (line_spec_.has_line() && line_spec_.has_non_custom_marker()) {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_line_and_marker) +
                   " title \"" + escape(title) + "\"";
        } else if (line_spec_.has_line()) {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_line_only) +
                   " title \"" + escape(title) + "\"";
        } else {
            return " keyentry " +
                   line_spec_.plot_string(
                       line_spec::style_to_plot::plot_marker_only) +
                   " title \"" + escape(title) + "\"";
        }
    }

    std::string line::data_string() {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        const auto& active_x_data = use_buffer_.load(std::memory_order_acquire) ? x_data_buffer_ : x_data_;
        const auto& active_y_data = use_buffer_.load(std::memory_order_acquire) ? y_data_buffer_ : y_data_;
        const auto& active_z_data = use_buffer_.load(std::memory_order_acquire) ? z_data_buffer_ : z_data_;
        const bool markers_are_automatic = marker_indices_.empty();
        // const bool has_line_and_marker = line_spec_.has_line() &&
        // line_spec_.has_non_custom_marker(); const bool can_plot_together =
        // markers_are_automatic &&
        // line_spec_.can_plot_line_and_marker_together() &&
        // has_line_and_marker; const bool needs_to_plot_twice =
        // !can_plot_together && has_line_and_marker; size_t repetitions = 1 +
        // needs_to_plot_twice;
        const bool x_is_manual = !active_x_data.empty();

        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        for (const auto &style : styles_to_plot()) {
            if (visible_) {
                const bool data_is_for_markers =
                    style == line_spec::style_to_plot::plot_marker_only ||
                    style == line_spec::style_to_plot::plot_marker_face_only;
                const bool only_at_marker_indices =
                    !markers_are_automatic && data_is_for_markers;
                const size_t n_points = only_at_marker_indices
                                            ? marker_indices_.size()
                                            : active_y_data.size();

                const bool marker_size_is_variable = !marker_sizes_.empty();
                const bool we_are_plotting_markers =
                    style != line_spec::style_to_plot::plot_line_only;
                const bool include_marker_size =
                    marker_size_is_variable && we_are_plotting_markers;
                const double marker_size_denominator =
                    style == line_spec::style_to_plot::plot_marker_face_only
                        ? 10
                        : 6;

                const bool color_is_variable = !marker_colors_.empty();
                const bool include_marker_color =
                    color_is_variable && we_are_plotting_markers;

                for (size_t i = 0; i < n_points; ++i) {
                    size_t index =
                        only_at_marker_indices ? marker_indices_[i] : i;

                    if (x_is_manual && index >= active_x_data.size()) {
                        break;
                    }
                    double x_value = x_is_manual ? active_x_data[index] : index + 1;
                    if (!std::isfinite(x_value) || !std::isfinite(active_y_data[i])) {
                        ss << "    \n";
                        continue;
                    }
                    ss << "    " << x_value;

                    ss << "  " << active_y_data[index];

                    if (is_3d()) {
                        ss << "  " << active_z_data[index];
                    }

                    if (include_marker_size) {
                        ss << "  "
                           << marker_sizes_[index] / marker_size_denominator;
                    }

                    if (include_marker_color) {
                        ss << "  " << marker_colors_[index];
                    }

                    ss << "  \n";
                }
            }
            ss << "    "
               << "e\n";
        }
        return ss.str();
    }

    void line::maybe_update_line_spec() {
        if (line_spec_.has_line() && !line_spec_.user_color()) {
            // if user didn't set the color, get color from xlim
            auto c = parent_->get_color_and_bump();
            line_spec_.color(c);
        } else if (line_spec_.has_non_custom_marker() &&
                   !line_spec_.user_color() &&
                   !line_spec_.marker_user_color()) {
            auto c = parent_->get_color_and_bump();
            line_spec_.marker_color(c);
        }
    }

    double line::xmax() {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        const auto& active_x_data = use_buffer_.load(std::memory_order_acquire) ? x_data_buffer_ : x_data_;
        const auto& active_y_data = use_buffer_.load(std::memory_order_acquire) ? y_data_buffer_ : y_data_;
        if (!is_polar()) {
            if (active_x_data.empty()) {
                if (!active_y_data.empty()) {
                    return static_cast<double>(active_y_data.size() - 1);
                } else {
                    return axes_object::xmax();
                }
            }
            return *std::max_element(active_x_data.begin(), active_x_data.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return +(parent_->r_axis().limits()[1] - 
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] = 
                std::minmax_element(active_y_data.begin(), active_y_data.end());
            if (max_rho != active_y_data.end() && min_rho != active_y_data.end()) {
                return +round_polar_max(std::abs(*max_rho));
            } else {
                return 1;
            }
        }
    }

    double line::xmin() {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        const auto& active_x_data = use_buffer_.load(std::memory_order_acquire) ? x_data_buffer_ : x_data_;
        const auto& active_y_data = use_buffer_.load(std::memory_order_acquire) ? y_data_buffer_ : y_data_;
        if (!is_polar()) {
            if (active_x_data.empty()) {
                if (!active_y_data.empty()) {
                    return 0;
                } else {
                    return axes_object::xmin();
                }
            }
            return *std::min_element(active_x_data.begin(), active_x_data.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return -(parent_->r_axis().limits()[1] - 
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] = 
                std::minmax_element(active_y_data.begin(), active_y_data.end());
            if (max_rho != active_y_data.end() && min_rho != active_y_data.end()) {
                return -round_polar_max(std::abs(*max_rho));
            } else {
                return -1;
            }
        }
    }

    double line::ymax() {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        const auto& active_y_data = use_buffer_.load(std::memory_order_acquire) ? y_data_buffer_ : y_data_;
        if (!is_polar()) {
            if (active_y_data.empty()) {
                return axes_object::ymax();
            }
            return *std::max_element(active_y_data.begin(), active_y_data.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return +(parent_->r_axis().limits()[1] - 
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] = 
                std::minmax_element(active_y_data.begin(), active_y_data.end());
            if (max_rho != active_y_data.end() && min_rho != active_y_data.end()) {
                return +round_polar_max(std::abs(*max_rho));
            } else {
                return 1;
            }
        }
    }

    double line::ymin() {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        const auto& active_y_data = use_buffer_.load(std::memory_order_acquire) ? y_data_buffer_ : y_data_;
        if (!is_polar()) {
            if (active_y_data.empty()) {
                return axes_object::ymin();
            }
            return *std::min_element(active_y_data.begin(), active_y_data.end());
        } else {
            // y = rho
            if (parent_->r_axis().limits_mode_manual()) {
                return -(parent_->r_axis().limits()[1] - 
                         parent_->r_axis().limits()[0]);
            }
            auto [min_rho, max_rho] = 
                std::minmax_element(active_y_data.begin(), active_y_data.end());
            if (max_rho != active_y_data.end() && min_rho != active_y_data.end()) {
                return -round_polar_max(std::abs(*max_rho));
            } else {
                return -1;
            }
        }
    }

    enum axes_object::axes_category line::axes_category() {
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

    bool line::requires_colormap() { return !marker_colors_.empty(); }

    class line &line::line_style(std::string_view str) {
        line_spec_.parse_string(str);
        touch();
        return *this;
    }

    const line_spec &line::line_spec() const { return line_spec_; }

    line_spec &line::line_spec() { return line_spec_; }

    class line &line::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const std::vector<double> &line::y_data() const {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        if (use_buffer_.load(std::memory_order_acquire)) {
            return y_data_buffer_;
        } else {
            return y_data_;
        }
    }

    class line &line::y_data(const std::vector<double> &y_data) {
        std::unique_lock<std::shared_mutex> lock(data_mutex_);
        // Write to the inactive buffer
        if (use_buffer_.load(std::memory_order_acquire)) {
            y_data_ = y_data;
        } else {
            y_data_buffer_ = y_data;
        }
        // Toggle buffer
        use_buffer_.store(!use_buffer_.load(std::memory_order_acquire), std::memory_order_release);
        touch();
        return *this;
    }

    const std::vector<double> &line::x_data() const {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        if (use_buffer_.load(std::memory_order_acquire)) {
            return x_data_buffer_;
        } else {
            return x_data_;
        }
    }

    class line &line::x_data(const std::vector<double> &x_data) {
        std::unique_lock<std::shared_mutex> lock(data_mutex_);
        // Write to the inactive buffer
        if (use_buffer_.load(std::memory_order_acquire)) {
            x_data_ = x_data;
        } else {
            x_data_buffer_ = x_data;
        }
        // Toggle buffer
        use_buffer_.store(!use_buffer_.load(std::memory_order_acquire), std::memory_order_release);
        touch();
        return *this;
    }

    const std::vector<double> &line::z_data() const {
        std::shared_lock<std::shared_mutex> lock(data_mutex_);
        if (use_buffer_.load(std::memory_order_acquire)) {
            return z_data_buffer_;
        } else {
            return z_data_;
        }
    }

    class line &line::z_data(const std::vector<double> &z_data) {
        std::unique_lock<std::shared_mutex> lock(data_mutex_);
        // Write to the inactive buffer
        if (use_buffer_.load(std::memory_order_acquire)) {
            z_data_ = z_data;
        } else {
            z_data_buffer_ = z_data;
        }
        // Toggle buffer
        use_buffer_.store(!use_buffer_.load(std::memory_order_acquire), std::memory_order_release);
        touch();
        return *this;
    }

    const std::vector<size_t> &line::marker_indices() const {
        return marker_indices_;
    }

    class line &
    line::marker_indices(const std::vector<size_t> &marker_indices) {
        marker_indices_ = marker_indices;
        touch();
        return *this;
    }

    bool line::visible() const { return visible_; }

    class line &line::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float line::line_width() const { return line_spec().line_width(); }

    class line &line::line_width(float line_width) {
        line_spec().line_width(line_width);
        return *this;
    }

    enum line_spec::marker_style line::marker_style() const {
        return line_spec().marker();
    }

    enum line_spec::marker_style line::marker() const {
        return line_spec().marker();
    }

    float line::marker_size() const { return line_spec().marker_size(); }

    class line &line::marker_size(float size) {
        line_spec().marker_size(size);
        return *this;
    }

    class line &line::marker_size(const std::vector<float> &size_vector) {
        marker_sizes_ = size_vector;
        touch();
        return *this;
    }

    class line &line::marker_size(const std::vector<double> &size_vector) {
        std::vector<float> size_vector_float(size_vector.size());
        std::transform(size_vector.begin(), size_vector.end(),
                       size_vector_float.begin(),
                       [](const double &x) { return static_cast<float>(x); });
        marker_size(size_vector_float);
        return *this;
    }

    bool line::marker_face() const { return line_spec().marker_face(); }

    class line &line::marker_face(bool marker_face) {
        line_spec().marker_face(marker_face);
        return *this;
    }

    const std::array<float, 4> &line::color() const {
        return line_spec().color();
    }

    const std::array<float, 4> &line::marker_color() const {
        return line_spec().marker_color();
    }

    const std::array<float, 4> &line::marker_face_color() const {
        return line_spec().marker_face_color();
    }

    bool line::use_y2() const { return use_y2_; }

    class line &line::use_y2(bool use_y_2) {
        use_y2_ = use_y_2;
        if (!parent()->y2_axis().visible()) {
            parent()->y2_axis().visible(true);
            parent()->y_axis().color(parent()->colororder()[0]);
            parent()->y2_axis().color(parent()->colororder()[1]);
        }
        touch();
        return *this;
    }

    bool line::impulse() const { return impulse_; }

    class line &line::impulse(bool impulse) {
        impulse_ = impulse;
        touch();
        return *this;
    }

    bool line::fill() const { return fill_; }

    class line &line::fill(bool fill) {
        fill_ = fill;
        touch();
        return *this;
    }

    bool line::use_y_2() const { return use_y2_; }

    class line &line::use_y_2(bool use_y_2) {
        use_y2_ = use_y_2;
        touch();
        return *this;
    }

    bool line::polar() const { return polar_; }

    class line &line::polar(bool polar) {
        polar_ = polar;
        touch();
        return *this;
    }

    void line::run_draw_commands() {
        // ask axes to draw the line
        maybe_update_line_spec();
        parent_->draw_path(x_data_, y_data_, line_spec_.color());
    }

} // namespace matplot