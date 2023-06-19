//
// Created by Alan Freitas on 2020-07-07.
//

#include <algorithm>
#include <cmath>
#include <matplot/axes_objects/contours.h>
#include <matplot/axes_objects/histogram.h>
#include <matplot/core/axes_type.h>
#include <matplot/freestanding/plot.h>
#include <matplot/util/common.h>
#include <numeric>
#include <regex>
#include <sstream>
#include <unordered_set>

namespace matplot {
    contours::contours(class axes_type *parent, const vector_2d &X,
                       const vector_2d &Y, const vector_2d &Z,
                       std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), X_data_(X),
          Y_data_(Y), Z_data_(Z) {
        initialize_preprocessed_data();
        contour_generator_ =
            QuadContourGenerator(X_data_, Y_data_, Z_data_, _corner_mask,
                                 static_cast<long>(nchunk_));
    }

    contours::contours(class axes_type *parent, const vector_2d &Z,
                       std::string_view line_spec)
        : axes_object(parent), line_spec_(this, line_spec), Z_data_(Z) {
        initialize_preprocessed_data();
        contour_generator_ =
            QuadContourGenerator(X_data_, Y_data_, Z_data_, _corner_mask,
                                 static_cast<long>(nchunk_));
    }

    std::string contours::set_variables_string() {
        return "    set style textbox opaque margins 0.5, 0.5 fc bgnd noborder "
               "linewidth  1.0\n";
    }

    std::string contours::plot_string() {
        make_sure_data_is_preprocessed();
        // double zmax_ = zmax();
        // double zmin_ = zmin();
        auto [min_it, max_it] =
            std::minmax_element(levels_.begin(), levels_.end());
        double contour_min_level = *min_it;
        double contour_max_level = *max_it;

        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        if (filled_) {
            auto [lower_levels, upper_levels] = get_lowers_and_uppers();
            // Command for background filled curve
            // The background polygon with the whole area has its level defined
            // by the largest polygon. Whatever level is outside this largest
            // polygon is the level for the "background". The largest polygon is
            // always touching the background because because some polygon
            // larger than it would always be touching it. This background
            // polygon will appear if we define a level < z_min.

            // We plot background only if one of the levels is below zmin
            bool plot_background = lower_levels[0] < zmin_;
            if (plot_background) {
                auto &largest_segment_with_children = line_segments_[0];
                auto &largest_segment =
                    std::get<0>(largest_segment_with_children);
                size_t line_index = std::get<0>(largest_segment);
                size_t segment_begin = std::get<1>(largest_segment);
                size_t segment_end = std::get<2>(largest_segment);
                bool parent_is_lower_level =
                    is_lower_level(line_index, segment_begin, segment_end);
                auto previous_color = line_spec_.color();
                bool previous_color_manual = line_spec_.user_color();
                if (parent_is_lower_level) {
                    // background is 1 lower than parent on lower level
                    size_t level_index = line_index > 0 ? line_index - 1 : 0;
                    double background_z_level = lower_levels[level_index];
                    line_spec_.color(parent_->colormap_interpolation(
                        background_z_level, contour_min_level,
                        contour_max_level));
                } else {
                    // background is 1 higher than parent on upper level
                    size_t level_index = line_index > 0 ? line_index - 1 : 0;
                    double background_z_level = upper_levels[level_index];
                    line_spec_.color(parent_->colormap_interpolation(
                        background_z_level, contour_min_level,
                        contour_max_level));
                }

                std::string ls =
                    " '-' with filledcurve " +
                    line_spec_.plot_string(
                        line_spec::style_to_plot::plot_line_only, false);
                // filledcurves need to use the palette to initialize the
                // colorbox
                ls =
                    std::regex_replace(ls, std::regex(" linecolor rgb +[^ ]+ "),
                                       " linecolor palette ");
                ss << ls;
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);
            }

            // Other curves
            for (size_t i = 0; i < line_segments_.size(); ++i) {
                if (i != 0 || plot_background) {
                    ss << ",";
                }
                parent_and_children_type &parent_and_children_segment =
                    line_segments_[i];
                line_segment_type &parent_segment =
                    std::get<0>(parent_and_children_segment);
                size_t line_index = std::get<0>(parent_segment);
                size_t segment_begin = std::get<1>(parent_segment);
                size_t segment_end = std::get<2>(parent_segment);

                bool is_ll =
                    is_lower_level(line_index, segment_begin, segment_end);
                double segment_z_level =
                    is_ll ? lower_levels[line_index] : upper_levels[line_index];

                auto previous_color = line_spec_.color();
                bool previous_color_manual = line_spec_.user_color();

                line_spec_.color(parent_->colormap_interpolation(
                    segment_z_level, contour_min_level, contour_max_level));
                std::string ls =
                    " '-' with filledcurve " +
                    line_spec_.plot_string(
                        line_spec::style_to_plot::plot_line_only, false);
                ls =
                    std::regex_replace(ls, std::regex(" linecolor rgb +[^ ]+ "),
                                       " linecolor palette ");
                ss << ls;
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);

                // create filledcurves for the children
                std::vector<line_segment_type> &children_segments =
                    std::get<1>(parent_and_children_segment);
                for (size_t j = 0; j < children_segments.size(); ++j) {
                    ss << ",";
                    line_segment_type &child_segment = children_segments[j];
                    line_index = std::get<0>(child_segment);
                    segment_begin = std::get<1>(child_segment);
                    segment_end = std::get<2>(child_segment);
                    // upper level
                    // might need to fix that later:
                    // children is not always the upper level
                    // it might be the other way around
                    // we need to check which is clockwise before deciding on
                    // that
                    const bool is_ll2 =
                        is_lower_level(line_index, segment_begin, segment_end);
                    const double segment_z_level2 =
                        is_ll2 ? lower_levels[line_index]
                               : upper_levels[line_index];

                    line_spec_.color(parent_->colormap_interpolation(
                        segment_z_level2, contour_min_level,
                        contour_max_level));
                    std::string ls2 =
                        " '-' with filledcurve " +
                        line_spec_.plot_string(
                            line_spec::style_to_plot::plot_line_only, false);
                    ls2 = std::regex_replace(
                        ls2, std::regex(" linecolor rgb +[^ ]+ "),
                        " linecolor palette ");
                    ss << ls2;
                }
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);
            }

            // check if Z has nans
            bool z_has_nans = false;
            for (size_t i = 0; !z_has_nans && i < Z_data_.size(); ++i) {
                for (size_t j = 0; !z_has_nans && j < Z_data_[i].size(); ++j) {
                    if (!std::isfinite(Z_data_[i][j])) {
                        z_has_nans = true;
                    }
                }
            }
            if (z_has_nans) {
                // include another filled curve with background color
                // this curve will hide everything that has nan in it
                auto previous_color = line_spec_.color();
                bool previous_color_manual = line_spec_.user_color();
                line_spec_.color(parent_->color());
                std::string ls =
                    ", '-' with filledcurve " +
                    line_spec_.plot_string(
                        line_spec::style_to_plot::plot_line_only, false);
                // this one does not use palletes. it's always the background
                // color
                ss << ls;
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);
            }
        }

        // plot normal contour lines
        bool first_non_empty_line = true;
        for (size_t i = 0; i < lines_.size(); ++i) {
            if (lines_[i].first.empty()) {
                continue;
            }
            if (!first_non_empty_line || filled_) {
                ss << ",";
            } else {
                first_non_empty_line = false;
            }
            auto previous_color = line_spec_.color();
            bool previous_color_manual = line_spec_.user_color();
            if (!previous_color_manual && filled_ &&
                !colormap_line_when_filled_) {
                line_spec_.color("black");
            }

            ss << " '-' ";
            std::string ls = line_spec_.plot_string(
                line_spec::style_to_plot::plot_line_only);

            // we might need to use the palette to draw colors
            if (!previous_color_manual) {
                if (!filled_ || colormap_line_when_filled_) {
                    ls = std::regex_replace(
                        ls, std::regex(" linecolor rgb +[^ ]+ "),
                        " linecolor palette ");
                }
            }
            ss << ls;

            if (!previous_color_manual) {
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);
            }
        }

        if (contour_text_) {
            for (size_t i = 0; i < lines_.size(); ++i) {
                if (lines_[i].first.empty()) {
                    continue;
                }
                ss << ", '-' with labels ";
                if (!filled_ && iequals(font_weight_, "normal")) {
                    ss << " boxed ";
                }
                ss << " textcolor '" << to_string(font_color_) << "' ";
                ss << " font '" << font() << "," << font_size() << "' ";
                ss << " rotate variable ";
            }
        }
        return ss.str();
    }

    void contours::make_sure_data_is_preprocessed() {
        if (levels_.empty() || n_levels_ == 0 || layers_.empty()) {
            process_contour_levels();
        }
        if (lines_.empty()) {
            process_all_segs_and_all_kinds();
        }
    }

    void contours::clear_preprocessed_data() {
        layers_.clear();
        if (!manual_n_levels_) {
            n_levels_ = 0;
        }
        if (!manual_levels_) {
            levels_.clear();
        }
        lines_.clear();
        codes_.clear();
    }

    std::vector<double> contours::determine_contour_levels(double z_min,
                                                           double z_max,
                                                           size_t n_levels_,
                                                           extend_option ext) {
        // generate levels with bin picker and remove levels out of range
        // we start trying to generate n + 2 levels to account for the ones
        // we will usually remove
        size_t target_n_edges = n_levels_ + 2;
        std::vector<double> levels = histogram::bin_picker(
            z_min, z_max, target_n_edges, (z_max - z_min) / (target_n_edges));
        auto remove_levels_out_of_range = [&](std::vector<double> &levels) {
            while (!levels.empty() && levels.front() < z_min) {
                levels.erase(levels.begin());
            }
            while (!levels.empty() && levels.back() > z_max) {
                levels.pop_back();
            }
        };
        remove_levels_out_of_range(levels);

        // check how many levels we have left and if we need to include level =
        // 0
        bool zero_is_in_the_range = z_min <= 0.0 && z_max >= 0;
        bool missing_zero =
            zero_is_in_the_range &&
            std::find(levels.begin(), levels.end(), 0.) == levels.end();
        size_t excess =
            levels.size() > n_levels_ ? levels.size() - n_levels_ : 0;
        if (missing_zero || excess) {
            // next target has one less because we will insert 0.0 in the range
            target_n_edges -= missing_zero;
            // next target has excess less
            target_n_edges -= excess;
            levels = histogram::bin_picker(z_min, z_max, target_n_edges,
                                           (z_max - z_min) / (target_n_edges));
            remove_levels_out_of_range(levels);
            missing_zero =
                zero_is_in_the_range &&
                std::find(levels.begin(), levels.end(), 0.) == levels.end();
            if (missing_zero) {
                auto it = std::lower_bound(levels.begin(), levels.end(), 0.);
                levels.insert(it, 0.);
            }
        }

        // If we are going to extend later, we remove the extreme
        // levels because we are going to include more extreme levels
        // later.
        if (ext == extend_option::min || ext == extend_option::both) {
            levels.erase(levels.begin());
        }
        if (ext == extend_option::max || ext == extend_option::both) {
            levels.pop_back();
        }

        if (levels.size() < 3) {
            levels = histogram::bin_picker(z_min, z_max, target_n_edges,
                                           (z_max - z_min) / (target_n_edges));
            remove_levels_out_of_range(levels);
            if (missing_zero) {
                auto it = std::lower_bound(levels.begin(), levels.end(), 0.);
                levels.insert(it, 0.);
            }
        }

        return levels;
    }

    std::vector<double> contours::determine_contour_levels(const vector_2d &Z,
                                                           size_t n_levels_,
                                                           extend_option ext) {
        double zmin_ = Z[0][0];
        double zmax_ = Z[0][0];
        for (const auto &row : Z) {
            auto [row_min_it, row_max_it] =
                std::minmax_element(row.begin(), row.end());
            zmin_ = std::min(zmin_, *row_min_it);
            zmax_ = std::max(zmax_, *row_max_it);
        }
        return determine_contour_levels(zmin_, zmax_, n_levels_, ext);
    }

    void contours::process_contour_levels() {
        // Set automatic numer of  levels and levels
        if (levels_.empty()) {
            if (n_levels_ == 0) {
                n_levels_ = !filled_ ? 7 : 10;
            }
            // Something to do: Find a nicer algorithm (simple) for that later
            // The same algorithm can be used to find ticks
            levels_ =
                determine_contour_levels(zmin(), zmax(), n_levels_, extend_);
        } else {
            if (n_levels_ == 0) {
                n_levels_ = levels_.size();
            }
        }

        // We need at least one level in the valid range
        // If this is a filled contour, this is ok
        if (!filled_) {
            bool any_inside = false;
            for (size_t i = 0; i < levels_.size(); ++i) {
                if (levels_[i] > zmin() && levels_[i] < zmax()) {
                    any_inside = true;
                    break;
                }
            }
            if (!any_inside) {
                levels_ = {zmin()};
            }
        }

        if (filled_) {
            if ((extend_ == extend_option::neither && levels_.size() < 2) ||
                levels_.empty()) {
                throw std::logic_error(
                    "Filled contours require at least 2 levels");
            }
        }

        if (levels_.size() > 1) {
            for (size_t i = 0; i < levels_.size() - 1; ++i) {
                if (levels_[i + 1] - levels_[i] <= 0.) {
                    throw std::logic_error("Contour levels must be increasing");
                }
            }
        }

        // Make a private _levels to include extended regions; we
        // want to leave the original levels attribute unchanged.
        // The plotting algorithm will use _levels only.
        // (Colorbar needs this even for line contours.)
        _levels = levels_;

        // Extend minimum beyond zmin (for filled plots)
        const bool log =
            parent_->z_axis().scale() == axis_type::axis_scale::log;
        if (extend_ == extend_option::both || extend_ == extend_option::min) {
            double lower = log ? 1e-250 : -1e250;
            _levels.insert(_levels.begin(), lower);
        }

        // Extend maximum beyond zmax (for filled plots)
        if (extend_ == extend_option::both || extend_ == extend_option::max) {
            double upper = 1e250;
            _levels.emplace_back(upper);
        }

        // Layers == levels if we are only plotting lines
        if (!filled_) {
            layers_ = levels_;
            return;
        }

        // Layer values are mid-way between levels in screen space
        if (log) {
            // Avoid overflow by taking sqrt before multiplying.
            layers_.resize(_levels.size() - 1);
            for (size_t i = 0; i < _levels.size() - 1; ++i) {
                layers_[i] = sqrt(_levels[i]) * sqrt(_levels[i + 1]);
            }
        } else {
            layers_.resize(_levels.size() - 1);
            for (size_t i = 0; i < _levels.size() - 1; ++i) {
                layers_[i] = 0.5 * (_levels[i] + _levels[i + 1]);
            }
        }
    }

    double contours::zmin() { return zmin_; }

    double contours::zmax() { return zmax_; }

    std::pair<vector_1d, vector_1d> contours::get_lowers_and_uppers() {
        vector_1d lowers(_levels.begin(), _levels.end() - 1);
        if (lowers.empty()) {
            return std::pair<vector_1d, vector_1d>{{}, {}};
        }
        if (zmin() == lowers[0]) {
            // Include minimum values in lowest interval
            // so we don't change levels_
            if (parent_->z_axis().scale() == axis_type::axis_scale::log) {
                lowers[0] = 0.99 * zmin();
            } else {
                lowers[0] -= 1;
            }
        }
        vector_1d uppers(_levels.begin() + 1, _levels.end());
        return std::make_pair(lowers, uppers);
    }

    std::string contours::legend_string(std::string_view title) {
        auto [min_level_it, max_level_it] =
            std::minmax_element(levels_.begin(), levels_.end());
        double zmax = *max_level_it;
        double zmin = *min_level_it;

        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        for (size_t i = 0; i < lines_.size(); ++i) {
            if (i != 0) {
                ss << ",";
            }
            auto previous_color = line_spec_.color();
            bool previous_color_manual = line_spec_.user_color();
            if (!previous_color_manual) {
                line_spec_.color(parent_->colormap_interpolation(
                    (zmax - zmin) - (levels_[i] - zmin), 0., zmax - zmin));
            }
            ss << " keyentry "
               << line_spec_.plot_string(
                      line_spec::style_to_plot::plot_line_only)
               << " title \"" << escape(title) << "\" ";
            if (!previous_color_manual) {
                line_spec_.color(previous_color);
                line_spec_.user_color(previous_color_manual);
            }
        }
        return ss.str();
    }

    /// If a line is lower (true) or upper level (false)
    /// We have to know if it's lower or upper level
    /// to decide its color.
    bool contours::is_lower_level(size_t line_index, size_t segment_begin,
                                  size_t segment_end) {
        // The parent non-hole is not always the lower or upper level.
        // That depends on whether the function is increasing
        // or decreasing on that region.
        // lower level <-> higher values on the left
        // upper level <-> higher values on the right

        // Find limits
        double _xmax = xmax();
        double _xmin = xmin();
        double _ymax = ymax();
        double _ymin = ymin();

        // Take two points - outside the border when possible
        double x1 = filled_lines_[line_index].first[segment_begin];
        double x2 = filled_lines_[line_index].first[segment_begin + 1];
        double y1 = filled_lines_[line_index].second[segment_begin];
        double y2 = filled_lines_[line_index].second[segment_begin + 1];
        auto is_on_border = [&]() {
            return (x1 <= _xmin || x1 >= _xmax || x2 <= _xmin || x2 >= _xmax ||
                    y1 <= _ymin || y1 >= _ymax || y2 <= _ymin || y2 >= _ymax);
        };
        size_t sample_begin = segment_begin;
        while (is_on_border() && sample_begin < segment_end - 1) {
            ++sample_begin;
            x1 = filled_lines_[line_index].first[sample_begin];
            x2 = filled_lines_[line_index].first[sample_begin + 1];
            y1 = filled_lines_[line_index].second[sample_begin];
            y2 = filled_lines_[line_index].second[sample_begin + 1];
        }
        double avg_x = 0.5 * (x1 + x2);
        double avg_y = 0.5 * (y1 + y2);
        bool x_is_increasing = x2 > x1;
        bool y_is_increasing = y2 > y1;

        // look for the grid position of (x > x1, y > y1) - NE
        auto it_y =
            std::find_if(Y_data_.begin(), Y_data_.end(),
                         [&](const auto &y_row) { return y_row[0] > avg_y; });
        auto it_x = std::find_if(
            X_data_[0].begin(), X_data_[0].end(),
            [&](const double &x_row_value) { return x_row_value > avg_x; });
        size_t n_row = it_y - Y_data_.begin();
        size_t n_col = it_x - X_data_[0].begin();

        // look at the left
        // If x is increasing, the left is in the north
        // - Do nothing because we are already at a position where y > avg_y
        // If x is not increasing, the left is in the south
        // - Try to reduce the n_row (our current grid position is NE)
        if (!x_is_increasing && n_row > 0) {
            n_row--;
        }
        // If y is increasing, the left is in the west
        // - Try to reduce the n_col (our current grid position is NE)
        // If y is not increasing, the left is in the east
        // - Do nothing because we are already at a position where x > avg_x
        if (y_is_increasing && n_col > 0) {
            n_col--;
        }

        // Find the opposite grid position to compare whether higher
        // values are on the left or on the right
        // The logic is the same
        size_t opposite_row = n_row;
        size_t opposite_col = n_col;

        if (x_is_increasing && opposite_row > 0) {
            opposite_row--;
        } else if (!x_is_increasing && (opposite_row < Y_data_.size() - 1)) {
            opposite_row++;
        }

        if (y_is_increasing && opposite_col < X_data_[0].size() - 1) {
            opposite_col++;
        } else if (!y_is_increasing && opposite_col > 0) {
            opposite_col--;
        }

        // if it increases
        if (Z_data_.size() < n_row + 1 || Z_data_[n_row].size() < n_col + 1 ||
            Z_data_.size() < opposite_row + 1 ||
            Z_data_[opposite_row].size() < opposite_col + 1) {
            // if we are outside the grid, we don't know
            // if it's lower or upper level
            return false;
        }
        bool higher_values_on_left =
            Z_data_[n_row][n_col] > Z_data_[opposite_row][opposite_col];
        if (higher_values_on_left) {
            // lower level
            return true;
        } else {
            // else, upper level
            return false;
        }
    }

    /// Generate points to close a polygon if not closed yet
    ///
    /// Some polygons might not be closed yet.
    /// Large polygons that end on borders might not be going around
    /// the borders properly as it should. That's fine for lines
    /// but it's not fine for filled curves.
    /// In these cases, we have to sanitize and continue clockwise or
    /// anticlockwise around the borders until we reach the initial point.
    ///
    /// If they start or end outside the borders, there is nothing
    /// to do because we don't know the appropriate path to close
    /// the polygon. For this reason, these interior areas are
    /// already closed.
    ///
    /// If they start and end at the borders, as we know it is
    /// going clockwise, we know how to close it because we can
    /// go around the borders. In these cases, the contourc
    /// algorithm does not close the polygons for us.
    std::pair<vector_1d, vector_1d>
    contours::fill_border_jump(double start_x, double start_y, double end_x,
                               double end_y, double x_min, double x_max,
                               double y_min, double y_max, bool is_parent) {
        // start_b = identify if starting border W,S,E,N
        constexpr uint8_t NONE = 0;
        constexpr uint8_t NORTH = 1;
        constexpr uint8_t SOUTH = 2;
        constexpr uint8_t WEST = 3;
        constexpr uint8_t EAST = 4;
        uint8_t xy1_border = NONE;
        if (start_x <= x_min) {
            xy1_border = WEST;
        } else if (start_x >= x_max) {
            xy1_border = EAST;
        } else if (start_y <= y_min) {
            xy1_border = SOUTH;
        } else if (start_y >= y_max) {
            xy1_border = NORTH;
        }
        if (xy1_border == NONE) {
            return {};
        }

        // end_b   = identify if closing border border W,S,E,N
        uint8_t xy2_border = NONE;
        if (end_x <= x_min) {
            xy2_border = WEST;
        } else if (end_x >= x_max) {
            xy2_border = EAST;
        } else if (end_y <= y_min) {
            xy2_border = SOUTH;
        } else if (end_y >= y_max) {
            xy2_border = NORTH;
        }
        if (xy2_border == NONE) {
            return {};
        }

        // parents go anticlockwise
        // we need to know the direction we should use to fill the border
        const bool clockwise = !is_parent;

        std::pair<vector_1d, vector_1d> result;
        if (clockwise) {
            while (xy1_border != xy2_border) {
                switch (xy1_border) {
                case WEST:
                    // append NW
                    result.first.emplace_back(x_min);
                    result.second.emplace_back(y_max);
                    xy1_border = NORTH;
                    break;
                case NORTH:
                    // append NE
                    result.first.emplace_back(x_max);
                    result.second.emplace_back(y_max);
                    xy1_border = EAST;
                    break;
                case EAST:
                    // append SE
                    result.first.emplace_back(x_max);
                    result.second.emplace_back(y_min);
                    xy1_border = SOUTH;
                    break;
                case SOUTH:
                    // append SW
                    result.first.emplace_back(x_min);
                    result.second.emplace_back(y_min);
                    xy1_border = WEST;
                    break;
                default:
                    throw std::logic_error("Invalid direction");
                }
            }
        } else {
            // anticlockwise
            while (xy1_border != xy2_border) {
                switch (xy1_border) {
                case WEST:
                    // append SW
                    result.first.emplace_back(x_min);
                    result.second.emplace_back(y_min);
                    xy1_border = SOUTH;
                    break;
                case SOUTH:
                    // append SE
                    result.first.emplace_back(x_max);
                    result.second.emplace_back(y_min);
                    xy1_border = EAST;
                    break;
                case EAST:
                    // append NE
                    result.first.emplace_back(x_max);
                    result.second.emplace_back(y_max);
                    xy1_border = NORTH;
                    break;
                case NORTH:
                    // append NW
                    result.first.emplace_back(x_min);
                    result.second.emplace_back(y_max);
                    xy1_border = WEST;
                    break;
                default:
                    throw std::logic_error("Invalid direction");
                }
            }
        }
        return result;
    }

    std::string contours::data_string() {
        // If there is a jump from a border to the other, we need to complete
        // the curve from one point to another to avoid filled curves that don't
        // make sense.
        double _xmax = xmax();
        double _xmin = xmin();
        double _ymax = ymax();
        double _ymin = ymin();
        auto is_border_jump = [&](double x1, double y1, double x2, double y2) {
            constexpr uint8_t NONE = 0;
            constexpr uint8_t NORTH = 1;
            constexpr uint8_t SOUTH = 2;
            constexpr uint8_t WEST = 3;
            constexpr uint8_t EAST = 4;
            uint8_t xy1_border = NONE;
            if (x1 <= _xmin) {
                xy1_border = WEST;
            } else if (x1 >= _xmax) {
                xy1_border = EAST;
            } else if (y1 <= _ymin) {
                xy1_border = SOUTH;
            } else if (y1 >= _ymax) {
                xy1_border = NORTH;
            }
            if (xy1_border != NONE) {
                uint8_t xy2_border = NONE;
                if (x2 <= _xmin) {
                    xy2_border = WEST;
                } else if (x2 >= _xmax) {
                    xy2_border = EAST;
                } else if (y2 <= _ymin) {
                    xy2_border = SOUTH;
                } else if (y2 >= _ymax) {
                    xy2_border = NORTH;
                }
                if (xy2_border != NONE && xy2_border != xy1_border) {
                    return true;
                }
            }
            return false;
        };

        auto [lower_levels, upper_levels] = get_lowers_and_uppers();

        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        if (filled_) {
            // Plot the line segments
            // Create one filled curve for each segment of a contour.

            bool plot_background = _levels[0] < zmin_;
            if (plot_background) {
                // find background polygon level
                auto &largest_segment_with_children = line_segments_[0];
                auto &largest_segment =
                    std::get<0>(largest_segment_with_children);
                size_t line_index = std::get<0>(largest_segment);
                size_t level_index = line_index > 0 ? line_index - 1 : 0;
                size_t segment_begin = std::get<1>(largest_segment);
                size_t segment_end = std::get<2>(largest_segment);
                bool parent_is_lower_level =
                    is_lower_level(line_index, segment_begin, segment_end);
                double background_z_level = parent_is_lower_level
                                                ? lower_levels[level_index]
                                                : upper_levels[level_index];
                // Plot background polygon
                ss << "    " << _xmin << "  " << _ymin << "  "
                   << background_z_level << "\n";
                ss << "    " << _xmin << "  " << _ymax << "  "
                   << background_z_level << "\n";
                ss << "    " << _xmax << "  " << _ymax << "  "
                   << background_z_level << "\n";
                ss << "    " << _xmax << "  " << _ymin << "  "
                   << background_z_level << "\n";
                ss << "    " << _xmin << "  " << _ymin << "  "
                   << background_z_level << "\n";
                ss << "    e\n";
            }

            for (size_t i = 0; i < line_segments_.size(); ++i) {
                // Send data for parent polygon
                auto &parent_and_children = line_segments_[i];
                auto &parent_segment = std::get<0>(parent_and_children);
                size_t line_index = std::get<0>(parent_segment);
                size_t begin_index = std::get<1>(parent_segment);
                size_t end_index = std::get<2>(parent_segment);
                for (size_t j = begin_index; j < end_index; ++j) {
                    double x = filled_lines_[line_index].first[j];
                    double y = filled_lines_[line_index].second[j];
                    // z = palette value
                    bool is_ll =
                        is_lower_level(line_index, begin_index, end_index);
                    double segment_z_level = is_ll ? lower_levels[line_index]
                                                   : upper_levels[line_index];
                    ss << "    " << x << "  " << y << "  " << segment_z_level
                       << "\n";

                    // work-around for edge cases
                    bool is_one_before_last = j == end_index - 2;
                    if (is_one_before_last) {
                        double next_x = filled_lines_[line_index].first[j + 1];
                        double next_y = filled_lines_[line_index].second[j + 1];
                        bool is_jump_to_border =
                            (next_x <= _xmin) || (next_x >= _xmax) ||
                            (next_y <= _ymin) || (next_y >= _ymax);
                        if (is_jump_to_border) {
                            // Parents are supposed to go anticlockwise
                            // Last move cannot be to the left
                            bool going_anticlockwise = next_x < x;
                            if (going_anticlockwise) {
                                // go to closest border and fill the path
                                std::array<double, 4> border_distances = {
                                    (_xmax - x), (x - _xmin), (_ymax - y),
                                    (y - _ymin)};
                                auto it =
                                    std::min_element(border_distances.begin(),
                                                     border_distances.end());
                                size_t index_closest =
                                    it - border_distances.begin();
                                if (index_closest == 0) {
                                    x = _xmax;
                                } else if (index_closest == 1) {
                                    x = _xmin;
                                } else if (index_closest == 2) {
                                    y = _ymax;
                                } else if (index_closest == 3) {
                                    y = _ymin;
                                }
                                ss << "    " << x << "  " << y << "  "
                                   << segment_z_level << "\n";
                                auto [xs, ys] = fill_border_jump(
                                    x, y, next_x, next_y, _xmin, _xmax, _ymin,
                                    _ymax, true);
                                for (size_t k = 0; k < xs.size(); ++k) {
                                    ss << "    " << xs[k] << "  " << ys[k]
                                       << "  " << segment_z_level << "\n";
                                }
                            }
                        }
                    }
                }
                ss << "    e\n";
                // Send data for child/hole polygons
                auto &child_segments = std::get<1>(parent_and_children);
                for (size_t j = 0; j < child_segments.size(); ++j) {
                    auto &child_segment = child_segments[j];
                    size_t child_line_index = std::get<0>(child_segment);
                    size_t child_begin_index = std::get<1>(child_segment);
                    size_t child_end_index = std::get<2>(child_segment);
                    for (size_t k = child_begin_index; k < child_end_index;
                         ++k) {
                        double x = filled_lines_[child_line_index].first[k];
                        double y = filled_lines_[child_line_index].second[k];
                        bool is_ll =
                            is_lower_level(child_line_index, child_begin_index,
                                           child_end_index);
                        double segment_z_level =
                            is_ll ? lower_levels[child_line_index]
                                  : upper_levels[child_line_index];
                        ss << "    " << x << "  " << y << "  "
                           << segment_z_level << "\n";
                        if (k != child_end_index - 1) {
                            double next_x =
                                filled_lines_[child_line_index].first[k + 1];
                            double next_y =
                                filled_lines_[child_line_index].second[k + 1];
                            if (is_border_jump(x, y, next_x, next_y)) {
                                auto [xs, ys] = fill_border_jump(
                                    x, y, next_x, next_y, _xmin, _xmax, _ymin,
                                    _ymax, true);
                                for (size_t l = 0; l < xs.size(); ++l) {
                                    ss << "    " << xs[l] << "  " << ys[l]
                                       << "  " << segment_z_level << "\n";
                                }
                            }
                        }
                    }
                    ss << "    e\n";
                }
            }

            // check if there are nans to hide
            bool z_has_nans = false;
            for (size_t i = 0; !z_has_nans && i < Z_data_.size(); ++i) {
                for (size_t j = 0; !z_has_nans && j < Z_data_[i].size(); ++j) {
                    if (!std::isfinite(Z_data_[i][j])) {
                        z_has_nans = true;
                    }
                }
            }
            if (z_has_nans) {
                std::vector<size_t> nan_columns;
                std::vector<size_t> nan_lines;
                // for each line
                for (size_t i = 0; i < Z_data_.size(); ++i) {
                    bool all_nan = true;
                    for (size_t j = 0; j < Z_data_[i].size(); ++j) {
                        if (std::isfinite(Z_data_[i][j])) {
                            all_nan = false;
                            break;
                        }
                    }
                    if (all_nan) {
                        nan_lines.emplace_back(i);
                    }
                }
                // for each column
                for (size_t i = 0; i < Z_data_[0].size(); ++i) {
                    bool all_nan = true;
                    for (size_t j = 0; j < Z_data_.size(); ++j) {
                        if (std::isfinite(Z_data_[j][i])) {
                            all_nan = false;
                            break;
                        }
                    }
                    if (all_nan) {
                        nan_columns.emplace_back(i);
                    }
                }
                // draw polygons hiding these nans
                for (size_t i = 0; i < nan_lines.size(); ++i) {
                    size_t first_line = nan_lines[i];
                    size_t end_line = nan_lines[i];
                    // find contiguous elements
                    for (size_t j = i + 1; j < nan_lines.size(); ++j) {
                        if (nan_lines[j] == end_line + 1) {
                            end_line = j;
                        } else {
                            break;
                        }
                    }
                    // plot a square hiding lines [first_line, end_line]
                    double ybegin = Y_data_[first_line][0];
                    double yend = end_line < Y_data_.size() - 1
                                      ? Y_data_[end_line + 1][0]
                                      : _ymax;
                    ss << "    " << _xmin << "  " << ybegin << "\n";
                    ss << "    " << _xmin << "  " << yend << "\n";
                    ss << "    " << _xmax << "  " << yend << "\n";
                    ss << "    " << _xmax << "  " << ybegin << "\n";
                    ss << "    " << _xmin << "  " << ybegin << "\n";
                    // jump the lines we are plotting already
                    i = end_line;
                }

                for (size_t i = 0; i < nan_columns.size(); ++i) {
                    size_t first_col = nan_columns[i];
                    size_t end_col = nan_columns[i];
                    // find contiguous elements
                    for (size_t j = i + 1; j < nan_columns.size(); ++j) {
                        if (nan_columns[j] == end_col + 1) {
                            end_col = j;
                        } else {
                            break;
                        }
                    }
                    // plot a square hiding cols [first_col, end_col]
                    double xbegin = X_data_[0][first_col];
                    double xend = end_col < X_data_[0].size() - 1
                                      ? X_data_[0][end_col + 1]
                                      : _xmax;
                    ss << "    " << xbegin << "  " << _ymin << "\n";
                    ss << "    " << xbegin << "  " << _ymax << "\n";
                    ss << "    " << xend << "  " << _ymax << "\n";
                    ss << "    " << xend << "  " << _ymin << "\n";
                    ss << "    " << xbegin << "  " << _ymin << "\n";
                    // jump the lines we are plotting already
                    i = end_col;
                }
                ss << "    e\n";
            }
        }

        // Plot the lines
        // For the lines, we don't need to plot the segments separately
        // One line is separated from the other with NaNs
        auto is_separator = [](double x, double y) {
            return !std::isfinite(x) || !std::isfinite(y);
        };

        for (size_t i = 0; i < lines_.size(); ++i) {
            if (lines_[i].first.empty()) {
                continue;
            }
            for (size_t j = 0; j < lines_[i].first.size(); ++j) {
                if (!is_separator(lines_[i].first[j], lines_[i].second[j])) {
                    double x = lines_[i].first[j];
                    double y = lines_[i].second[j];
                    ss << "    " << x << "  " << y;
                    if (!line_spec_.user_color()) {
                        if (!filled_ || colormap_line_when_filled_) {
                            // plot palette colors
                            ss << "  " << levels_[i];
                        }
                    }
                    ss << "\n";
                } else {
                    // Skip the next nans separating parents and children
                    // to avoid extra useless empty lines in case there is
                    // more than one nan (with out-of-bounds check if we
                    // have a corner case then the lines_ has one or more
                    // nans at end of the underlying storage)
                    while (j + 1 < lines_[i].first.size() &&
                           is_separator(lines_[i].first[j + 1],
                                        lines_[i].second[j + 1])) {
                        ++j;
                    }
                    // Include an empty line to indicate this polygon or line
                    // segment is over
                    ss << "  \n";
                    continue;
                }
            }
            ss << "e\n";
        }

        if (contour_text_) {
            // We always check if a label is not too close to another label
            static constexpr double minimum_distance = 0.8;
            auto too_close = [](double x1, double y1, double x2, double y2) {
                return std::abs(x1 - x2) < minimum_distance &&
                       std::abs(y1 - y2) < minimum_distance;
            };

            // Instead of iterating each line, we iterate
            // each line a little bit. The rationale is that
            // we might put some labels in line i and there
            // is no room left for more labels in line i+1
            // with the minimum distance.
            // The current index in each line
            std::vector<size_t> point_index(lines_.size(), 0);
            std::vector<size_t> n_points(lines_.size());
            for (size_t i = 0; i < lines_.size(); ++i) {
                n_points[i] = lines_[i].first.size();
            }

            // Label positions for each line
            std::vector<std::vector<double>> labels_xs(lines_.size());
            std::vector<std::vector<double>> labels_ys(lines_.size());
            std::vector<std::vector<double>> labels_degrees(lines_.size());

            // Vectors with line direction for each line
            std::vector<double> direction_us(lines_.size(), 0.0);
            std::vector<double> direction_vs(lines_.size(), 0.0);

            // while we have not iterated all points in all lines
            while (point_index != n_points) {
                // iterate each line until we put a label on that line or the
                // line is over
                for (size_t i = 0; i < lines_.size(); ++i) {
                    if (lines_[i].first.empty()) {
                        continue;
                    }
                    // references to line directions
                    double &direction_u = direction_us[i];
                    double &direction_v = direction_vs[i];

                    // for each point in that line (starting from where we
                    // stopped)
                    for (; point_index[i] < lines_[i].first.size();
                         ++point_index[i]) {
                        size_t j = point_index[i];
                        if (is_separator(lines_[i].first[j],
                                         lines_[i].second[j])) {
                            // reset line direction
                            direction_u = 0.0;
                            direction_v = 0.0;
                            continue;
                        }

                        // check if not too close to another label
                        const double x = lines_[i].first[j];
                        const double y = lines_[i].second[j];
                        bool too_close_to_another_label = false;
                        for (size_t k = 0; !too_close_to_another_label &&
                                           k < labels_xs.size();
                             ++k) {
                            for (size_t l = 0; !too_close_to_another_label &&
                                               l < labels_xs[k].size();
                                 ++l) {
                                if (too_close(x, y, labels_xs[k][l],
                                              labels_ys[k][l])) {
                                    too_close_to_another_label = true;
                                }
                            }
                        }

                        // Update vector of line direction
                        constexpr double moving_average_reduction_factor = 0.1;
                        direction_u *= (1 - moving_average_reduction_factor);
                        direction_v *= (1 - moving_average_reduction_factor);
                        bool has_previous =
                            j != 0 && std::isfinite(lines_[i].first[j - 1]);
                        if (has_previous) {
                            direction_u +=
                                lines_[i].first[j] - lines_[i].first[j - 1];
                            direction_v +=
                                lines_[i].second[j] - lines_[i].second[j - 1];
                        } else {
                            bool has_next =
                                j != lines_[i].first.size() - 1 &&
                                std::isfinite(lines_[i].first[j + 1]);
                            if (has_next) {
                                direction_u +=
                                    lines_[i].first[j + 1] - lines_[i].first[j];
                                direction_v += lines_[i].second[j + 1] -
                                               lines_[i].second[j];
                            }
                        }

                        // insert label if we can
                        if (!too_close_to_another_label) {
                            labels_xs[i].emplace_back(x);
                            labels_ys[i].emplace_back(y);
                            double rho =
                                vector_radians(direction_u, direction_v);
                            double degrees = deg2rad(rho);
                            constexpr double min_degree = -90;
                            constexpr double max_degree = +90;
                            while (degrees < min_degree) {
                                degrees += 180;
                            }
                            while (degrees > max_degree) {
                                degrees -= 180;
                            }
                            labels_degrees[i].emplace_back(degrees);
                            // if we inserted a label, give a chance to another
                            // line
                            break;
                        }
                    }
                }
            }
            // Send command for all labels at once
            for (size_t i = 0; i < labels_xs.size(); ++i) {
                for (size_t j = 0; j < labels_xs[i].size(); ++j) {
                    ss << "    " << labels_xs[i][j] << "  " << labels_ys[i][j]
                       << " \"";
                    if (iequals(font_weight_, "bold")) {
                        ss << "{/:Bold " << _levels[i] << "}";
                    } else {
                        ss << levels_[i];
                    }
                    ss << "\"";
                    ss << " " << labels_degrees[i][j];
                    ss << "\n";
                }
                if (labels_xs[i].empty()) {
                    ss << "\n";
                }
                ss << "e\n";
            }
        }

        return ss.str();
    }

    bool contours::requires_colormap() { return true; }

    double contours::xmax() {
        double m = X_data_[0][0];
        for (size_t i = 0; i < X_data_.size(); ++i) {
            for (size_t j = 0; j < X_data_[i].size(); ++j) {
                m = std::max(m, X_data_[i][j]);
            }
        }
        return m;
    }

    double contours::xmin() {
        double m = X_data_[0][0];
        for (size_t i = 0; i < X_data_.size(); ++i) {
            for (size_t j = 0; j < X_data_[i].size(); ++j) {
                m = std::min(m, X_data_[i][j]);
            }
        }
        return m;
    }

    double contours::ymax() {
        double m = Y_data_[0][0];
        for (size_t i = 0; i < Y_data_.size(); ++i) {
            for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                m = std::max(m, Y_data_[i][j]);
            }
        }
        return m;
    }

    double contours::ymin() {
        double m = Y_data_[0][0];
        for (size_t i = 0; i < Y_data_.size(); ++i) {
            for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                m = std::min(m, Y_data_[i][j]);
            }
        }
        return m;
    }

    enum axes_object::axes_category contours::axes_category() {
        return axes_object::axes_category::two_dimensional;
    }

    class contours &contours::line_style(std::string_view str) {
        line_spec_.parse_string(str);
        touch();
        return *this;
    }

    const line_spec &contours::line_spec() const { return line_spec_; }

    line_spec &contours::line_spec() { return line_spec_; }

    class contours &contours::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const vector_2d &contours::Y_data() const { return Y_data_; }

    class contours &contours::Y_data(const vector_2d &Y_data) {
        Y_data_ = Y_data;
        touch();
        return *this;
    }

    const vector_2d &contours::X_data() const { return X_data_; }

    class contours &contours::X_data(const vector_2d &X_data) {
        X_data_ = X_data;
        touch();
        return *this;
    }

    const vector_2d &contours::Z_data() const { return Z_data_; }

    class contours &contours::Z_data(const vector_2d &Z_data) {
        Z_data_ = Z_data;
        touch();
        return *this;
    }

    float contours::line_width() const { return line_spec().line_width(); }

    class contours &contours::line_width(float line_width) {
        line_spec().line_width(line_width);
        return *this;
    }

    const std::array<float, 4> &contours::color() const {
        return line_spec().color();
    }

    const std::vector<vector_1d> &contours::x_data() const { return X_data_; }

    class contours &contours::x_data(const std::vector<vector_1d> &x_data) {
        X_data_ = x_data;
        touch();
        return *this;
    }

    const std::vector<vector_1d> &contours::y_data() const { return Y_data_; }

    class contours &contours::y_data(const std::vector<vector_1d> &y_data) {
        Y_data_ = y_data;
        touch();
        return *this;
    }

    const std::vector<vector_1d> &contours::z_data() const { return Z_data_; }

    class contours &contours::z_data(const std::vector<vector_1d> &z_data) {
        Z_data_ = z_data;
        touch();
        return *this;
    }

    bool contours::contour_text() const { return contour_text_; }

    class contours &contours::contour_text(bool contour_text) {
        contour_text_ = contour_text;
        touch();
        return *this;
    }

    float contours::font_size() const {
        if (font_size_) {
            return *font_size_;
        } else {
            return parent_->font_size();
        }
    }

    class contours &contours::font_size(const float &font_size) {
        font_size_ = font_size;
        touch();
        return *this;
    }

    const std::string contours::font() const {
        if (font_) {
            return *font_;
        } else {
            return parent_->font();
        }
    }

    class contours &contours::font(std::string_view font) {
        font_ = font;
        touch();
        return *this;
    }

    const std::string &contours::font_weight() const { return font_weight_; }

    class contours &contours::font_weight(std::string_view font_weight) {
        font_weight_ = font_weight;
        touch();
        return *this;
    }

    const color_array &contours::font_color() const { return font_color_; }

    class contours &contours::font_color(const color_array &font_color) {
        font_color_ = font_color;
        touch();
        return *this;
    }

    class contours &contours::font_color(std::string_view fc) {
        font_color(to_array(fc));
        return *this;
    }

    size_t contours::n_levels() const { return n_levels_; }

    class contours &contours::n_levels(size_t n_levels) {
        n_levels_ = n_levels;
        manual_n_levels_ = n_levels != 0;
        clear_preprocessed_data();
        touch();
        return *this;
    }

    const std::vector<double> &contours::levels() const { return levels_; }

    class contours &contours::levels(const std::vector<double> &levels) {
        levels_ = levels;
        if (!std::is_sorted(levels_.begin(), levels_.end())) {
            std::sort(levels_.begin(), levels_.end());
        }
        auto it = std::unique(levels_.begin(), levels_.end());
        levels_.resize(std::distance(levels_.begin(), it));
        manual_levels_ = !levels.empty();
        if (manual_levels_) {
            extend_ = filled_ ? extend_option::max : extend_option::neither;
        } else {
            extend_ = filled_ ? extend_option::both : extend_option::neither;
        }
        clear_preprocessed_data();
        touch();
        return *this;
    }

    void square_trace(
        size_t start_i, size_t start_j, const vector_2d &Z, double level,
        std::unordered_set<std::pair<size_t, size_t>, pair_hash<size_t, size_t>>
            &quadrants_visited,
        std::vector<std::pair<size_t, size_t>> &boundary_quadrants) {
        // The starting point
        // We convert tot int because the square trace algorithm
        // might go outside the image
        auto start = std::make_pair(static_cast<int>(start_i),
                                    static_cast<int>(start_j));

        const size_t n_rows = Z.size();
        const size_t n_cols = Z[0].size();

        // We found at least one boundary pixel
        boundary_quadrants.emplace_back(start);

        // Our initial direction was going from left to right, hence (1, 0)
        std::pair<int, int> direction = std::make_pair(1, 0);

        // Try to apply direction to a position
        // We apply direction by i += y / j += x
        // We represent the position with int because the position might be
        // negative
        auto apply_direction = [](std::pair<int, int> position,
                                  const std::pair<int, int> &direction) {
            return std::make_pair<int, int>(position.first + direction.second,
                                            position.second + direction.first);
        };
        //        auto undo_direction = [](std::pair<int, int> position,
        //                                 const std::pair<int, int> &direction)
        //                                 {
        //            return std::make_pair<int, int>(position.first -
        //            direction.second,
        //                                            position.second -
        //                                            direction.first);
        //        };

        // Change direction
        // The quadrant rows go from n1-1 to 1.
        // The quadrant columns go from 0 to n2-1.
        auto turn_left = [](std::pair<int, int> direction) {
            return std::make_pair(-direction.second, direction.first);
        };
        auto turn_right = [](std::pair<int, int> direction) {
            return std::make_pair(direction.second, -direction.first);
        };

        // Check if two quadrants are neighbors
        auto are_neighbors = [](const auto &a, const auto &b) {
            size_t distance_x = std::max(static_cast<size_t>(a.second),
                                         static_cast<size_t>(b.second)) -
                                std::min(static_cast<size_t>(a.second),
                                         static_cast<size_t>(b.second));
            size_t distance_y = std::max(static_cast<size_t>(a.first),
                                         static_cast<size_t>(b.first)) -
                                std::min(static_cast<size_t>(a.first),
                                         static_cast<size_t>(b.first));
            return distance_x <= 1 && distance_y <= 1;
        };

        // Nan pair
        constexpr auto nan_pair =
            std::make_pair(std::numeric_limits<size_t>::quiet_NaN(),
                           std::numeric_limits<size_t>::quiet_NaN());

        // The first quadrant is an inflection point by definition, so we go
        // left.
        direction = turn_left(direction);
        auto current_quadrant = apply_direction(start, direction);
        while (current_quadrant != start) {
            // Check if current_quadrant point is inside the boundaries
            bool inside_boundaries =
                current_quadrant.first <= static_cast<int>(n_rows) - 1 &&
                current_quadrant.first >= 1 && current_quadrant.second >= 0 &&
                current_quadrant.second <= static_cast<int>(n_cols) - 2;

            // Check if point is an inflection point
            bool is_inflection_point = false;
            if (inside_boundaries) {
                // Mark points we visited
                quadrants_visited.insert(std::make_pair<size_t, size_t>(
                    static_cast<size_t>(current_quadrant.first),
                    static_cast<size_t>(current_quadrant.second)));

                // Get the quadrant indices
                size_t i = current_quadrant.first;
                size_t j = current_quadrant.second;

                // Create an abstract quadrant
                // nw -> z[i][j]     ne -> z[i][j+1]
                // sw-> z[i-1][j]    se -> z[i-1][j+1]
                const double nw = Z[i][j];
                const double ne = Z[i][j + 1];
                const double sw = Z[i - 1][j];
                const double se = Z[i - 1][j + 1];

                // Check if quadrant is an inflection point
                const bool all_higher =
                    nw > level && ne > level && sw > level && se > level;
                const bool all_lower =
                    nw < level && ne < level && sw < level && se < level;
                const bool all_equal =
                    nw == level && ne == level && sw == level && se == level;
                is_inflection_point = !all_higher && !all_lower && !all_equal;
            }

            if (is_inflection_point) {
                if (!are_neighbors(current_quadrant,
                                   boundary_quadrants.back())) {
                    // If not neighbor with the last, it means we are jumping
                    // This contour line has no area.
                    // So we stop the line here start over from this point
                    boundary_quadrants.emplace_back(nan_pair);
                    // find last neighbor
                    for (auto iter = boundary_quadrants.rbegin() + 1;
                         iter != boundary_quadrants.rend(); ++iter) {
                        if (*iter == nan_pair) {
                            break;
                        }
                        if (are_neighbors(*iter, current_quadrant)) {
                            // next line starts from the closest point
                            // to the current point
                            boundary_quadrants.emplace_back(*iter);
                            break;
                        }
                    }
                }

                boundary_quadrants.emplace_back(current_quadrant);
                direction = turn_left(direction);
                current_quadrant = apply_direction(current_quadrant, direction);
            } else {
                direction = turn_right(direction);
                current_quadrant = apply_direction(current_quadrant, direction);
            }
        }
        // Put starting point again to complete the circle
        auto last_quadrant = boundary_quadrants.back();
        if (are_neighbors(last_quadrant, start)) {
            boundary_quadrants.emplace_back(start);
        }
        // Put nan in the end to identify the limits of the boundary
        boundary_quadrants.emplace_back(nan_pair);
    }

    bool contours::filled() const { return filled_; }

    class contours &contours::filled(bool filled) {
        if (filled_ != filled) {
            filled_ = filled;
            if (manual_levels_) {
                extend_ = filled_ ? extend_option::max : extend_option::neither;
            } else {
                extend_ =
                    filled_ ? extend_option::both : extend_option::neither;
            }
            clear_preprocessed_data();
            touch();
        }
        return *this;
    }

    void contours::initialize_preprocessed_data() {
        if (X_data_.empty() || Y_data_.empty()) {
            initialize_x_y();
        } else {
            check_xyz();
        }
        zmin_ = Z_data_[0][0];
        zmax_ = Z_data_[0][0];
        for (size_t i = 0; i < Z_data_.size(); ++i) {
            auto [row_min, row_max] =
                std::minmax_element(Z_data_[i].begin(), Z_data_[i].end());
            if (*row_min < zmin_) {
                zmin_ = *row_min;
            }
            if (*row_max > zmax_) {
                zmax_ = *row_max;
            }
        }
    }

    /// Check that the shapes of the input arrays match; if x and y are 1D,
    //        convert them to 2D using meshgrid.
    void contours::check_xyz() {
        if (Z_data_.size() < 2 || Z_data_[0].size() < 2) {
            throw std::invalid_argument(
                "Input z must be at least a (2, 2) shaped array");
        }
        // const size_t Ny = Z_data_.size();
        // const size_t Nx = Z_data_[0].size();

        if (Z_data_.size() != X_data_.size() ||
            Z_data_[0].size() < X_data_[0].size()) {
            throw std::invalid_argument("Shapes of x and z do not match");
        }

        if (Z_data_.size() != Y_data_.size() ||
            Z_data_[0].size() < Y_data_[0].size()) {
            throw std::invalid_argument("Shapes of y and z do not match");
        }
    }

    void contours::initialize_x_y() {
        vector_1d x_1d = iota(1., static_cast<double>(Z_data_[0].size()));
        vector_1d y_1d = iota(1., static_cast<double>(Z_data_.size()));
        std::tie(X_data_, Y_data_) = meshgrid(x_1d, y_1d);
    }

    void contours::process_all_segs_and_all_kinds() {
        lines_.clear();
        codes_.clear();
        if (filled_) {
            auto [lowers, uppers] = get_lowers_and_uppers();
            for (size_t i = 0; i < lowers.size(); ++i) {
                double level = lowers[i];
                double level_upper = uppers[i];
                auto [vertices, kinds] =
                    contour_generator_.create_filled_contour(level,
                                                             level_upper);
                filled_lines_.emplace_back(vertices);
                codes_.emplace_back(kinds);
            }
            // Identify line segments
            line_segments_.clear();
            for (size_t i = 0; i < filled_lines_.size(); ++i) {
                parent_and_children_type cur;
                line_segment_type &cur_parent = std::get<0>(cur);
                line_segment_type cur_child;
                bool is_parent = true;
                std::get<0>(cur_parent) = i;
                std::get<1>(cur_parent) = 0;
                size_t j = 0;
                while (j < filled_lines_[i].first.size()) {
                    if (std::isfinite(filled_lines_[i].first[j])) {
                        ++j;
                    } else {
                        // Nan indicates and end of segment
                        if (is_parent) {
                            std::get<2>(cur_parent) = j;
                        } else {
                            std::get<2>(cur_child) = j;
                            std::get<1>(cur).emplace_back(cur_child);
                        }
                        // check for bounds before accessing value and
                        // break while loop if fails.
                        if (j + 1 >= filled_lines_[i].first.size()) {
                            break;
                        }
                        // Two nans in a row indicate a new parent
                        bool only_one_nan =
                            std::isfinite(filled_lines_[i].first[j + 1]);
                        if (only_one_nan) {
                            is_parent = false;
                            ++j;
                            // start new child
                            std::get<0>(cur_child) = i;
                            std::get<1>(cur_child) = j;
                        } else {
                            is_parent = true;
                            // emplace parent and its children
                            line_segments_.emplace_back(cur);
                            std::get<1>(cur).clear();
                            ++j;
                            while (j < filled_lines_[i].first.size() &&
                                   !std::isfinite(filled_lines_[i].first[j])) {
                                ++j;
                            }
                            // start new parent
                            std::get<0>(cur_parent) = i;
                            std::get<1>(cur_parent) = j;
                        }
                    }
                }
            }
            // calculate the bounding rectangle of each parent
            for (auto &parent_and_children : line_segments_) {
                line_segment_type &parent_segment =
                    std::get<0>(parent_and_children);
                size_t line_idx = std::get<0>(parent_segment);
                size_t start_idx = std::get<1>(parent_segment);
                size_t end_idx = std::get<2>(parent_segment);
                double ax_max = filled_lines_[line_idx].first[0];
                double ax_min = filled_lines_[line_idx].first[0];
                double ay_max = filled_lines_[line_idx].second[0];
                double ay_min = filled_lines_[line_idx].second[0];
                for (size_t i = start_idx; i < end_idx; ++i) {
                    double x = filled_lines_[line_idx].first[i];
                    double y = filled_lines_[line_idx].second[i];
                    if (x > ax_max) {
                        ax_max = x;
                    }
                    if (x < ax_min) {
                        ax_min = x;
                    }
                    if (y > ay_max) {
                        ay_max = y;
                    }
                    if (y < ay_min) {
                        ay_min = y;
                    }
                }
                double area = (ax_max - ax_min) * (ay_max - ay_min);
                std::get<2>(parent_and_children) = area;
            }
            // Sort line segments by their bounding rectangle areas
            // We plot the largest ones first because if A is inside B,
            // B's bounding rectangle will be larger. We plot B before A,
            // because the fillcolors of B cannot be in front of A, which
            // is more internal.
            std::sort(line_segments_.begin(), line_segments_.end(),
                      [&](const parent_and_children_type &a,
                          const parent_and_children_type &b) {
                          return std::get<2>(a) > std::get<2>(b);
                      });
        }
        // Generate normal lines
        // Do not use the extended levels we used for the filled lines
        for (const double &level : levels_) {
            auto vertices = contour_generator_.create_contour(level);
            lines_.emplace_back(vertices);
        }
    }

    bool contours::colormap_line_when_filled() const {
        return colormap_line_when_filled_;
    }

    class contours &
    contours::colormap_line_when_filled(bool colormap_line_when_filled) {
        colormap_line_when_filled_ = colormap_line_when_filled;
        touch();
        return *this;
    }

} // namespace matplot
