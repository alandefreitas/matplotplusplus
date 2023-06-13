//
// Created by Alan Freitas on 11/07/20.
//

#include <algorithm>
#include <cmath>
#include <matplot/axes_objects/bars.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <sstream>

namespace matplot {

    bars::bars(class axes_type *parent) : axes_object(parent) {}

    bars::bars(class axes_type *parent, const std::vector<double> &y)
        : bars(parent, std::vector<std::vector<double>>({y})) {}

    bars::bars(class axes_type *parent,
               const std::vector<std::vector<double>> &Y)
        : axes_object(parent), ys_(Y) {
        if (parent_->children().empty()) {
            parent_->x_axis().limits({0, double(ys_[0].size() + 1)});
            if (ys_[0].size() <= 15) {
                parent_->x_axis().tick_values(
                    iota(1., static_cast<double>(ys_[0].size())));
            }
        }
        if (parent_->y_axis().limits_mode_auto()) {
            double min_min = ys_[0][0];
            for (size_t i = 0; i < ys_.size(); ++i) {
                min_min = std::min(
                    min_min, *std::min_element(ys_[i].begin(), ys_[i].end()));
            }
            if (min_min > 0) {
                parent_->y_axis().limits({ymin(), inf});
            }
        }
        parent_->x_axis().zero_axis(true);
    }

    bars::bars(class axes_type *parent, const std::vector<double> &x,
               const std::vector<double> &y)
        : bars(parent, x, std::vector<std::vector<double>>({y})) {}

    bars::bars(class axes_type *parent, const std::vector<double> &x,
               const std::vector<std::vector<double>> &Y)
        : axes_object(parent), x_(x), ys_(Y) {
        if (parent_->children().empty()) {
            double min_x_diff = x_[1] - x_[0];
            for (size_t i = 1; i < x_.size() - 1; ++i) {
                double x_diff = x_[i + 1] - x_[i];
                if (x_diff < min_x_diff) {
                    min_x_diff = x_diff;
                }
            }
            parent_->x_axis().limits(
                {x.front() - min_x_diff, x.back() + min_x_diff});
            if (ys_[0].size() <= 15) {
                parent_->x_axis().tick_values(x);
            }
        }
        if (parent_->y_axis().limits_mode_auto()) {
            double min_min = ys_[0][0];
            for (size_t i = 0; i < ys_.size(); ++i) {
                min_min = std::min(
                    min_min, *std::min_element(ys_[i].begin(), ys_[i].end()));
            }
            if (min_min > 0) {
                parent_->y_axis().limits({ymin(), inf});
            }
        }
        parent_->x_axis().zero_axis(true);
    }

    std::string bars::plot_string() {
        maybe_update_face_colors();
        std::string res;
        for (size_t i = 0; i < ys_.size(); ++i) {
            res += " '-' with boxes linewidth " + num2str(line_width_) +
                   " fillstyle solid border rgb '" + to_string(edge_color_) +
                   "'";

            res += " fillcolor '" + to_string(face_colors_[i]) + "'";
            if (i != ys_.size() - 1) {
                res += ", ";
            }
        }
        return res;
    }

    std::string bars::legend_string(std::string_view title) {
        std::string res;
        for (size_t i = 0; i < ys_.size(); ++i) {
            res += " keyentry with boxes fillstyle solid border rgb '" +
                   to_string(edge_color_) + "' fillcolor '" +
                   to_string(face_colors_[i]) + "' title \"" + escape(title) +
                   "\"";
            if (i != ys_.size() - 1) {
                res += ", ";
            }
        }
        return res;
    }

    double bars::x_minimum_difference() {
        double min_x_diff = 1.;
        if (x_.size() > 1) {
            min_x_diff = x_[1] - x_[0];
            for (size_t i = 1; i < x_.size() - 1; ++i) {
                double x_diff = x_[i + 1] - x_[i];
                if (x_diff < min_x_diff) {
                    min_x_diff = x_diff;
                }
            }
        }
        return min_x_diff;
    }

    double bars::cluster_width() {
        // actual width of the cluster (clusters don't take all their space)
        return range_for_cluster() * (ys_.size() == 1 ? 1.0 : cluster_width_);
    }

    double bars::range_for_cluster() {
        // double min_x_diff = x_minimum_difference();
        const size_t n_bar_groups = ys_.size();
        // space taken by the cluster in the x axis
        return bar_width_ / n_bar_groups;
    }

    double bars::x_end_point(size_t cluster_index, size_t i) {
        // get the x_value (automatic or manual)
        double x_value = x_.size() > i ? x_[i] : i + 1;
        // go to point where bar width starts
        x_value -= bar_width_ / 2;
        // advance according to the group index
        x_value += range_for_cluster() * cluster_index;
        // center
        x_value += range_for_cluster() / 2;
        return x_value;
    }

    std::string bars::data_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        double m = x_minimum_difference();
        double cw = cluster_width();
        for (size_t bar_group = 0; bar_group < ys_.size(); ++bar_group) {
            for (size_t i = 0; i < ys_[bar_group].size(); ++i) {
                // <box center>    <box height>    <box width>
                ss << "    " << x_end_point(bar_group, i) << " "
                   << ys_[bar_group][i] << " " << m * cw << "\n";
            }
            ss << "e\n";
        }
        return ss.str();
    }

    bool bars::requires_colormap() { return false; }

    double bars::xmax() {
        if (!x_.empty()) {
            return *std::max_element(x_.begin(), x_.end());
        } else {
            return static_cast<double>(ys_[0].size() + 1);
        }
    }

    double bars::xmin() {
        if (!x_.empty()) {
            return *std::min_element(x_.begin(), x_.end());
        } else {
            return static_cast<double>(ys_[0].size() + 1);
        }
    }

    double bars::ymax() {
        double max_max = ys_[0][0];
        for (size_t i = 0; i < ys_.size(); ++i) {
            max_max = std::max(max_max,
                               *std::max_element(ys_[i].begin(), ys_[i].end()));
        }
        return max_max;
    }

    double bars::ymin() {
        double min_min = ys_[0][0];
        for (size_t i = 0; i < ys_.size(); ++i) {
            min_min = std::min(min_min,
                               *std::min_element(ys_[i].begin(), ys_[i].end()));
        }
        return std::min(min_min, 0.);
    }

    enum axes_object::axes_category bars::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    void bars::maybe_update_face_colors() {
        if (!manual_face_color_) {
            face_colors_.clear();
            for (size_t i = 0; i < ys_.size(); ++i) {
                face_colors_.emplace_back(parent_->get_color_and_bump());
            }
            manual_face_color_ = true;
        }
    }

    const color_array &bars::face_color() const { return face_colors_[0]; }

    class bars &bars::face_color(const color_array &face_color) {
        face_colors_[0] = face_color;
        manual_face_color_ = true;
        touch();
        return *this;
    }

    class bars &bars::face_color(const std::array<float, 3> &face_color) {
        color_array c = {0, face_color[0], face_color[1], face_color[2]};
        face_colors_[0] = c;
        manual_face_color_ = true;
        touch();
        return *this;
    }

    class bars &bars::face_color(std::initializer_list<float> color) {
        if (color.size() == 3) {
            face_color(to_array<3>(color));
        } else {
            face_color(to_array<4>(color));
        }
        return *this;
    }

    class bars &bars::face_color(std::string_view color) {
        face_color(to_array(color));
        return *this;
    }

    bool bars::manual_face_color() const { return manual_face_color_; }

    class bars &bars::manual_face_color(bool manual_face_color) {
        manual_face_color_ = manual_face_color;
        touch();
        return *this;
    }

    const color_array &bars::edge_color() const { return edge_color_; }

    class bars &bars::edge_color(const color_array &edge_color) {
        edge_color_ = edge_color;
        touch();
        return *this;
    }

    class bars &bars::edge_color(const std::array<float, 3> &edge_color) {
        color_array c = {0, edge_color[0], edge_color[1], edge_color[2]};
        edge_color_ = c;
        touch();
        return *this;
    }

    class bars &bars::edge_color(std::initializer_list<float> color) {
        if (color.size() == 3) {
            edge_color(to_array<3>(color));
        } else {
            edge_color(to_array<4>(color));
        }
        return *this;
    }

    class bars &bars::edge_color(std::string_view color) {
        edge_color(to_array(color));
        return *this;
    }

    const line_spec &bars::edge_style() const { return edge_style_; }

    class bars &bars::edge_style(const line_spec &edge_style) {
        edge_style_ = edge_style;
        touch();
        return *this;
    }

    float bars::line_width() const { return line_width_; }

    class bars &bars::line_width(float line_width) {
        line_width_ = line_width;
        touch();
        return *this;
    }

    bool bars::vertical_orientation() const { return vertical_orientation_; }

    class bars &bars::vertical_orientation(bool vertical_orientation) {
        vertical_orientation_ = vertical_orientation;
        if (!vertical_orientation_) {
            std::cerr << "Cannot set horizontal orientation in gnuplot yet"
                      << std::endl;
            vertical_orientation_ = true;
        }
        return *this;
    }

    bool bars::visible() const { return visible_; }

    class bars &bars::visible(bool visible) {
        visible_ = visible;
        touch();
        return *this;
    }

    float bars::bar_width() const { return bar_width_; }

    class bars &bars::bar_width(float bar_width) {
        if (bar_width != bar_width_) {
            bar_width_ = bar_width;
            touch();
        }
        return *this;
    }

    const std::vector<color_array> &bars::face_colors() const {
        return face_colors_;
    }

    std::vector<color_array> &bars::face_colors() { return face_colors_; }

    class bars &bars::face_colors(const std::vector<color_array> &face_colors) {
        face_colors_ = face_colors;
        touch();
        return *this;
    }

} // namespace matplot