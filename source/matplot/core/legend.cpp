//
// Created by Alan Freitas on 10/07/20.
//

#include <matplot/core/axes_type.h>
#include <matplot/core/legend.h>
#include <matplot/util/common.h>

namespace matplot {
    legend::legend(class axes_type *parent) : legend(parent, {}) {}

    legend::legend(class axes_type *parent,
                   std::initializer_list<std::string> names)
        : legend(parent, std::vector(names)) {}

    legend::legend(class axes_type *parent,
                   const std::vector<std::string> &names)
        : strings_(names), parent_(parent) {
        if (names.empty() && !parent_->children().empty()) {
            for (size_t i = 0; i < parent_->children().size(); ++i) {
                strings_.emplace_back("data" + num2str(i + 1));
            }
        }
    }

    void legend::touch() { parent_->touch(); }

    const std::string &legend::operator[](size_t index) const {
        return strings_.operator[](index);
    }

    std::string &legend::operator[](size_t index) {
        return strings_.operator[](index);
    }

    size_t legend::size() const { return strings_.size(); }

    bool legend::empty() const { return strings_.empty(); }

    std::vector<std::string>::const_iterator legend::begin() const {
        return strings_.begin();
    }

    std::vector<std::string>::iterator legend::begin() {
        return strings_.begin();
    }

    std::vector<std::string>::const_iterator legend::end() const {
        return strings_.end();
    }

    std::vector<std::string>::iterator legend::end() { return strings_.end(); }

    const std::vector<std::string> &legend::strings() const { return strings_; }

    std::vector<std::string> &legend::strings() { return strings_; }

    void legend::strings(const std::vector<std::string> &strings) {
        strings_ = strings;
        touch();
    }

    void legend::inside(bool inside) {
        if (inside != inside_) {
            inside_ = inside;
            touch();
        }
    }

    bool legend::inside() const { return inside_; }

    bool legend::label_after_sample() const { return label_after_sample_; }

    void legend::label_after_sample(bool label_after_sample) {
        if (label_after_sample != label_after_sample_) {
            label_after_sample_ = label_after_sample;
            touch();
        }
    }

    bool legend::box() const { return box_; }

    void legend::box(bool box) {
        box_ = box;
        touch();
    }

    const line_spec &legend::box_line() const { return box_line_; }

    line_spec &legend::box_line() { return box_line_; }

    void legend::box_line(const line_spec &box_line) {
        box_line_ = box_line;
        touch();
    }

    bool legend::vertical() const { return vertical_; }

    void legend::vertical(bool vertical) {
        if (vertical != vertical_) {
            vertical_ = vertical;
            touch();
        }
    }

    bool legend::horizontal() const { return !vertical(); }

    void legend::horizontal(bool horizontal) { vertical(!horizontal); }

    legend::general_alignment legend::location() const {
        switch (horizontal_location_) {
        case horizontal_alignment::left:
            switch (vertical_location_) {
            case vertical_alignment::top:
                return general_alignment::topleft;
            case vertical_alignment::center:
                return general_alignment::left;
            case vertical_alignment::bottom:
                return general_alignment::bottomleft;
            }
            break;
        case horizontal_alignment::center:
            switch (vertical_location_) {
            case vertical_alignment::top:
                return general_alignment::top;
            case vertical_alignment::center:
                return general_alignment::center;
            case vertical_alignment::bottom:
                return general_alignment::bottom;
            }
            break;
        case horizontal_alignment::right:
            switch (vertical_location_) {
            case vertical_alignment::top:
                return general_alignment::topright;
            case vertical_alignment::center:
                return general_alignment::right;
            case vertical_alignment::bottom:
                return general_alignment::bottomright;
            }
            break;
        }
        throw std::logic_error("legend::location: could not find the legend location");
    }

    void legend::location(general_alignment alignment) {
        switch (alignment) {
        case general_alignment::topleft:
            horizontal_location_ = horizontal_alignment::left;
            vertical_location_ = vertical_alignment::top;
            break;
        case general_alignment::top:
            horizontal_location_ = horizontal_alignment::center;
            vertical_location_ = vertical_alignment::top;
            break;
        case general_alignment::topright:
            horizontal_location_ = horizontal_alignment::right;
            vertical_location_ = vertical_alignment::top;
            break;
        case general_alignment::left:
            horizontal_location_ = horizontal_alignment::left;
            vertical_location_ = vertical_alignment::center;
            break;
        case general_alignment::center:
            horizontal_location_ = horizontal_alignment::center;
            vertical_location_ = vertical_alignment::center;
            break;
        case general_alignment::right:
            horizontal_location_ = horizontal_alignment::right;
            vertical_location_ = vertical_alignment::center;
            break;
        case general_alignment::bottomleft:
            horizontal_location_ = horizontal_alignment::left;
            vertical_location_ = vertical_alignment::bottom;
            break;
        case general_alignment::bottom:
            horizontal_location_ = horizontal_alignment::center;
            vertical_location_ = vertical_alignment::bottom;
            break;
        case general_alignment::bottomright:
            horizontal_location_ = horizontal_alignment::right;
            vertical_location_ = vertical_alignment::bottom;
            break;
        }
        touch();
    }

    legend::horizontal_alignment legend::horizontal_location() const {
        return horizontal_location_;
    }

    void legend::horizontal_location(
        legend::horizontal_alignment horizontal_location) {
        horizontal_location_ = horizontal_location;
    }

    legend::vertical_alignment legend::vertical_location() const {
        return vertical_location_;
    }

    void
    legend::vertical_location(legend::vertical_alignment vertical_location) {
        vertical_location_ = vertical_location;
    }

    bool legend::manual_position() const { return manual_position_; }

    void legend::manual_position(bool manual_position) {
        manual_position_ = manual_position;
        touch();
    }

    const std::array<float, 2> &legend::position() const { return position_; }

    void legend::position(const std::array<float, 2> &position) {
        position_ = position;
        manual_position_ = true;
        touch();
    }

    bool legend::invert() const { return invert_; }

    void legend::invert(bool invert) {
        invert_ = invert;
        touch();
    }

    bool legend::visible() const { return visible_; }

    void legend::visible(bool visible) {
        if (visible_ != visible) {
            visible_ = visible;
            if (strings_.empty() && !parent_->children().empty()) {
                for (size_t i = 0; i < parent_->children().size(); ++i) {
                    strings_.emplace_back("data" + num2str(i + 1));
                }
            }
        }
        touch();
    }

    const std::string &legend::title() const { return title_; }

    void legend::title(std::string_view title) {
        title_ = title;
        touch();
    }

    const std::string &legend::font_name() const { return font_name_; }

    void legend::font_name(std::string_view font_name) {
        font_name_ = font_name;
        touch();
    }

    float legend::font_size() const { return font_size_; }

    void legend::font_size(float font_size) {
        font_size_ = font_size;
        touch();
    }

    const std::string &legend::font_angle() const { return font_angle_; }

    void legend::font_angle(std::string_view font_angle) {
        font_angle_ = font_angle;
        touch();
    }

    const std::string &legend::font_weight() const { return font_weight_; }

    void legend::font_weight(std::string_view font_weight) {
        font_weight_ = font_weight;
        touch();
    }

    const color_array &legend::text_color() const { return text_color_; }

    void legend::text_color(const color_array &text_color) {
        text_color_ = text_color;
        touch();
    }

    size_t legend::num_columns() const { return num_columns_; }

    void legend::num_columns(size_t num_columns) {
        num_columns_ = num_columns;
        num_rows_ = 0;
        touch();
    }

    size_t legend::num_rows() const { return num_rows_; }

    void legend::num_rows(size_t num_rows) {
        num_rows_ = num_rows;
        num_columns_ = 0;
        touch();
    }
} // namespace matplot