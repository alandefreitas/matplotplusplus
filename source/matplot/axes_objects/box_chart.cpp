//
// Created by Alan Freitas on 13/07/20.
//

#include <limits>
#include <matplot/axes_objects/box_chart.h>
#include <matplot/core/axes_object.h>
#include <matplot/core/axes_type.h>
#include <sstream>

namespace matplot {
    box_chart::box_chart(class axes_type *parent) : axes_object(parent) {}

    box_chart::box_chart(class axes_type *parent,
                         const std::vector<double> &y_data,
                         const std::vector<double> &groups)
        : axes_object(parent), y_data_(y_data), x_data_(groups) {}

    std::string box_chart::plot_string() {
        maybe_update_face_color();
        std::vector<double> unique_groups = unique(x_data_);
        if (unique_groups.empty()) {
            unique_groups.emplace_back(1.);
        }
        std::string res;
        // one plot for each category
        for (size_t i = 0; i < unique_groups.size(); ++i) {
            if (i != 0) {
                res += ", ";
            }
            if (box_style_ == box_chart::box_style_option::filled) {
                res += " '-' with boxplot fillstyle solid border rgb '" +
                       to_string(edge_color_) + "' fillcolor '" +
                       to_string(face_color_) + " linewidth " +
                       num2str(edge_width_) + "' pointsize " +
                       num2str(whisker_size_);
            } else {
                res += " '-' with boxplot fillstyle solid 0.0 border rgb '" +
                       to_string(edge_color_) + "' fillcolor '" +
                       to_string(face_color_) + " linewidth " +
                       num2str(edge_width_) + "' pointsize " +
                       num2str(whisker_size_);
            }
            switch (whisker_style_) {
            case line_spec::marker_style::plus_sign:
                res += " pointtype 1";
                break;
            case line_spec::marker_style::circle:
                res += (!whisker_face_ ? " pointtype 6" : " pointtype 7");
                break;
            case line_spec::marker_style::asterisk:
                res += " pointtype 3";
                break;
            case line_spec::marker_style::point:
                res += " pointtype 7";
                break;
            case line_spec::marker_style::cross:
                res += " pointtype 2";
                break;
            case line_spec::marker_style::square:
                res += (!whisker_face_ ? " pointtype 4" : " pointtype 5");
                break;
            case line_spec::marker_style::diamond:
                res += (!whisker_face_ ? " pointtype 12" : " pointtype 13");
                break;
            case line_spec::marker_style::upward_pointing_triangle:
                res += (!whisker_face_ ? " pointtype 8" : " pointtype 9");
                break;
            case line_spec::marker_style::downward_pointing_triangle:
                res += (!whisker_face_ ? " pointtype 10" : " pointtype 11");
                break;
            case line_spec::marker_style::pentagram:
                res += (!whisker_face_ ? " pointtype 14" : " pointtype 15");
                break;
            default:
                // if none or custom, we have to plot the markers as labels
                // the labels should come before the plot command,
                // so there is nothing to do here
                res += "pointtype -1";
                break;
            }
        }
        return res;
    }

    void box_chart::maybe_update_face_color() {
        if (!manual_face_color_) {
            face_color_ = parent_->get_color_and_bump();
            edge_color_ = face_color_;
            manual_face_color_ = true;
            face_color_[0] = 0.6f + 0.4f * face_color_[0];
            edge_color_[0] /= 2.;
            edge_color_[1] /= 2.;
            edge_color_[2] /= 2.;
            edge_color_[3] /= 2.;
        }
    }

    std::string box_chart::set_variables_string() {
        //     set bars {small | large | fullwidth | <size>}
        //     unset bars
        //     show bars
        std::string res;
        if (cap_size_ / 3. != 1.) {
            res += "set bars " + num2str(cap_size_ / 3.) + "\n";
        }
        if (jitter_outliers_) {
            res += "set jitter\n";
        }
        return res;
    }

    std::string box_chart::unset_variables_string() {
        std::string res;
        if (cap_size_ / 3. != 1.) {
            res += "unset bars\n";
        }
        if (jitter_outliers_) {
            res += "unset jitter\n";
        }
        return "";
    }

    std::string box_chart::data_string() {
        if (x_data_.empty()) {
            x_data_ = std::vector<double>(y_data_.size(), 1.0);
        }
        std::vector<double> unique_groups = unique(x_data_);
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        // for each group
        for (size_t i = 0; i < unique_groups.size(); ++i) {
            // for each point
            for (size_t j = 0; j < y_data_.size(); ++j) {
                // if point is part of this group
                if (unique_groups[i] == x_data_[j]) {
                    ss << "    " << unique_groups[i] << "  " << y_data_[j]
                       << "  " << box_width_ << "\n";
                }
            }
            ss << "e\n";
        }
        return ss.str();
    }

    bool box_chart::requires_colormap() { return false; }

    enum axes_object::axes_category box_chart::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    box_chart::box_style_option box_chart::box_style() const {
        return box_style_;
    }

    class box_chart &
    box_chart::box_style(box_chart::box_style_option box_style) {
        if (box_style != box_style_) {
            box_style_ = box_style;
            if (box_style_ == box_style_option::outline) {
                whisker_style_ = line_spec::marker_style::plus_sign;
            } else {
                whisker_style_ = line_spec::marker_style::circle;
            }
            touch();
        }
        return *this;
    }

    const std::vector<double> &box_chart::y_data() const { return y_data_; }

    class box_chart &box_chart::y_data(const std::vector<double> &y_data) {
        y_data_ = y_data;
        touch();
        return *this;
    }

    const std::vector<double> &box_chart::x_data() const { return x_data_; }

    class box_chart &box_chart::x_data(const std::vector<double> &x_data) {
        x_data_ = x_data;
        touch();
        return *this;
    }

    const color_array &box_chart::face_color() const { return face_color_; }

    class box_chart &box_chart::face_color(const color_array &face_color) {
        face_color_ = face_color;
        touch();
        return *this;
    }

    bool box_chart::manual_face_color() const { return manual_face_color_; }

    class box_chart &box_chart::manual_face_color(bool manual_face_color) {
        manual_face_color_ = manual_face_color;
        touch();
        return *this;
    }

    const color_array &box_chart::edge_color() const { return edge_color_; }

    class box_chart &box_chart::edge_color(const color_array &edge_color) {
        edge_color_ = edge_color;
        touch();
        return *this;
    }

    float box_chart::edge_width() const { return edge_width_; }

    class box_chart &box_chart::edge_width(float edge_width) {
        edge_width_ = edge_width;
        touch();
        return *this;
    }

    enum line_spec::marker_style box_chart::whisker_style() const {
        return whisker_style_;
    }

    class box_chart &
    box_chart::whisker_style(enum line_spec::marker_style whisker_style) {
        whisker_style_ = whisker_style;
        touch();
        return *this;
    }

    float box_chart::whisker_size() const { return whisker_size_; }

    class box_chart &box_chart::whisker_size(float whisker_size) {
        whisker_size_ = whisker_size;
        touch();
        return *this;
    }

    bool box_chart::whisker_face() const { return whisker_face_; }

    class box_chart &box_chart::whisker_face(bool whisker_face) {
        whisker_face_ = whisker_face;
        touch();
        return *this;
    }

    bool box_chart::jitter_outliers() const { return jitter_outliers_; }

    class box_chart &box_chart::jitter_outliers(bool jitter_outliers) {
        jitter_outliers_ = jitter_outliers;
        touch();
        return *this;
    }

    float box_chart::box_width() const { return box_width_; }

    class box_chart &box_chart::box_width(float box_width) {
        box_width_ = box_width;
        touch();
        return *this;
    }

    float box_chart::cap_size() const { return cap_size_; }

    class box_chart &box_chart::cap_size(float cap_size) {
        cap_size_ = cap_size;
        touch();
        return *this;
    }

} // namespace matplot