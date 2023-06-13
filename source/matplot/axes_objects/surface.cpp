//
// Created by Alan Freitas on 2020-07-07.
//

#include <cmath>
#include <matplot/axes_objects/surface.h>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <regex>
#include <sstream>

namespace matplot {
    surface::surface(class axes_type *parent) : axes_object(parent) {}

    surface::surface(class axes_type *parent, const vector_2d &X,
                     const vector_2d &Y, const vector_2d &Z, const vector_2d &C,
                     std::string_view line_spec)
        : axes_object(parent), X_data_(X), Y_data_(Y), Z_data_(Z), C_data_(C),
          is_parametric_(false), line_spec_(this, line_spec),
          contour_line_spec_(this, "") {
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

    //    surface::surface(class xlim* parent, const vector_1d& x, const
    //    vector_1d& y, const vector_1d& z, const vector_1d& c, const
    //    std::string& line_spec)
    //            : axes_object(parent), X_data_({x}), Y_data_({y}),
    //            Z_data_({z}), C_data_({c}), line_spec_(this, line_spec),
    //            contour_line_spec_(this, ""), is_parametric_(true) {
    //        auto [zmin_it, zmax_it] = std::minmax_element(z.begin(), z.end());
    //        zmin_ = *zmin_it;
    //        zmax_ = *zmax_it;
    //    }

    size_t surface::create_line_index() {
        auto it =
            std::find_if(parent_->children().begin(), parent_->children().end(),
                         [this](const auto &c) { return c.get() == this; });
        if (it != parent_->children().end()) {
            return 100 * (1 + it - parent_->children().begin());
        } else {
            std::cerr << "Cannot find surface in the parent xlim" << std::endl;
            return 100;
        }
    }

    std::string surface::set_variables_string() {
        maybe_update_line_spec();
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;

        if (surface_in_2d_) {
            ss << "    set view map\n";
        }

        // We used to create a line style for the surface
        // We now create the line style directly in the plot command
        // size_t line_index = create_line_index();
        // Line style for the surface
        //        if (line_spec_.user_color()) {
        //            ss << "    set style line " << line_index << " " <<
        //            line_spec_.plot_string(line_spec::style_to_plot::plot_line_only,
        //            false) << "\n";
        //        } else {
        //            ss << "    set style line " << line_index << " linecolor
        //            rgb \"black\" dashtype 1 linewidth 0.5 linetype -1\n";
        //        }

        // If there is any kind of palette surface being plotted besides the
        // lines if there is a pm3d surface, set the options
        const bool palette_map_3d = palette_map_at_bottom_ ||
                                    palette_map_at_surface_ ||
                                    palette_map_at_top_;
        if (palette_map_3d) {
            ss << "    set pm3d";
            ss << " noborder";
            // explicit: only show surface if we use with 3dpm
            ss << " explicit";

            // simple lighting
            if (lighting_) {
                ss << " lighting";
                if (primary_ >= 0) {
                    ss << " primary " << std::clamp(primary_, 0.f, 1.f);
                }
                if (specular_ >= 0) {
                    ss << " specular " << std::clamp(specular_, 0.f, 1.f);
                }
            }

            if (depthorder_) {
                // depthorder:
                ss << " depthorder base";
            }
            // hidden3d hides what's behind / makes the surface solid
            // - this makes the surface easier to visualize
            // - 1 is the linestyle for pm3d lines
            if (!fences_) {
                // We used to create a line style for the surface
                // We now create the line style directly in the plot command
                // ss << " hidden3d " << line_index;
                ss << " hidden3d ";
            }
            ss << "\n";
            if (face_alpha_ != 1.) {
                ss << "    set style fill transparent solid " << face_alpha_
                   << " noborder\n";
            }
        }

        // global hidden3d
        if (hidden3d_) {
            ss << "    set hidden3d\n";
        } else {
            // unset the global hidden3d
            ss << "    unset hidden3d\n";
        }

        // set dgrid3d
        constexpr bool use_gnuplots_grid3d = false;
        if (use_gnuplots_grid3d) {
            bool manual_color = size(C_data_) == size(Z_data_);
            if (!manual_color) {
                // dgrid ensures non-grid data is converted to grids
                // dgrid3d does not work with the forth color column
                ss << "    set dgrid3d " << Y_data_.size() << ","
                   << Y_data_[0].size() << " qnorm " << norm_ << "\n";
            }
        }

        // include contour
        const bool contour = (contour_base_ || contour_surface_);
        if (contour) {
            const bool explicit_contour_levels = !contour_values_.empty();
            if (explicit_contour_levels) {
                ss << "    set cntrparam levels discrete "
                   << contour_values_[0];
                for (size_t i = 1; i < contour_values_.size(); ++i) {
                    ss << "," << contour_values_[i];
                }
                ss << "\n";
            } else if (contour_levels_ != 0) {
                ss << "    set cntrparam levels " << contour_levels_ << "\n";
            }

            if (contour_base_ && contour_surface_) {
                ss << "    set contour both\n";
            } else if (contour_surface_) {
                ss << "    set contour surface\n";
            } else {
                ss << "    set contour base\n";
            }

            if (contour_text_) {
                ss << "    set style textbox opaque margins 0.5, 0.5 fc bgnd "
                      "noborder linewidth  1.0\n";
                ss << "    set cntrlabel format '";
                if (iequals(font_weight_, "bold")) {
                    ss << "{/:Bold %8.3g}";
                } else {
                    ss << "%8.3g";
                }
                ss << "' font '" << escape(font()) << "," << font_size()
                   << "' start 5 interval 10\n";
            }

            size_t n_contour_lines = explicit_contour_levels
                                         ? contour_values_.size()
                                         : contour_levels_;

            // line types for contour
            auto [min_level_it, max_level_it] = std::minmax_element(
                contour_values_.begin(), contour_values_.end());
            for (size_t i = 0; i < n_contour_lines; ++i) {
                // ss << "    set linetype " << i + line_index + 1;
                ss << "    set linetype " << i + 1;
                switch (contour_line_spec_.line_style()) {
                case line_spec::line_style::solid_line:
                    ss << " dashtype 1";
                    break;
                case line_spec::line_style::dashed_line:
                    ss << " dashtype '--'";
                    break;
                case line_spec::line_style::dotted_line:
                    ss << " dashtype '.'";
                    break;
                case line_spec::line_style::dash_dot_line:
                    ss << " dashtype '-.'";
                    break;
                default:
                    break;
                }

                // user set the color and it was not for the surface
                if (contour_line_spec_.user_color()) {
                    // use the user color for all contour lines
                    ss << " linecolor rgb '"
                       << to_string(contour_line_spec_.color()) << "'";
                } else {
                    // otherwise, use the colormap
                    if (contour_values_.empty()) {
                        // use the color map for contour lines
                        const bool not_using_palette =
                            line_spec_.user_color() &&
                            !palette_map_at_bottom_ && !palette_map_at_top_ &&
                            !palette_map_at_surface_;
                        const bool using_palette = !not_using_palette;
                        if (using_palette) {
                            ss << " linecolor palette";
                        } else {
                            ss << " linecolor rgb '"
                               << to_string(parent_->colormap_interpolation(
                                      static_cast<double>(i), 0.,
                                      static_cast<double>(n_contour_lines)))
                               << "'";
                        }
                    } else {
                        // if we know the values, get a more precise colormap
                        // interpolation
                        double zmax = *max_level_it;
                        double zmin = *min_level_it;
                        ss << " linecolor rgb '"
                           << to_string(parent_->colormap_interpolation(
                                  (zmax - zmin) - (contour_values_[i] - zmin),
                                  0., zmax - zmin))
                           << "'";
                    }
                }
                ss << " linewidth " << contour_line_spec_.line_width() << "\n";
            }
        }
        if (!surface_visible_) {
            ss << "    unset surface\n";
        }
        return ss.str();
    }

    std::string surface::plot_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        // plot surface
        bool is_solid_surface = palette_map_at_bottom_ ||
                                palette_map_at_surface_ || palette_map_at_top_;

        decltype(C_data_)::value_type::const_iterator min_it, max_it;
        if (fences_ && !C_data_.empty()) {
            std::tie(min_it, max_it) =
                std::minmax_element(C_data_[0].begin(), C_data_[0].end());
        }

        // if we have a waterfall or fences, we create one command per row
        // if we have ribbons, we create one command per column
        size_t n_plots = (waterfall_ || fences_) ? Z_data_.size()
                         : ribbons_              ? Z_data_[0].size()
                                                 : 1;
        for (size_t i = 0; i < n_plots; ++i) {
            if (i != 0) {
                ss << ",\\\n        ";
            }
            if (is_solid_surface) {
                // size_t line_index = create_line_index();
                ss << " '-' with";
                if (!fences_) {
                    ss << " pm3d at ";
                    if (palette_map_at_bottom_) {
                        ss << "b";
                    }
                    if (palette_map_at_surface_) {
                        ss << "s";
                    }
                    if (palette_map_at_top_) {
                        ss << "t";
                    }
                    // ss << " linestyle " << line_index;
                    if (line_spec_.user_color()) {
                        ss << " "
                           << line_spec_.plot_string(
                                  line_spec::style_to_plot::plot_line_only,
                                  false);
                    } else {
                        // Default line style for surfaces is black
                        // we don't follow the color order from the axes
                        // This would just be too ugly for surfaces.
                        ss << " "
                           << " linecolor rgb \"black\" dashtype 1 linewidth "
                              "0.5 linetype -1";
                    }
                } else {
                    ss << " zerrorfill";
                    if (!line_spec_.user_color()) {
                        color_array c;
                        if (C_data_.empty()) {
                            size_t color_index =
                                i % parent_->colororder().size();
                            c = parent_->colororder()[color_index];
                        } else {
                            c = parent_->colormap_interpolation(
                                C_data_[0][i], *min_it, *max_it);
                        }
                        ss << " linecolor rgb '" << to_string(c) << "'";
                    } else {
                        ss << " linecolor rgb '"
                           << to_string(line_spec_.color()) << "'";
                    }
                }
            } else {
                if (!line_spec_.user_color()) {
                    ss << " '-' with lines linecolor palette";
                } else {
                    ss << " '-' with lines linecolor rgb '"
                       << to_string(line_spec_.color()) << "'";
                }
            }
        }

        // plot contour
        const bool contour = (contour_base_ || contour_surface_);
        if (contour && contour_text_) {
            ss << ", '-' with labels ";
            const bool only_contour =
                !surface_visible_ && !palette_map_at_bottom_ &&
                !palette_map_at_surface_ && !palette_map_at_surface_;
            if (only_contour && iequals(font_weight_, "normal")) {
                ss << "boxed ";
            }
            ss << " textcolor '" << to_string(font_color_) << "' ";
        }
        return ss.str();
    }

    std::string surface::legend_string(std::string_view title) {
        return " keyentry " +
               line_spec_.plot_string(
                   line_spec::style_to_plot::plot_line_only) +
               " title \"" + escape(title) + "\"";
    }

    std::string surface::grid_data_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        const bool contour = (contour_base_ || contour_surface_);
        const bool palette_map_3d = palette_map_at_bottom_ ||
                                    palette_map_at_surface_ ||
                                    palette_map_at_top_;
        const bool repeat_data_for_contour_labels = contour && contour_text_;
        const bool manual_color = size(Z_data_) == size(C_data_);
        const size_t replicates = 1 + repeat_data_for_contour_labels;

        auto send_point = [](std::stringstream &ss, double x, double y,
                             double z, double c) {
            ss << "    " << x;
            ss << "  " << y;
            ss << "  " << z;
            if (std::isfinite(c)) {
                ss << "  " << c;
            }
            ss << "\n";
        };

        auto send_point_fill = [](std::stringstream &ss, double x, double y,
                                  double z, double zlow, double zhigh,
                                  double c) {
            ss << "    " << x;
            ss << "  " << y;
            ss << "  " << z;
            ss << "  " << zlow;
            ss << "  " << zhigh;
            if (std::isfinite(c)) {
                ss << "  " << c;
            }
            ss << "\n";
        };

        auto color_value = [&](size_t data_replicate, size_t i, size_t j) {
            if (manual_color && data_replicate == 0) {
                return C_data_[i][j];
            } else if (!palette_map_3d && !line_spec_.user_color()) {
                return Z_data_[i][j];
            } else {
                return NaN;
            }
        };

        for (size_t data_replicate = 0; data_replicate < replicates;
             ++data_replicate) {
            if (curtain_) {
                // open curtain - first line with zmin instead of z
                size_t i = Y_data_.size() - 1;
                send_point(ss, X_data_[i][0], Y_data_[i][0], zmin_,
                           color_value(data_replicate, i, 0));
                for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                    send_point(ss, X_data_[i][j], Y_data_[i][j], zmin_,
                               color_value(data_replicate, i, j));
                }
                send_point(
                    ss, X_data_[i][Y_data_[i].size() - 1],
                    Y_data_[i][Y_data_[i].size() - 1], zmin_,
                    color_value(data_replicate, i, Y_data_[i].size() - 1));
                ss << "\n";
            }
            // each row is an isoline
            for (long i = static_cast<long>(Y_data_.size()) - 1; i >= 0; --i) {
                // open row curtain or waterfall
                if (curtain_ || waterfall_) {
                    send_point(ss, X_data_[i][0], Y_data_[i][0], zmin_,
                               color_value(data_replicate, i, 0));
                }
                // send all points in that row
                for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                    if (!fences_) {
                        send_point(ss, X_data_[i][j], Y_data_[i][j],
                                   Z_data_[i][j],
                                   color_value(data_replicate, i, j));
                    } else {
                        send_point_fill(ss, X_data_[i][j], Y_data_[i][j],
                                        Z_data_[i][j], zmin_, Z_data_[i][j],
                                        color_value(data_replicate, i, j));
                    }
                }
                // close row curtain or waterfall
                if (curtain_ || waterfall_) {
                    send_point(
                        ss, X_data_[i][Y_data_[i].size() - 1],
                        Y_data_[i][Y_data_[i].size() - 1], zmin_,
                        color_value(data_replicate, i, Y_data_[i].size() - 1));
                }
                // end the current isoline
                if (!waterfall_ && !fences_) {
                    // usually an empty line to indicate this isoline is over
                    ss << "\n";
                } else {
                    // waterfalls and fences have one splot per row
                    // so we end not only the isoline
                    ss << "e\n";
                }
            }
            if (curtain_) {
                // close curtain
                size_t i = 0;
                send_point(ss, X_data_[i][0], Y_data_[i][0], zmin_,
                           color_value(data_replicate, i, 0));
                for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                    send_point(ss, X_data_[i][j], Y_data_[i][j], zmin_,
                               color_value(data_replicate, i, j));
                }
                send_point(
                    ss, X_data_[i][Y_data_[i].size() - 1],
                    Y_data_[i][Y_data_[i].size() - 1], zmin_,
                    color_value(data_replicate, i, Y_data_[i].size() - 1));
                ss << "\n";
            }

            // finish the plot
            // waterfalls don't need closing again
            if (!waterfall_ && !fences_) {
                ss << "e\n";
            }
        }
        return ss.str();
    }

    std::string surface::ribbon_data_string() {
        std::stringstream ss;
        ss.precision(10);
        ss << std::fixed;
        auto send_point = [](std::stringstream &ss, double x, double y,
                             double z, double c) {
            ss << "    " << x;
            ss << "  " << y;
            ss << "  " << z;
            if (std::isfinite(c)) {
                ss << "  " << c;
            }
            ss << "\n";
        };

        const bool manual_color = size(Z_data_) == size(C_data_);
        const bool palette_map_3d = palette_map_at_bottom_ ||
                                    palette_map_at_surface_ ||
                                    palette_map_at_top_;
        auto color_value = [&](size_t i, size_t j) {
            if (manual_color) {
                return C_data_[i][j];
            } else if (!palette_map_3d && !line_spec_.user_color()) {
                return Z_data_[i][j];
            } else {
                return NaN;
            }
        };

        const size_t n_rows = Z_data_.size();
        const size_t n_cols = Z_data_[0].size();

        const double x_diff = X_data_[0][1] - X_data_[0][0];
        const double absolute_width = ribbon_width_ * x_diff;

        // one ribbon per col
        for (size_t i = 0; i < n_cols; ++i) {
            // two isolines per row
            for (size_t j = 0; j < n_rows; ++j) {
                send_point(ss, X_data_[j][i] - absolute_width / 2.,
                           Y_data_[j][i], Z_data_[j][i], color_value(j, i));
                send_point(ss, X_data_[j][i] + absolute_width / 2.,
                           Y_data_[j][i], Z_data_[j][i], color_value(j, i));
                ss << "\n";
            }
            ss << "e\n";
        }
        return ss.str();
    }

    std::string surface::data_string() {
        return !ribbons_ ? grid_data_string() : ribbon_data_string();
    }

    void surface::maybe_update_line_spec() {
        if (!line_spec_.user_color()) {
            const bool plotting_contour =
                (contour_surface_ || contour_base_) && !contour_values_.empty();
            if (plotting_contour) {
                const bool using_colormap_for_contour =
                    contour_values_.size() != 1;
                if (!using_colormap_for_contour) {
                    auto c = parent_->get_color_and_bump();
                    contour_line_spec_.color(c);
                }
            }
        }
    }

    double surface::xmax() {
        double m = X_data_[0][0];
        for (size_t i = 0; i < X_data_.size(); ++i) {
            for (size_t j = 0; j < X_data_[i].size(); ++j) {
                m = std::max(m, X_data_[i][j]);
            }
        }
        return m;
    }

    double surface::xmin() {
        double m = X_data_[0][0];
        for (size_t i = 0; i < X_data_.size(); ++i) {
            for (size_t j = 0; j < X_data_[i].size(); ++j) {
                m = std::min(m, X_data_[i][j]);
            }
        }
        return m;
    }

    double surface::ymax() {
        double m = Y_data_[0][0];
        for (size_t i = 0; i < Y_data_.size(); ++i) {
            for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                m = std::max(m, Y_data_[i][j]);
            }
        }
        return m;
    }

    double surface::ymin() {
        double m = Y_data_[0][0];
        for (size_t i = 0; i < Y_data_.size(); ++i) {
            for (size_t j = 0; j < Y_data_[i].size(); ++j) {
                m = std::min(m, Y_data_[i][j]);
            }
        }
        return m;
    }

    enum axes_object::axes_category surface::axes_category() {
        if (!surface_in_2d_) {
            return axes_object::axes_category::three_dimensional;
        } else {
            return axes_object::axes_category::three_dimensional_map;
        }
    }

    class surface &surface::line_style(std::string_view str) {
        line_spec_.parse_string(str);
        touch();
        return *this;
    }

    const line_spec &surface::line_spec() const { return line_spec_; }

    line_spec &surface::line_spec() { return line_spec_; }

    class surface &surface::line_spec(const class line_spec &line_spec) {
        line_spec_ = line_spec;
        touch();
        return *this;
    }

    const vector_2d &surface::Y_data() const { return Y_data_; }

    class surface &surface::Y_data(const vector_2d &Y_data) {
        Y_data_ = Y_data;
        touch();
        return *this;
    }

    const vector_2d &surface::X_data() const { return X_data_; }

    class surface &surface::X_data(const vector_2d &X_data) {
        X_data_ = X_data;
        touch();
        return *this;
    }

    const vector_2d &surface::Z_data() const { return Z_data_; }

    class surface &surface::Z_data(const vector_2d &Z_data) {
        Z_data_ = Z_data;
        touch();
        return *this;
    }

    float surface::line_width() const { return line_spec().line_width(); }

    class surface &surface::line_width(float line_width) {
        line_spec().line_width(line_width);
        return *this;
    }

    const std::array<float, 4> &surface::edge_color() const {
        return line_spec().color();
    }

    size_t surface::norm() const { return norm_; }

    class surface &surface::norm(size_t norm) {
        norm_ = norm;
        touch();
        return *this;
    }

    const std::vector<vector_1d> &surface::x_data() const { return X_data_; }

    class surface &surface::x_data(const std::vector<vector_1d> &x_data) {
        X_data_ = x_data;
        touch();
        return *this;
    }

    const std::vector<vector_1d> &surface::y_data() const { return Y_data_; }

    class surface &surface::y_data(const std::vector<vector_1d> &y_data) {
        Y_data_ = y_data;
        touch();
        return *this;
    }

    const std::vector<vector_1d> &surface::z_data() const { return Z_data_; }

    class surface &surface::z_data(const std::vector<vector_1d> &z_data) {
        Z_data_ = z_data;
        touch();
        return *this;
    }

    bool surface::hidden_3d() const { return hidden3d_; }

    class surface &surface::hidden_3d(bool hidden_3_d) {
        hidden3d_ = hidden_3_d;
        touch();
        return *this;
    }

    bool surface::surface_visible() const { return surface_visible_; }

    class surface &surface::surface_visible(bool surface_visible) {
        surface_visible_ = surface_visible;
        touch();
        return *this;
    }

    bool surface::surface_in_2d() const { return surface_in_2d_; }

    class surface &surface::surface_in_2d(bool surface_in_2d) {
        surface_in_2d_ = surface_in_2d;
        touch();
        return *this;
    }

    bool surface::palette_map_at_bottom() const {
        return palette_map_at_bottom_;
    }

    class surface &surface::palette_map_at_bottom(bool palette_map_at_bottom) {
        palette_map_at_bottom_ = palette_map_at_bottom;
        touch();
        return *this;
    }

    bool surface::palette_map_at_surface() const {
        return palette_map_at_surface_;
    }

    class surface &
    surface::palette_map_at_surface(bool palette_map_at_surface) {
        palette_map_at_surface_ = palette_map_at_surface;
        touch();
        return *this;
    }

    bool surface::palette_map_at_top() const { return palette_map_at_top_; }

    class surface &surface::palette_map_at_top(bool palette_map_at_top) {
        palette_map_at_top_ = palette_map_at_top;
        touch();
        return *this;
    }

    bool surface::contour_base() const { return contour_base_; }

    class surface &surface::contour_base(bool contour_base) {
        contour_base_ = contour_base;
        if (contour_base) {
            hidden3d_ = true;
        }
        touch();
        return *this;
    }

    bool surface::contour_surface() const { return contour_surface_; }

    class surface &surface::contour_surface(bool contour_surface) {
        contour_surface_ = contour_surface;
        if (contour_surface) {
            hidden3d_ = true;
        }
        touch();
        return *this;
    }

    size_t surface::contour_levels() const { return contour_levels_; }

    class surface &surface::contour_levels(size_t contour_levels) {
        contour_levels_ = contour_levels;
        touch();
        return *this;
    }

    const std::vector<double> &surface::contour_values() const {
        return contour_values_;
    }

    class surface &
    surface::contour_values(const std::vector<double> &contour_values) {
        contour_values_ = contour_values;
        touch();
        return *this;
    }

    bool surface::contour_text() const { return contour_text_; }

    class surface &surface::contour_text(bool contour_text) {
        contour_text_ = contour_text;
        touch();
        return *this;
    }

    float surface::font_size() const {
        if (font_size_) {
            return *font_size_;
        } else {
            return parent_->font_size();
        }
    }

    class surface &surface::font_size(const float &font_size) {
        font_size_ = font_size;
        touch();
        return *this;
    }

    const std::string surface::font() const {
        if (font_) {
            return *font_;
        } else {
            return parent_->font();
        }
    }

    class surface &surface::font(std::string_view font) {
        font_ = font;
        touch();
        return *this;
    }

    const std::string &surface::font_weight() const { return font_weight_; }

    class surface &surface::font_weight(std::string_view font_weight) {
        font_weight_ = font_weight;
        touch();
        return *this;
    }

    const color_array &surface::font_color() const { return font_color_; }

    class surface &surface::font_color(const color_array &font_color) {
        font_color_ = font_color;
        touch();
        return *this;
    }

    class surface &surface::font_color(std::string_view fc) {
        font_color(to_array(fc));
        return *this;
    }

    bool surface::depthorder() const { return depthorder_; }

    class surface &surface::depthorder(bool depthorder) {
        depthorder_ = depthorder;
        touch();
        return *this;
    }

    float surface::face_alpha() const { return face_alpha_; }

    class surface &surface::face_alpha(float face_alpha) {
        face_alpha_ = face_alpha;
        touch();
        return *this;
    }

    bool surface::lighting() const { return lighting_; }

    class surface &surface::lighting(bool lighting) {
        lighting_ = lighting;
        touch();
        return *this;
    }

    float surface::primary() const { return primary_; }

    class surface &surface::primary(float primary) {
        primary_ = primary;
        touch();
        return *this;
    }

    float surface::specular() const { return specular_; }

    class surface &surface::specular(float specular) {
        specular_ = specular;
        touch();
        return *this;
    }

    const class line_spec &surface::contour_line_spec() const {
        return contour_line_spec_;
    }

    class line_spec &surface::contour_line_spec() {
        return contour_line_spec_;
    }

    class surface &
    surface::contour_line_spec(const class line_spec &contour_line_spec) {
        contour_line_spec_ = contour_line_spec;
        return *this;
    }

    double surface::zmin() { return zmin_; }

    double surface::zmax() { return zmax_; }

    bool surface::curtain() const { return curtain_; }

    class surface &surface::curtain(bool curtain) {
        curtain_ = curtain;
        touch();
        return *this;
    }

    bool surface::waterfall() const { return waterfall_; }

    class surface &surface::waterfall(bool waterfall) {
        waterfall_ = waterfall;
        touch();
        return *this;
    }

    double surface::ribbon_width() const { return ribbon_width_; }

    class surface &surface::ribbon_width(double ribbon_width) {
        if (ribbon_width != ribbon_width_) {
            ribbon_width_ = ribbon_width;
            touch();
        }
        return *this;
    }

    bool surface::ribbons() const { return ribbons_; }

    class surface &surface::ribbons(bool ribbons) {
        ribbons_ = ribbons;
        touch();
        return *this;
    }

    bool surface::fences() const { return fences_; }

    class surface &surface::fences(bool fences) {
        fences_ = fences;
        palette_map_at_surface_ = true;
        depthorder_ = true;
        touch();
        return *this;
    }

} // namespace matplot