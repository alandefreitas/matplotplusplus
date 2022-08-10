//
// Created by Alan Freitas on 2020-07-05.
//

#include <cmath>
#if defined(__unix__)
#include <time.h>
#elif defined(_WIN32)
#include <time.h>
#else
#include <ctime>
#endif
#include <matplot/core/axes_type.h>
#include <matplot/core/axis_type.h>
#include <matplot/core/figure_type.h>
#include <matplot/util/colors.h>
#include <matplot/util/common.h>

namespace matplot {

    axis_type::axis_type() : axis_type(nullptr, inf, inf) {}

    axis_type::axis_type(class axes_type *parent)
        : axis_type(parent, inf, inf) {}

    axis_type::axis_type(class axes_type *parent, bool visible)
        : axis_type(parent, inf, inf, visible) {}

    axis_type::axis_type(class axes_type *parent, double min, double max)
        : axis_type(parent, min, max, true) {}

    axis_type::axis_type(class axes_type *parent, double min, double max,
                         bool visible)
        : limits_({min, max}), visible_(visible), parent_(parent) {}

    void axis_type::touch() { parent_->touch(); }

    std::array<double, 2> axis_type::limits() const {
        if (parent_->parent()->backend()->consumes_gnuplot_commands()) {
            return limits_;
        } else {
            if (!std::isfinite(limits_[0]) || !std::isfinite(limits_[1])) {
                return {-10, +10};
            } else {
                return limits_;
            }
        }
    }

    class axis_type &axis_type::limits(const std::array<double, 2> &limits) {
        limits_ = limits;
        limits_mode_manual(true);
        touch();
        return *this;
    }

    bool axis_type::limits_mode_auto() const {
        return limits_mode_auto_ ||
               (!std::isfinite(limits_[0]) && !std::isfinite(limits_[0]));
    }

    bool axis_type::limits_mode_manual() const { return !limits_mode_auto_; }

    class axis_type &axis_type::limits_mode_auto(bool limits_mode_auto) {
        limits_mode_auto_ = limits_mode_auto;
        parent_->touch();
        return *this;
    }

    class axis_type &axis_type::limits_mode_manual(bool limits_mode_manual) {
        limits_mode_auto_ = !limits_mode_manual;
        parent_->touch();
        return *this;
    }

    std::string axis_type::range_string() const {
        if (!reverse_) {
            std::string r = "[";
            if (!limits_mode_auto_ && std::isfinite(limits_[0])) {
                r += std::to_string(limits_[0]);
            }
            r += ":";
            if (!limits_mode_auto_ && std::isfinite(limits_[1])) {
                r += std::to_string(limits_[1]);
            }
            r += "] noreverse";
            return r;
        } else {
            std::string r = "[";
            if (!limits_mode_auto_ && std::isfinite(limits_[1])) {
                r += std::to_string(limits_[1]);
            }
            r += ":";
            if (!limits_mode_auto_ && std::isfinite(limits_[0])) {
                r += std::to_string(limits_[0]);
            }
            r += "] reverse";
            return r;
        }
    }

    bool axis_type::reverse() const { return reverse_; }

    class axis_type &axis_type::reverse(bool reverse) {
        reverse_ = reverse;
        parent_->touch();
        return *this;
    }

    const color_array &axis_type::color() const { return color_; }

    class axis_type &axis_type::color(const color_array &color) {
        color_ = color;
        touch();
        return *this;
    }

    class axis_type &axis_type::color(std::string_view c) {
        color(string_to_color(c));
        return *this;
    }

    class axis_type &axis_type::color(const enum color &c) {
        return color(to_array(c));
    }

    const std::string &axis_type::label() const { return label_; }

    class axis_type &axis_type::label(std::string_view label) {
        label_ = label;
        touch();
        return *this;
    }

    std::string axis_type::label_string() const {
        std::string cmd = " \"";
        if (label_weight_ == "bold") {
            cmd += "{/:Bold ";
        }
        if (label_weight_ == "italic") {
            cmd += "{/:Italic ";
        }
        cmd += escape(label());
        if (label_weight_ == "bold") {
            cmd += "}";
        }
        if (label_weight_ == "italic") {
            cmd += "}";
        }
        cmd += "\"";
        if (parent_->parent()->backend()->supports_fonts()) {
            cmd += " font \"" + escape(parent_->font()) + "," +
                   num2str(unsigned(label_font_size_)) + "\"";
            cmd += " textcolor \"" + to_string(label_color_) + "\"";
        }
        return cmd;
    }

    const std::string &axis_type::tick_label_format() const {
        return tick_label_format_;
    }

    class axis_type &
    axis_type::tick_label_format(std::string_view tick_label_format) {
        if (tick_label_format == "usd") {
            tick_label_format_ = "$%.2f";
        } else if (tick_label_format == "degrees") {
            tick_label_format_ = "%g°";
        } else if (tick_label_format == "percentage") {
            tick_label_format_ = "%g%%";
        } else if (tick_label_format == "auto") {
            tick_label_format_ = "%g";
        } else {
            tick_label_format_ = tick_label_format;
        }
        touch();
        return *this;
    }

    bool axis_type::tick_values_automatic() const {
        return tick_values_automatic_;
    }

    class axis_type &
    axis_type::tick_values_automatic(bool tick_values_automatic) {
        tick_values_automatic_ = tick_values_automatic;
        touch();
        return *this;
    }

    bool axis_type::tick_values_manual() const {
        return !tick_values_automatic_;
    }

    class axis_type &axis_type::tick_values_manual(bool tick_values_manual) {
        tick_values_automatic_ = !tick_values_manual;
        touch();
        return *this;
    }

    const std::vector<double> &axis_type::tick_values() const {
        return tick_values_;
    }

    class axis_type &
    axis_type::tick_values(const std::vector<double> &tick_values) {
        tick_values_ = tick_values;
        tick_values_automatic_ = false;
        touch();
        return *this;
    }

    const std::vector<std::string> &axis_type::ticklabels() const {
        return ticklabels_;
    }

    class axis_type &
    axis_type::ticklabels(const std::vector<std::string> &ticklabels) {
        ticklabels_ = ticklabels;
        if (ticklabels.empty()) {
            tick_values({});
        }
        touch();
        return *this;
    }

    bool axis_type::ticklabels_mode() const { return ticklabels_mode_; }

    class axis_type &axis_type::ticklabels_mode(bool ticklabels_mode) {
        ticklabels_mode_ = ticklabels_mode;
        touch();
        return *this;
    }

    std::string axis_type::tick_values_string(bool minor_ticks) const {
        std::string r = " (";
        // Future work: use calcticks inside tick_values and
        // ticklabels whenever we call this function
        // regardless of whether the ticks are automatic.
        // This requires keeping preprocessed data in this object.
        // std::vector<double> values = tick_values();
        // std::vector<std::string> labels = ticklabels();
        // And proceed with these labels.
        // We can then always use manual labels in the gnuplot
        // backend, which would make axes more consistent
        // with our format labels and avoid approximation
        // errors in polar plots. It would also be useful
        // for a future opengl backend, where we always
        // need explicit labels.
        for (size_t i = 0; i < tick_values_.size(); ++i) {
            if (i != 0) {
                r += ", ";
            }
            if (ticklabels_mode() && ticklabels().size() > i) {
                r += "\"" + escape(ticklabels_[i]) + "\" ";
            } else {
                // Unfortunately, gnuplot cancels the formatting
                // when we set the tic values explicitly.
                // So if there is no explicit label but there is
                // a format we want to apply, we also need to explicitly
                // create a label according to our format.
                if (is_timestamp_) {
                    char buff[20] = {0};
                    time_t now = (time_t)tick_values_[i];
                    struct tm buf;
#if defined(__unix__)
                    localtime_r(&now, &buf);
#elif defined(_WIN32)
                    localtime_s(&buf, &now);
#else
                    buf = *std::localtime(&now);
#endif
                    strftime(buff, 20, tick_label_format_.c_str(), &buf);
                    r += "\"" + escape(std::string(buff)) + "\" ";
                } else {
                    r += "\"" +
                         escape(num2str(tick_values_[i], tick_label_format_)) +
                         "\" ";
                }
            }
            r += num2str(tick_values_[i]);
            if (minor_ticks) {
                r += " 0";
                if (i != tick_values_.size() - 1) {
                    double m_tick =
                        (tick_values_[i] + tick_values_[i + 1]) / 2.;
                    r += ", " + num2str(m_tick) + " 1";
                }
            }
        }
        r += ")";
        return r;
    }

    std::string axis_type::tick_rotate_string() const {
        if (tickangle_ == 0) {
            return " norotate";
        } else {
            return " rotate by " + num2str(tickangle_) + " right";
        }
    }

    axis_type::axis_scale axis_type::scale() const { return scale_; }

    class axis_type &axis_type::scale(axis_type::axis_scale scale) {
        if (scale != scale_) {
            scale_ = scale;
            touch();
        }
        return *this;
    }

    bool axis_type::visible() const { return visible_; }

    class axis_type &axis_type::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float axis_type::tick_length() const { return tick_length_; }

    class axis_type &axis_type::tick_length(float tick_length) {
        tick_length_ = tick_length;
        touch();
        return *this;
    }

    bool axis_type::zero_axis() const { return zero_axis_; }

    class axis_type &axis_type::zero_axis(bool zero_axis) {
        zero_axis_ = zero_axis;
        touch();
        return *this;
    }

    bool axis_type::geographic() const { return geographic_; }

    class axis_type &axis_type::geographic(bool geographic) {
        geographic_ = geographic;
        touch();
        return *this;
    }

    bool axis_type::on_axis() const { return on_axis_; }

    class axis_type &axis_type::on_axis(bool on_axis) {
        on_axis_ = on_axis;
        touch();
        return *this;
    }

    const std::string &axis_type::label_weight() const { return label_weight_; }

    class axis_type &axis_type::label_weight(std::string_view label_weight) {
        label_weight_ = label_weight;
        touch();
        return *this;
    }

    float axis_type::label_font_size() const { return label_font_size_; }

    class axis_type &axis_type::label_font_size(float label_font_size) {
        label_font_size_ = label_font_size;
        touch();
        return *this;
    }

    const color_array &axis_type::label_color() const { return label_color_; }

    class axis_type &axis_type::label_color(const color_array &label_color) {
        label_color_ = label_color;
        touch();
        return *this;
    }

    bool axis_type::is_timestamp() const { return is_timestamp_; }

    class axis_type &axis_type::is_timestamp(bool is_timestamp) {
        is_timestamp_ = is_timestamp;
        touch();
        return *this;
    }

    float axis_type::tickangle() const { return tickangle_; }

    class axis_type &axis_type::tickangle(float tickangle) {
        tickangle_ = tickangle;
        touch();
        return *this;
    }

} // namespace matplot