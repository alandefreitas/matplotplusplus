//
// Created by Alan Freitas on 16/07/20.
//

#include <algorithm>
#include <cmath>
#include <matplot/axes_objects/labels.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {
    labels::labels(class axes_type *parent, const std::vector<double> &x,
                   const std::vector<double> &y,
                   const std::vector<std::string> &labels,
                   const std::vector<double> &colors,
                   const std::vector<double> &sizes)
        : axes_object(parent), x_(x), y_(y), labels_(labels), colors_(colors),
          sizes_(sizes) {}

    std::string labels::set_variables_string() {
        std::string res;
        if (rectangles_) {
            auto word_size = [this](size_t word_index) {
                if (sizes_.size() > word_index) {
                    return round(sizes_[word_index]);
                } else {
                    return static_cast<double>(round(parent_->font_size()));
                }
            };
            for (size_t i = 0; i < labels_.size(); ++i) {
                double s = word_size(i);
                res += "    set object " + num2str(1000 + i) +
                       " rectangle at " + num2str(x_[i]) + ", " +
                       num2str(y_[i]) + " size " +
                       num2str(s * labels_[i].size() * width_factor) + ", " +
                       num2str(s * height_factor) + " fc lt 2 \n";
            }
        }
        return res;
    }

    std::string labels::plot_string() {
        std::string res = " '-' with labels";
        switch (alignment_) {
        case alignment::left:
            res += " left";
            break;
        case alignment::right:
            res += " right";
            break;
        case alignment::center:
            res += " center";
            break;
        default:
            break;
        }
        res += " font \"" + escape(font()) + "," +
               num2str(unsigned(font_size())) + "\"";
        if (!colors_.empty()) {
            res += " textcolor palette";
        } else {
            res += " textcolor rgb '" + to_string(color_) + "'";
        }
        return res;
    }

    std::string labels::data_string() {
        // in absolute size, the rectangles match the words when
        // the ranges are x [-17;+17], y [-17,+17]
        // if the ranges are smaller than that, we increase the font size
        // if the ranges are larger than that, we reduce the font size
        double font_size_factor = 1.0;
        if (!absolute_size_) {
            auto [xmin, xmax, ymin, ymax] = parent_->child_limits();
            if (parent_->x_axis().limits_mode_manual()) {
                auto [axmin, axmax] = parent_->x_axis().limits();
                if (std::isfinite(axmin)) {
                    xmin = std::min(xmin, axmin);
                }
                if (std::isfinite(axmax)) {
                    xmax = std::max(xmax, axmax);
                }
            }
            if (parent_->y_axis().limits_mode_manual()) {
                auto [aymin, aymax] = parent_->y_axis().limits();
                if (std::isfinite(aymin)) {
                    ymin = std::min(ymin, aymin);
                }
                if (std::isfinite(aymax)) {
                    ymax = std::max(ymax, aymax);
                }
            }
            double xrange = xmax - xmin;
            double yrange = ymax - ymin;
            double xrange_increase = xrange / 34;
            double yrange_increase = yrange / 34;
            font_size_factor = (xrange_increase + yrange_increase) / 2.;
        }
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        const bool custom_sizes = !sizes_.empty();
        const bool custom_colors = !colors_.empty();
        for (size_t i = 0; i < labels_.size(); ++i) {
            ss << "    " << x_[i] << "  " << y_[i] << "  \"";
            if (custom_sizes) {
                ss << "{/=" << round(sizes_[i] / font_size_factor) << " ";
            }
            ss << escape(labels_[i]);
            if (custom_sizes) {
                ss << "}";
            }
            ss << "\"  ";
            if (custom_colors) {
                ss << colors_[i];
            }
            ss << "\n";
        }
        ss << "e\n";
        return ss.str();
    }

    std::string labels::unset_variables_string() {
        std::string res;
        if (rectangles_) {
            for (size_t i = 0; i < labels_.size(); ++i) {
                res += "    unset object " + num2str(1000 + i) + "\n";
            }
        }
        return res;
    }

    double labels::xmax() {
        auto it = std::max_element(x_.begin(), x_.end());
        if (it != x_.end()) {
            return *it + 0.2 + labels_[it - x_.begin()].size() * 0.01;
        } else {
            return axes_object::xmax();
        }
    }

    double labels::xmin() {
        auto it = std::min_element(x_.begin(), x_.end());
        if (it != x_.end()) {
            return *it - 0.2;
        } else {
            return axes_object::xmin();
        }
    }

    double labels::ymax() {
        auto it = std::max_element(y_.begin(), y_.end());
        if (it != y_.end()) {
            return *it + 0.2;
        } else {
            return axes_object::ymax();
        }
    }

    double labels::ymin() {
        auto it = std::min_element(y_.begin(), y_.end());
        if (it != y_.end()) {
            return *it - 0.2;
        } else {
            return axes_object::ymin();
        }
    }

    enum axes_object::axes_category labels::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    bool labels::rectangles() const { return rectangles_; }

    class labels &labels::rectangles(bool rectangles) {
        rectangles_ = rectangles;
        touch();
        return *this;
    }

    bool labels::absolute_size() const { return absolute_size_; }

    class labels &labels::absolute_size(bool absolute_size) {
        absolute_size_ = absolute_size;
        touch();
        return *this;
    }

    enum labels::alignment labels::alignment() const { return alignment_; }

    class labels &labels::alignment(enum labels::alignment alignment) {
        alignment_ = alignment;
        touch();
        return *this;
    }

    const color_array &labels::color() const { return color_; }

    class labels &labels::color(const color_array &color) {
        color_ = color;
        touch();
        return *this;
    }

    const std::string &labels::font() const { return font_; }

    class labels &labels::font(std::string_view font) {
        font_ = font;
        touch();
        return *this;
    }

    float labels::font_size() const { return font_size_; }

    class labels &labels::font_size(float font_size) {
        font_size_ = font_size;
        touch();
        return *this;
    }

    const std::vector<double> &labels::x() const { return x_; }

    class labels &labels::x(const std::vector<double> &x) {
        x_ = x;
        touch();
        return *this;
    }

    const std::vector<double> &labels::y() const { return y_; }

    class labels &labels::y(const std::vector<double> &y) {
        y_ = y;
        touch();
        return *this;
    }

    const std::vector<std::string> &labels::label_values() const {
        return labels_;
    }

    class labels &labels::label_values(const std::vector<std::string> &labels) {
        labels_ = labels;
        touch();
        return *this;
    }

    const std::vector<double> &labels::colors() const { return colors_; }

    class labels &labels::colors(const std::vector<double> &colors) {
        colors_ = colors;
        touch();
        return *this;
    }

    const std::vector<double> &labels::sizes() const { return sizes_; }

    class labels &labels::sizes(const std::vector<double> &sizes) {
        sizes_ = sizes;
        touch();
        return *this;
    }

    bool labels::visible() const { return visible_; }

    class labels &labels::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

} // namespace matplot
