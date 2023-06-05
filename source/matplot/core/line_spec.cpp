//
// Created by Alan Freitas on 2020-07-06.
//

#include <iostream>
#include <matplot/core/line_spec.h>
#include <matplot/util/colors.h>
#include <matplot/util/common.h>
#include <matplot/util/handle_types.h>
#include <string>

namespace matplot {
    line_spec::line_spec() = default;

    line_spec::line_spec(std::string_view expr)
        : marker_style_(marker_style::none) {
        parse_string(expr);
    }

    void line_spec::parse_string(std::string_view expr) {
        for (size_t expr_pos = 0; expr_pos != expr.size(); ++expr_pos) {
            size_t chars_left = expr.size() - expr_pos;
            switch (expr[expr_pos]) {
            case '-':
                if (chars_left != 1 && expr[expr_pos + 1] == '-') {
                    // "--"
                    line_style_ = line_style::dashed_line;
                    ++expr_pos;
                } else if (chars_left != 1 && expr[expr_pos + 1] == '.') {
                    // "-."
                    line_style_ = line_style::dash_dot_line;
                    ++expr_pos;
                } else {
                    line_style_ = line_style::solid_line;
                }
                break;
            case ':':
                line_style_ = line_style::dotted_line;
                break;
            case '+':
                marker_style_ = marker_style::plus_sign;
                break;
            case 'o':
                marker_style_ = marker_style::circle;
                break;
            case '*':
                marker_style_ = marker_style::asterisk;
                break;
            case '.':
                marker_style_ = marker_style::point;
                break;
            case 'x':
                marker_style_ = marker_style::cross;
                break;
            case 's':
                marker_style_ = marker_style::square;
                if (expr.substr(expr_pos, 6) == "square") {
                    expr_pos += 5;
                }
                break;
            case 'd':
                marker_style_ = marker_style::diamond;
                if (expr.substr(expr_pos, 7) == "diamond") {
                    expr_pos += 6;
                }
                break;
            case '^':
                marker_style_ = marker_style::upward_pointing_triangle;
                break;
            case 'V':
            case 'v':
                marker_style_ = marker_style::downward_pointing_triangle;
                break;
            case '>':
                marker_style_ = marker_style::custom;
                custom_marker_ = "▶";
                break;
            case '<':
                marker_style_ = marker_style::custom;
                custom_marker_ = "◀";
                break;
            case 'p':
                marker_style_ = marker_style::pentagram;
                if (expr.substr(expr_pos, 9) == "pentagram") {
                    expr_pos += 8;
                }
                break;
            case 'h':
                marker_style_ = marker_style::hexagram;
                if (expr.substr(expr_pos, 8) == "hexagram") {
                    expr_pos += 7;
                }
                break;
            case 'f':
                marker_face_ = true;
                if (marker_style_ == marker_style::none &&
                    line_style_ == line_style::none) {
                    marker_style_ = marker_style::circle;
                    line_style_ = line_style::solid_line;
                }
                if (expr.substr(expr_pos, 6) == "filled") {
                    expr_pos += 5;
                }
                break;
            case 'b':
            case 'k':
            case 'r':
            case 'g':
            case 'y':
            case 'c':
            case 'm':
            case 'w':
                color_ = to_array(char_to_color(expr[expr_pos]));
                marker_color_ = color_;
                marker_face_color_ = color_;
                user_color_ = true;
                marker_user_color_ = true;
                marker_face_user_color_ = true;
                break;
            }
        }
        if (!has_line() && !has_non_custom_marker()) {
            line_style_ = line_style::solid_line;
        }
    }

    std::string line_spec::plot_string(style_to_plot sty, bool include_style) {
        // plot cos(x) with linespoints linecolor rgb "#000000" dashtype 3
        // linewidth 3 linetype 4
        std::string res;
        if (include_style) {
            switch (sty) {
            case style_to_plot::plot_line_and_marker:
                res += " with linespoints";
                break;
            case style_to_plot::plot_line_only:
                res += " with line";
                break;
            case style_to_plot::plot_marker_only:
            case style_to_plot::plot_marker_face_only:
                res += " with points";
                break;
            }
        }

        switch (sty) {
        case style_to_plot::plot_line_and_marker:
        case style_to_plot::plot_line_only:
            res += " linecolor rgb \"" + to_string(color_) + "\"";
            break;
        case style_to_plot::plot_marker_only:
            res += " linecolor rgb \"" + to_string(marker_color_) + "\"";
            break;
        case style_to_plot::plot_marker_face_only:
            res += " linecolor rgb \"" + to_string(marker_face_color_) + "\"";
            break;
        }

        const bool is_plotting_line =
            sty == style_to_plot::plot_line_and_marker ||
            sty == style_to_plot::plot_line_only;
        if (is_plotting_line) {
            switch (line_style_) {
            case line_style::solid_line:
                res += " dashtype 1";
                break;
            case line_style::dashed_line:
                res += " dashtype '--'";
                break;
            case line_style::dotted_line:
                res += " dashtype '.'";
                break;
            case line_style::dash_dot_line:
                res += " dashtype '-.'";
                break;
            default:
                break;
            }
            res += " linewidth " + num2str(line_width_);
        }

        switch (sty) {
        case style_to_plot::plot_line_and_marker:
        case style_to_plot::plot_marker_only:
            res += " pointsize " + num2str(marker_size_ / 6.);
            break;
        case style_to_plot::plot_line_only:
            break;
        case style_to_plot::plot_marker_face_only:
            res += " pointsize " + num2str(marker_size_ / 10.);
            break;
        }

        std::string marker_type_key;
        switch (sty) {
        case style_to_plot::plot_line_only:
            marker_type_key = " linetype";
            break;
        case style_to_plot::plot_line_and_marker:
        case style_to_plot::plot_marker_only:
        case style_to_plot::plot_marker_face_only:
            marker_type_key = " pointtype";
            break;
        }

        switch (marker_style_) {
        case marker_style::plus_sign:
            res += marker_type_key + " 1";
            break;
        case marker_style::circle:
            res += marker_type_key + (!marker_face_ ? " 6" : " 7");
            break;
        case marker_style::asterisk:
            res += marker_type_key + " 3";
            break;
        case marker_style::point:
            res += marker_type_key + " 7";
            break;
        case marker_style::cross:
            res += marker_type_key + " 2";
            break;
        case marker_style::square:
            res += marker_type_key + (!marker_face_ ? " 4" : " 5");
            break;
        case marker_style::diamond:
            res += marker_type_key + (!marker_face_ ? " 12" : " 13");
            break;
        case marker_style::upward_pointing_triangle:
            res += marker_type_key + (!marker_face_ ? " 8" : " 9");
            break;
        case marker_style::downward_pointing_triangle:
            res += marker_type_key + (!marker_face_ ? " 10" : " 11");
            break;
        case marker_style::pentagram:
            res += marker_type_key + (!marker_face_ ? " 14" : " 15");
            break;
        default:
            // if none or custom, we have to plot the markers as labels
            // the labels should come before the plot command,
            // so there is nothing to do here
            res += marker_type_key + " -1";
            break;
        }
        return res;
    }

    bool line_spec::can_plot_line_and_marker_together() {
        return has_line() && has_non_custom_marker() &&
               line_and_marker_are_the_same_color();
    }

    bool line_spec::has_line() { return line_style_ != line_style::none; }

    bool line_spec::has_non_custom_marker() {
        return marker_style_ != marker_style::none &&
               marker_style_ != marker_style::custom;
    }

    bool line_spec::has_marker_face() {
        return has_non_custom_marker() && marker_face_;
    }

    bool line_spec::line_and_marker_are_the_same_color() const {
        return std::equal(color_.begin(), color_.end(),
                          marker_color_.begin()) &&
               marker_and_face_are_the_same_color();
    }

    bool line_spec::marker_and_face_are_the_same_color() const {
        return std::equal(marker_color_.begin(), marker_color_.end(),
                          marker_face_color_.begin());
    }

    void line_spec::touch() {
        if (touch_function_) {
            touch_function_();
        }
    }

    const std::array<float, 4> &line_spec::color() const { return color_; }

    float line_spec::alpha() const { return color_[0]; }

    void line_spec::color(const std::array<float, 3> &color) {
        color_ = {0, color[0], color[1], color[2]};
        touch();
    }

    void line_spec::color(const std::array<float, 4> &color) {
        color_ = color;
        user_color_ = true;
        if (!marker_user_color_) {
            marker_color_ = color;
        }
        if (!marker_face_user_color_) {
            marker_face_color_ = color;
        }
        touch();
    }

    void line_spec::color(std::initializer_list<float> il) {
        if (il.size() == 4) {
            std::array<float, 4> a{};
            std::copy(il.begin(), il.end(), a.begin());
            color(a);
        } else {
            std::array<float, 3> a{};
            std::copy(il.begin(), il.end(), a.begin());
            color(a);
        }
    }

    void line_spec::color(std::string_view c) {
        color(to_array(string_to_color(c)));
    }

    void line_spec::color(enum color c) { color(to_array(c)); }

    void line_spec::alpha(float alpha) {
        color_[0] = alpha;
        touch();
    }

    bool line_spec::user_color() const { return user_color_; }

    void line_spec::user_color(bool user_color) {
        user_color_ = user_color;
        touch();
    }

    enum line_spec::line_style line_spec::line_style() const {
        return line_style_;
    }

    void line_spec::line_style(enum line_spec::line_style line_style) {
        line_style_ = line_style;
        touch();
    }

    float line_spec::line_width() const { return line_width_; }

    void line_spec::line_width(float line_width) {
        line_width_ = line_width;
        touch();
    }

    enum line_spec::marker_style line_spec::marker_style() const {
        return marker_style_;
    }

    void line_spec::marker_style(enum line_spec::marker_style marker_style) {
        marker_style_ = marker_style;
        touch();
    }

    void line_spec::marker_style(std::string_view marker_style) {
        switch (marker_style[0]) {
        case '+':
            marker_style_ = marker_style::plus_sign;
            break;
        case 'O':
        case 'o':
            marker_style_ = marker_style::circle;
            break;
        case '*':
            marker_style_ = marker_style::asterisk;
            break;
        case '.':
            marker_style_ = marker_style::point;
            break;
        case 'X':
        case 'x':
            marker_style_ = marker_style::cross;
            break;
        case 'S':
        case 's':
            marker_style_ = marker_style::square;
            break;
        case 'D':
        case 'd':
            marker_style_ = marker_style::diamond;
            break;
        case '^':
            marker_style_ = marker_style::upward_pointing_triangle;
            break;
        case 'V':
        case 'v':
            marker_style_ = marker_style::downward_pointing_triangle;
            break;
        case '>':
            marker_style_ = marker_style::custom;
            custom_marker_ = "▶";
            break;
        case '<':
            marker_style_ = marker_style::custom;
            custom_marker_ = "◀";
            break;
        case 'P':
        case 'p':
            marker_style_ = marker_style::pentagram;
            break;
        case 'H':
        case 'h':
            marker_style_ = marker_style::hexagram;
            break;
        default:
            return;
        }
        touch();
    }

    enum line_spec::marker_style line_spec::marker() const {
        return marker_style();
    }

    const std::string &line_spec::custom_marker() const {
        return custom_marker_;
    }

    void line_spec::custom_marker(std::string_view custom_marker) {
        custom_marker_ = custom_marker;
        touch();
    }

    float line_spec::marker_size() const { return marker_size_; }

    void line_spec::marker_size(float marker_size) {
        marker_size_ = marker_size;
        touch();
    }

    const std::array<float, 4> &line_spec::marker_color() const {
        return marker_color_;
    }

    float line_spec::marker_alpha() const { return marker_color_[0]; }

    void line_spec::marker_color(const std::array<float, 3> &color) {
        marker_color(std::array<float, 4>({0, color[0], color[1], color[2]}));
    }

    void line_spec::marker_color(const std::array<float, 4> &marker_color) {
        marker_color_ = marker_color;
        marker_user_color_ = true;
        if (!marker_face_user_color_) {
            marker_face_color_ = marker_color_;
        }
        touch();
    }

    void line_spec::marker_color(std::initializer_list<float> il) {
        if (il.size() == 3) {
            std::array<float, 3> ar{};
            std::copy(il.begin(), il.end(), ar.begin());
            marker_color(ar);
        } else if (il.size() == 4) {
            std::array<float, 4> ar{};
            std::copy(il.begin(), il.end(), ar.begin());
            marker_color(ar);
        }
    }

    void line_spec::marker_color(std::string_view m) {
        marker_color_ = to_array(string_to_color(m));
        touch();
    }

    void line_spec::marker_color(enum color m) {
        marker_color_ = to_array(m);
        touch();
    }

    void line_spec::marker_alpha(float alpha) {
        marker_color_[0] = alpha;
        touch();
    }

    bool line_spec::marker_user_color() const { return marker_user_color_; }

    void line_spec::marker_user_color(bool user_color) {
        marker_user_color_ = user_color;
    }

    const std::array<float, 4> &line_spec::marker_face_color() const {
        return marker_face_color_;
    }

    float line_spec::marker_face_alpha() const { return marker_face_color_[0]; }

    void line_spec::marker_face_color(std::initializer_list<float> color) {
        if (color.size() == 3) {
            std::array<float, 3> ar{};
            std::copy(color.begin(), color.end(), ar.begin());
            marker_face_color(ar);
        } else {
            std::array<float, 3> ar{};
            std::copy(color.begin(), color.end(), ar.begin());
            marker_face_color(ar);
        }
    }

    void line_spec::marker_face_color(std::string_view color) {
        marker_face_color(to_array(color));
    }

    void line_spec::marker_face_color(enum color c) {
        marker_face_color(to_array(c));
    }

    void line_spec::marker_face_alpha(float alpha) {
        marker_face_color_[0] = alpha;
    }

    bool line_spec::marker_face_user_color() const {
        return marker_face_user_color_;
    }

    void line_spec::marker_face_user_color(bool user_color) {
        marker_face_user_color_ = user_color;
    }

    bool line_spec::marker_face() const { return marker_face_; }

    void line_spec::marker_face(bool marker_face) {
        marker_face_ = marker_face;
        touch();
    }

    void line_spec::marker_face_color(
        const std::array<float, 4> &marker_face_color) {
        marker_face_color_ = marker_face_color;
        marker_face_user_color_ = true;
        marker_face_ = true;
        touch();
    }

    void line_spec::marker_face_color(const std::array<float, 3> &color) {
        marker_face_color(
            std::array<float, 4>({0, color[0], color[1], color[2]}));
    }

} // namespace matplot
