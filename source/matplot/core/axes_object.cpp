//
// Created by Alan Freitas on 2020-07-05.
//

#include <matplot/core/axes_object.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>

namespace matplot {
    axes_object::axes_object(class axes_type *parent) : parent_(parent) {
        const bool axes_have_legend = parent_->legend() != nullptr;
        if (axes_have_legend) {
            const bool all_objects_have_legend =
                parent_->legend()->strings().size() >=
                parent_->children().size();
            if (all_objects_have_legend) {
                parent_->legend()->strings().emplace_back(
                    "data " + num2str(parent_->children().size() + 1));
            }
        }
    }

    axes_object::axes_object(axes_handle parent) : axes_object(parent.get()) {}

    const class axes_type *axes_object::parent() const { return parent_; }

    class axes_type *&axes_object::parent() {
        return parent_;
    }

    void axes_object::touch() { parent_->touch(); }

    void axes_object::parent(class axes_type *&parent) { parent_ = parent; }

    std::string axes_object::data_string() { return ""; }

    std::string axes_object::set_variables_string() { return ""; }

    std::string axes_object::legend_string(std::string_view title) {
        return "keyentry with boxes title \"" + escape(title) + "\"";
    }

    std::string
    axes_object::legend_string(std::vector<std::string>::iterator &titles_begin,
                               std::vector<std::string>::iterator &titles_end) {
        if (!display_name_.empty()) {
            // if this object has its own display name
            return legend_string(display_name_);
        } else {
            // by default, take just one title
            std::string res;
            if (titles_begin != titles_end) {
                res += legend_string(*titles_begin);
            }
            ++titles_begin;
            return res;
        }
    }

    std::string axes_object::unset_variables_string() { return ""; }

    double axes_object::xmin() { return -10; }

    double axes_object::xmax() { return +10; }

    double axes_object::ymin() { return -10; }

    double axes_object::ymax() { return +10; }

    double axes_object::zmin() { return -10; }

    double axes_object::zmax() { return +10; }

    enum axes_object::axes_category axes_object::axes_category() {
        return axes_category::two_dimensional;
    }

    bool axes_object::requires_colormap() { return true; }

    std::string axes_object::tag() { return tag_; }

    void axes_object::tag(std::string_view tag_name) { tag_ = tag_name; }

    bool axes_object::is_2d() {
        return axes_category() == axes_category::two_dimensional;
    }

    bool axes_object::is_3d() {
        return axes_category() == axes_category::three_dimensional ||
               axes_category() == axes_category::three_dimensional_map;
    }

    bool axes_object::is_3d_map() {
        return axes_category() == axes_category::three_dimensional_map;
    }

    bool axes_object::is_polar() {
        return axes_category() == axes_category::polar;
    }

    const std::string &axes_object::display_name() const {
        return display_name_;
    }

    void axes_object::display_name(std::string_view display_name) {
        display_name_ = display_name;
        touch();
    }

    void axes_object::run_draw_commands() {}

} // namespace matplot