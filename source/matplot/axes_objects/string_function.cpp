//
// Created by Alan Freitas on 2020-07-06.
//

#include <matplot/axes_objects/string_function.h>
#include <matplot/core/axes_type.h>

namespace matplot {
    string_function::string_function(class axes_type *parent)
        : string_function(parent, "x", "") {}

    string_function::string_function(class axes_type *parent,
                                     std::string_view equation,
                                     std::string_view line_spec)
        : line(parent, {}, line_spec), equation_(equation) {}

    std::string string_function::plot_string() {
        maybe_update_line_spec();
        std::string res;
        bool first = true;
        for (const auto &style : styles_to_plot()) {
            if (!first) {
                res += ",";
            }
            res += " " + equation_ + " " + line_spec_.plot_string(style, true);
            if (first) {
                first = false;
            }
        }
        return res;
    }

    std::string string_function::data_string() { return ""; }

    double string_function::xmax() {
        if (is_polar()) {
            return 2;
        }
        return axes_object::xmax();
    }

    double string_function::xmin() {
        if (is_polar()) {
            return -2;
        }
        return axes_object::xmin();
    }

    double string_function::ymax() {
        if (is_polar()) {
            return 2;
        }
        return axes_object::ymax();
    }

    double string_function::ymin() {
        if (is_polar()) {
            return -2;
        }
        return axes_object::ymin();
    }

    enum axes_object::axes_category string_function::axes_category() {
        if (polar_) {
            return axes_object::axes_category::polar;
        } else {
            return axes_object::axes_category::two_dimensional;
        }
    }

    const std::string &string_function::equation() const { return equation_; }

    class string_function &
    string_function::equation(std::string_view equation) {
        equation_ = equation;
        parent()->draw();
        return *this;
    }

} // namespace matplot