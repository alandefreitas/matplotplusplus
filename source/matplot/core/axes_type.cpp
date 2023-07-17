//
// Created by Alan Freitas on 2020-07-05.
//

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <memory>
#include <sstream>

#include <matplot/core/axes_object.h>
#include <matplot/core/axes_type.h>
#include <matplot/core/figure_type.h>

#include <matplot/util/colors.h>
#include <matplot/util/common.h>
#include <matplot/util/concepts.h>
#include <matplot/util/contourc.h>
#include <matplot/util/geodata.h>

#include <matplot/axes_objects/bars.h>
#include <matplot/axes_objects/box_chart.h>
#include <matplot/axes_objects/circles.h>
#include <matplot/axes_objects/contours.h>
#include <matplot/axes_objects/error_bar.h>
#include <matplot/axes_objects/filled_area.h>
#include <matplot/axes_objects/function_line.h>
#include <matplot/axes_objects/labels.h>
#include <matplot/axes_objects/line.h>
#include <matplot/axes_objects/matrix.h>
#include <matplot/axes_objects/network.h>
#include <matplot/axes_objects/parallel_lines.h>
#include <matplot/axes_objects/stair.h>
#include <matplot/axes_objects/string_function.h>
#include <matplot/axes_objects/surface.h>
#include <matplot/axes_objects/vectors.h>

#include <matplot/freestanding/histcounts.h>

namespace matplot {

    void axes_type::draw() { parent_->draw(); }

    void axes_type::touch() { parent_->touch(); }

    void axes_type::emplace_object(axes_object_handle obj) {
        if (next_plot_replace_) {
            children_.clear();
            colororder_index_ = 0;
        }
        children_.push_back(obj);
        touch();
    }

    void axes_type::run_title_command() {
        include_comment("Axes title");
        if (title_visible_ && !title_.empty()) {
            std::string cmd;
            cmd += "set title \"";
            if (title_enhanced_ && iequals(title_font_weight_, "bold")) {
                cmd += "{/:Bold ";
            }
            if (title_enhanced_ && iequals(title_font_weight_, "italic")) {
                cmd += "{/:Italic ";
            }
            cmd += escape(title());
            if (title_enhanced_ && iequals(title_font_weight_, "bold")) {
                cmd += "}";
            }
            if (title_enhanced_ && iequals(title_font_weight_, "italic")) {
                cmd += "}";
            }
             cmd += "\"";
            if (parent_->backend_->supports_fonts()) {
                cmd += " font \"" + escape(font()) + "," +
                       std::to_string(unsigned(font_size() *
                                               title_font_size_multiplier_)) +
                       "\"";
                cmd += " textcolor '" + to_string(title_color_) + "'";
            }
            if (title_enhanced_) {
                cmd += " enhanced";
            } else {
                cmd += " noenhanced";
            }
            run_command(cmd);
        } else {
            run_command("unset title");
        }
    }

    void axes_type::run_colormap_command() {
        bool any_obj_needs_colormap = false;
        for (const auto &child : children_) {
            if (child->requires_colormap()) {
                any_obj_needs_colormap = true;
                break;
            }
        }
        if (any_obj_needs_colormap && !colormap_.empty() &&
            !children_.empty()) {
            std::stringstream ss;
            ss.precision(10);
            ss << std::fixed;
            // limit the number of colors in the palette
            // this is useful for contour plots
            if (max_colors_) {
                ss << "    set palette maxcolors " << max_colors_ << "\n";
            }
            ss << "    set palette model RGB\n";
            ss << "    set palette defined (\\\n";
            if (max_colors_ == 0) {
                // put all colormap colors in the palette
                if (colormap_.size() == 1) {
                    colormap_.emplace_back(colormap_.front());
                }
                for (size_t i = 0; i < colormap_.size(); ++i) {
                    if (colormap_[i].size() == 4) {
                        std::array<double, 4> c = to_array<4>(colormap_[i]);
                        std::array<float, 4> c2{};
                        std::transform(c.begin(), c.end(), c2.begin(),
                                       [](const double &x) {
                                           return static_cast<float>(x);
                                       });
                        // std::copy(c.begin(), c.end(), c2.begin());
                        ss << "    " << i << "   \"" << to_string(c2) << "\"";
                    } else {
                        ss << "    " << i << "   " << colormap_[i][0] << " "
                           << colormap_[i][1] << " " << colormap_[i][2];
                    }
                    if (i != colormap_.size() - 1) {
                        ss << ",\\\n";
                    } else {
                        ss << ")\n";
                    }
                }
            } else {
                // if we limit the number of colors, the new colors
                // are the max_color appropriate interpolations in
                // the original colormap
                for (size_t i = 0; i < max_colors_; ++i) {
                    color_array c = colormap_interpolation(
                        static_cast<double>(i), 0., max_colors_ - 1.);
                    ss << "    " << i << "   " << c[0 + 1] << " " << c[1 + 1]
                       << " " << c[2 + 1];
                    if (i != max_colors_ - 1 || max_colors_ == 1) {
                        ss << ",\\\n";
                    } else {
                        ss << ")\n";
                    }
                }
                // we can't have just one color, at least in some versions
                // of gnuplot we can't
                if (max_colors_ == 1) {
                    color_array c =
                        colormap_interpolation(0, 0., max_colors_ - 1.);
                    ss << "    " << 1 << "   " << c[0 + 1] << " " << c[1 + 1]
                       << " " << c[2 + 1] << ")\n";
                }
            }
            run_command(ss.str());
        }
    }

    std::tuple<double, double, double, double, double, double>
    axes_type::calculate_margins() {
        // There are conditions on which the xlim are smaller than requested
        // to make room for other elements
        double width_multiplier = is_3d() && !is_3d_map() ? 0.72 : 1.;
        double height_multiplier = is_3d() && !is_3d_map() ? 0.72 : 1.;

        double x = x_origin();
        double y = y_origin();

        // if parent has a title
        if (!parent_->title().empty()) {
            // open room for the title
            double room_needed = 0.2;
            double this_axes_contribution = room_needed * height();
            height_multiplier -= this_axes_contribution;
            // the further from y=0, the larger the shift
            y -= 1.5 * y * this_axes_contribution;
        }

        double rmargin_multiplier = (1. - width_multiplier) / 2.;
        double lmargin_multiplier = (1. - width_multiplier) / 2.;
        double tmargin_multiplier = (1. - height_multiplier) / 2.;
        double bmargin_multiplier = (1. - height_multiplier) / 2.;

        const auto bg_width = width_multiplier * width();
        const auto bg_height = height_multiplier * height();

        // We increase the margins for colorbar
        double colorbar_lmargin = 0.;
        double colorbar_rmargin = 0.;
        double colorbar_bmargin = 0.;
        double colorbar_tmargin = 0.;

        constexpr double extra_for_ticks = 0.05;
        if (cb_axis_.visible() && !cb_inside_) {
            // identify if colorbar is north/south/east/west
            auto [cbx, cby, cbw, cbh] = cb_position_;
            bool automatic_position = cbw == cbh;
            if (automatic_position) {
                if (cb_vertical_) {
                    colorbar_rmargin = 0.04;
                } else {
                    colorbar_bmargin = 0.1;
                }
            } else if (cbw > cbh) {
                // north/south
                float south_margin = cby + cbh;
                float north_margin = 1.f - cby;
                if (south_margin <= north_margin) {
                    // south
                    colorbar_bmargin = south_margin + extra_for_ticks;
                } else {
                    // north
                    colorbar_tmargin = north_margin + extra_for_ticks;
                }
            } else {
                // west/east
                float east_margin = cbx + cbw;
                float west_margin = 1.f - cbx;
                if (east_margin <= west_margin) {
                    // east
                    colorbar_rmargin = east_margin + extra_for_ticks;
                } else {
                    // west
                    colorbar_lmargin = west_margin + extra_for_ticks;
                }
            }
        }

        const double lmargin =
            x + width() * lmargin_multiplier + colorbar_lmargin;
        const double rmargin =
            x + width() - width() * rmargin_multiplier - colorbar_rmargin;
        const double bmargin =
            y + height() * bmargin_multiplier + colorbar_bmargin;
        const double tmargin =
            y + height() - height() * tmargin_multiplier - colorbar_tmargin;

        return std::make_tuple(bg_width, bg_height, lmargin, rmargin, bmargin,
                               tmargin);
    }

    void axes_type::run_position_margin_command() {
        include_comment("Axes position");
        run_command("set origin " + std::to_string(x_origin()) + "," +
                    std::to_string(y_origin()));

        auto [bg_width, bg_height, lmargin, rmargin, bmargin, tmargin] =
            calculate_margins();

        run_command("set size " + num2str(bg_width) + "," + num2str(bg_height));

        run_command("set lmargin at screen " + num2str(lmargin));
        run_command("set rmargin at screen " + num2str(rmargin));
        run_command("set bmargin at screen " + num2str(bmargin));
        run_command("set tmargin at screen " + num2str(tmargin));

        if (!axes_aspect_ratio_auto_) {
            if (is_3d()) {
                run_command("set view equal xyz");
            } else {
                run_command("set size ratio " +
                            std::to_string(axes_aspect_ratio_));
            }
        }
    }

    void axes_type::run_grid_command() {
        // if the user didn't explicitly set the grid, turn on the grid
        // by default in logscale
        if (!x_user_grid_) {
            x_grid_ = x_axis().scale() == axis_type::axis_scale::log || is_3d();
            x_minor_grid_ = x_axis().scale() == axis_type::axis_scale::log;
        }
        if (!y_user_grid_) {
            y_grid_ = y_axis().scale() == axis_type::axis_scale::log || is_3d();
            y_minor_grid_ = y_axis().scale() == axis_type::axis_scale::log;
        }
        if (!z_user_grid_) {
            z_grid_ = z_axis().scale() == axis_type::axis_scale::log || is_3d();
            z_minor_grid_ = z_axis().scale() == axis_type::axis_scale::log;
        }
        if (!r_user_grid_) {
            r_grid_ =
                r_axis().scale() == axis_type::axis_scale::log || is_polar();
            r_minor_grid_ = r_axis().scale() == axis_type::axis_scale::log;
        }

        // set grid xtics ytics mxtics mytics layerdefault linecolor 'blue'
        // linetype 1 linewidth 2, linecolor 'red'
        if (x_grid_ || x_minor_grid_ || y_grid_ || y_minor_grid_ || z_grid_ ||
            z_minor_grid_ || r_grid_ || r_minor_grid_) {
            include_comment("Create grid");
            std::string cmd = "set grid";
            if (x_grid_) {
                cmd += " xtics";
            }
            if (x_minor_grid_) {
                cmd += " mxtics";
            }
            if (y_grid_) {
                cmd += " ytics";
            }
            if (y_minor_grid_) {
                cmd += " mytics";
            }
            if (z_grid_) {
                cmd += " ztics";
            }
            if (z_minor_grid_) {
                cmd += " mztics";
            }
            if (r_grid_) {
                cmd += " polar";
            }
            if (r_minor_grid_) {
                cmd += " mrtics";
            }
            cmd += " vertical";

            if (grid_front_) {
                cmd += " front";
            }
            cmd += grid_line_style_.plot_string(
                line_spec::style_to_plot::plot_line_only, false);
            if (x_minor_grid_ || y_minor_grid_ || z_minor_grid_) {
                cmd += ",";
                if (grid_front_) {
                    cmd += " front ";
                }
                cmd += minor_grid_line_style_.plot_string(
                    line_spec::style_to_plot::plot_line_only, false);
            }
            run_command(cmd);
        }
    }

    void axes_type::run_box_command() {
        include_comment("Axes box");
        if (!visible_) {
            run_command("unset border");
            run_command("unset xtics");
            run_command("unset ytics");
            run_command("unset ztics");
            return;
        }

        /*  Border types for box
            1	bottom	    bottom left front
            2	left	    bottom left back
            4	top	        bottom right front
            8	right	    bottom right back
            16	no effect	left vertical
            32	no effect	back vertical
            64	no effect	right vertical
            128	no effect	front vertical
            256	no effect	top left back
            512	no effect	top right back
            1024	no effect	top left front
            2048	no effect	top right front
         */
        if (!box_ || is_polar()) {
            if (is_3d()) {
                // create borders
                // bottom left front 1
                // bottom right front 4
                // left vertical 16
                run_command("set border 21 linecolor \"" +
                            to_string(x_axis().color()) + "\" linewidth " +
                            num2str(line_width()));
            } else if (is_2d()) {
                // create left (2) and bottom (1)
                unsigned border = 0;
                if (x_axis().visible()) {
                    border += 1;
                }
                if (y_axis().visible()) {
                    border += 2;
                }
                run_command("set border " + num2str(border) + " linecolor \"" +
                            to_string(x_axis().color()) + "\" linewidth " +
                            num2str(line_width()));
            } else {
                run_command("unset border");
            }
        } else {
            // if (box_)
            if (is_3d()) {
                if (box_full_) {
                    run_command("set border 4095 linecolor \"" +
                                to_string(box_color_) + "\" linewidth " +
                                num2str(line_width()));
                } else {
                    // create borders
                    // bottom left front 1
                    // bottom left back 2
                    // bottom right front 4
                    // bottom right back 8
                    // left vertical 16
                    // back vertical 32
                    // right vertical 64
                    // top left back 256
                    // top right back 512
                    run_command("set border 895 linecolor \"" +
                                to_string(box_color_) + "\" linewidth " +
                                num2str(line_width()));
                }
            } else {
                // create all borders (bottom / left / top / right) in black
                run_command("set border 15 linecolor \"" +
                            to_string(box_color_) + "\" linewidth " +
                            num2str(line_width()));
            }
        }
    }

    void axes_type::run_axes_command() {
        // plot tics
        // http://www.gnuplot.info/docs_4.2/node182.html
        if (is_polar()) {
            run_command("set polar");
        }
        auto set_or_unset_axis = [this](class axis_type &ax,
                                        std::string axis_name,
                                        bool minor_ticks = false) {
            // cb is the only axis we don't unset if tics are empty
            // r-axis labels should still be handled even if axis is invisible since we use the grid
            if ((ax.visible() || axis_name == "r") && !ax.tick_values().empty()) {
                if (axis_name == "r" && !ax.visible()) {
                    // Hide the r-axis but not the grid.
                    // We can't completely unset the tics or the grid does not
                    // appear properly
                    run_command("unset " + axis_name + "axis");
                    run_command("set " + axis_name + "tics scale 0");
                }
                if (ax.geographic()) {
                    run_command("set " + axis_name + "tics geographic");
                }
                if (ax.on_axis()) {
                    run_command("set " + axis_name + "tics axis");
                }
                run_command("set format " + axis_name + " \"" +
                            escape(ax.tick_label_format()) + "\"");
                std::string tics_str = "set " + axis_name +
                                       "tics textcolor \"" +
                                       to_string(ax.color());
                if (parent_->backend_->supports_fonts()) {
                    tics_str += "\" font \"" + font() + "," +
                                num2str(unsigned(font_size())) + "\"";
                }
                tics_str +=
                    " nomirror in scale " + num2str(ax.tick_length()) + " ";
                run_command(tics_str);
                if (ax.tick_values_manual() || ax.tick_values().empty()) {
                    run_command("set " + axis_name + "tics" +
                                ax.tick_values_string(minor_ticks) +
                                ax.tick_rotate_string());
                } else {
                    if (ax.tickangle() != 0.) {
                        run_command("set " + axis_name + "tics" +
                                    ax.tick_rotate_string());
                    }
                    if (minor_ticks) {
                        run_command("set m" + axis_name + "tics 2");
                    }
                }
            } else { // (!visible && !r) || empty
                if (axis_name == "r") {
                    // Hiding the r-axis works differently.
                    // We can't completely unset the tics or the grid does not
                    // appear properly
                    run_command("unset " + axis_name + "axis");
                    run_command("set " + axis_name + "tics scale 0");
                    if (ax.tick_values().empty()) {
                        // Note: Disabling tick labels without also
                        // disabling the grid works via 'set format'
                        // as per gnuplot manual "xticks" (see tic
                        // labels). using 'set xticks () ...' would be
                        // insufficient.
                        run_command("set format " + axis_name + " \"\"");
                    }
                } else if (axis_name == "cb") {
                    // the colorbar / colorbox has a special command to unset it
                    // if only ticks are empty we still want to show the box
                    if (!ax.visible()) {
                        run_command("unset colorbox");
                    }
                    if (ax.tick_values().empty()) {
                        run_command("unset cbtics");
                    }

                } else {
                    run_command("unset " + axis_name + "tics");
                }
            }
            const bool zero_axis_is_possible =
                axis_name != "r" && axis_name != "t" && axis_name != "cb";
            if (zero_axis_is_possible) {
                if (ax.zero_axis()) {
                    run_command("set " + axis_name + "zeroaxis linestyle -1");
                } else {
                    run_command("unset " + axis_name + "zeroaxis");
                }
            }
        };

        if (!visible_) {
            run_command("unset xtics");
            run_command("unset ytics");
            run_command("unset x2tics");
            run_command("unset y2tics");
            if (is_3d()) {
                run_command("unset ztics");
            }
            if (is_polar()) {
                run_command("unset raxis");
                run_command("set rtics scale 0");
                run_command("unset ttics");
            }
            if (!cb_axis_.visible()) {
                run_command("unset colorbox");
            }
        } else {
            set_or_unset_axis(x_axis_, "x", x_minor_grid_);
            set_or_unset_axis(x2_axis_, "x2", x_minor_grid_);
            set_or_unset_axis(y_axis_, "y", y_minor_grid_);
            set_or_unset_axis(y2_axis_, "y2", y_minor_grid_);

            const bool is_map = azimuth_ == 0 && elevation_ == 90;
            if (is_3d() && !is_map) {
                set_or_unset_axis(z_axis_, "z", z_minor_grid_);
            }

            if (is_polar()) {
                set_or_unset_axis(r_axis_, "r", r_minor_grid_);
                set_or_unset_axis(t_axis_, "t", t_minor_grid_);
            }

            if (cb_axis_.visible()) {
                std::stringstream ss;
                ss.precision(10);
                ss << std::fixed;
                ss << "set colorbox";
                if (!cb_axis_.reverse()) {
                    ss << " noinvert";
                } else {
                    ss << " invert";
                }
                if (cb_vertical_) {
                    ss << " vertical";
                } else {
                    ss << " horizontal";
                }
                if (cb_position_[2] == 0. && cb_position_[3] == 0.) {
                    ss << " default";
                } else {
                    auto [x, y, w, h] = position_;
                    ss << " user";
                    ss << " origin " << x + cb_position_[0] * w << ","
                       << y + cb_position_[1] * h;
                    ss << " size " << cb_position_[2] * w << ","
                       << cb_position_[3] * h;
                }
                run_command(ss.str());
            }
            set_or_unset_axis(cb_axis_, "cb", false);

            /*
             * if (color_box_) {
                run_command("set colorbox");
                if (color_box_log_scale_) {
                    run_command("set logscale cb");
                }
            } else {
                run_command("unset colorbox");
                if (color_box_log_scale_) {
                    run_command("unset logscale cb");
                }
            }
             */
        }

        include_comment("Axes ranges");
        run_command("set xrange " + x_axis().range_string());
        run_command("set yrange " + y_axis().range_string());
        run_command("set y2range " + y2_axis().range_string());
        if (is_3d()) {
            run_command("set zrange " + z_axis().range_string());
        }
        if (is_polar() && r_axis().limits_mode_manual()) {
            run_command("set rrange " + r_axis().range_string());
        }
        if (cb_axis_.visible()) {
            run_command("set cbrange " + cb_axis_.range_string());
        }

        include_comment("Axes scale");
        if (x_axis().scale() == axis_type::axis_scale::log) {
            run_command("set logscale x");
        }
        if (y_axis().scale() == axis_type::axis_scale::log) {
            run_command("set logscale y");
        }
        if (is_3d()) {
            if (z_axis().scale() == axis_type::axis_scale::log) {
                run_command("set logscale z");
            }
        }

        if (is_3d()) {
            include_comment("Axes view");
            run_command("set xyplane 0");
            // gnuplot azimuths are bounded [0:360]
            float gnuplot_azimuth_ = azimuth_ + 360;
            while (gnuplot_azimuth_ < 0) {
                gnuplot_azimuth_ += 360;
            }
            while (gnuplot_azimuth_ > 360) {
                gnuplot_azimuth_ -= 360;
            }

            // gnuplot elevation is bounded [0,180]
            // our elevation is bounded [-90,+90]
            // matlab -90 = gnuplot 180
            // matlab -45 = gnuplot 135
            // matlab  0  = gnuplot  90
            // matlab +45 = gnuplot  45
            // matlab +90 = gnuplot   0
            float gnuplot_elevation_ = 90 - elevation_;
            while (gnuplot_elevation_ < 0) {
                gnuplot_elevation_ += 180;
            }
            while (gnuplot_elevation_ > 180) {
                gnuplot_elevation_ -= 180;
            }
            run_command("set view " + num2str(gnuplot_elevation_) + "," +
                        num2str(gnuplot_azimuth_));
        }
    }

    void axes_type::run_labels_command() {
        include_comment("Axes labels");
        //      set xlabel {"<label>"} {offset <offset>} {font
        //      "<font>{,<size>}"}
        //                {{textcolor | tc} {lt <line_type> | default}}
        //                {{no}enhanced} {rotate by <degrees>}
        if (!x_axis_.label().empty()) {
            std::string cmd = "set xlabel" + x_axis_.label_string();
            run_command(cmd);
        }
        if (!y_axis_.label().empty()) {
            std::string cmd = "set ylabel" + y_axis_.label_string();
            run_command(cmd);
        }
        if (!y2_axis_.label().empty()) {
            std::string cmd = "set y2label" + y2_axis_.label_string();
            run_command(cmd);
        }

        const bool is_map = azimuth_ == 0 && elevation_ == 90;
        if (is_3d() && !z_axis_.label().empty() && !is_map) {
            std::string cmd =
                "set zlabel" + z_axis_.label_string() + " rotate parallel";
            run_command(cmd);
        }
        if (!cb_axis_.label().empty()) {
            std::string cmd =
                "set cblabel" + cb_axis_.label_string() + " rotate parallel";
            run_command(cmd);
        }
    }

    void axes_type::run_legend_command() {
        //  set key {on|off} {default}
        //  {{inside | outside} | {lmargin | rmargin | tmargin | bmargin}
        //      | {at <position>}}
        //  {left | right | center} {top | bottom | center}
        //  {vertical | horizontal} {Left | Right}
        //  {{no}reverse} {{no}invert}
        //  {samplen <sample_length>} {spacing <vertical_spacing>}
        //  {width <width_increment>}
        //  {height <height_increment>}
        //  {{no}autotitle {columnheader}}
        //  {title "<text>"} {{no}enhanced}
        //  {{no}box { {linestyle | ls <line_style>}
        //              | {linetype | lt <line_type>}
        //              {linewidth | lw <line_width>}}}
        include_comment("Axes legend");
        // Gnuplot version needs to be 5.2.6+ for keyentry
        bool ok = true;
        if (parent_->backend_->consumes_gnuplot_commands()) {
            if (backend::gnuplot::gnuplot_version() <
                std::tuple<int, int, int>{5, 2, 6}) {
                ok = false;
            }
        }
        if (legend_ == nullptr || !legend_->visible() || !ok) {
            run_command("set key off");
        } else {
            bool legends_to_plot = !legend_->empty();
            if (!legends_to_plot) {
                for (const auto &child : children_) {
                    if (!child->display_name().empty()) {
                        legends_to_plot = true;
                        break;
                    }
                }
            }

            if (!legends_to_plot) {
                run_command("set key off");
            } else {
                std::string cmd = "set key on";
                if (parent_->backend_->supports_fonts()) {
                    cmd += " font \"" + legend_->font_name() + "," +
                           num2str(legend_->font_size()) +
                           "\" textcolor rgb '" +
                           to_string(legend_->text_color()) + "'";
                }
                // position
                cmd += legend_->inside() ? " inside" : " outside";
                if (legend_->manual_position()) {
                    cmd += " at " + num2str(legend_->position()[0]) + "," +
                           num2str(legend_->position()[1]);
                }
                switch (legend_->horizontal_location()) {
                case legend::horizontal_alignment::left:
                    cmd += " left";
                    break;
                case legend::horizontal_alignment::right:
                    cmd += " right";
                    break;
                case legend::horizontal_alignment::center:
                    cmd += " center";
                    break;
                }
                switch (legend_->vertical_location()) {
                case legend::vertical_alignment::top:
                    cmd += " top";
                    break;
                case legend::vertical_alignment::bottom:
                    cmd += " bottom";
                    break;
                case legend::vertical_alignment::center:
                    cmd += " center";
                    break;
                }
                cmd += legend_->vertical() ? " vertical" : " horizontal";
                // text aligned to the left
                cmd += " Left";
                cmd += " opaque";
                cmd +=
                    legend_->label_after_sample() ? " reverse" : " noreverse";
                cmd += legend_->invert() ? " invert" : " noinvert";
                // increase the width to make labels fit
                cmd += " width 1";
                // forbid automatic labels
                // we are going to generate them explicitly for our xlim objects
                cmd += " noautotitle";
                // box
                if (!legend_->title().empty()) {
                    cmd += " title \"";
                    if (iequals(legend_->font_weight(), "bold")) {
                        cmd += "{/:Bold ";
                    }
                    cmd += escape(legend_->title());
                    if (iequals(legend_->font_weight(), "bold")) {
                        cmd += "}";
                    }
                    cmd += "\"";
                    if (parent_->backend_->supports_fonts()) {
                        cmd += " font \"" + escape(legend_->font_name()) + "," +
                               num2str(unsigned(legend_->font_size())) + "\"";
                        cmd += " textcolor rgb \"" +
                               to_string(legend_->text_color()) + "\"";
                    }
                }
                if (legend_->box() && legend_->box_line().has_line()) {
                    cmd += " box";
                    cmd += legend_->box_line().plot_string(
                        line_spec::style_to_plot::plot_line_only, false);
                } else {
                    cmd += " nobox";
                }
                if (legend_->num_columns() != 0) {
                    size_t n_strings = legend_->size();
                    for (size_t i = 0; i < children_.size(); ++i) {
                        n_strings += !children_[i]->display_name().empty();
                    }
                    cmd += " maxrows " +
                           num2str(ceil(static_cast<double>(n_strings) /
                                        legend_->num_columns()));
                }
                if (legend_->num_rows() != 0) {
                    cmd += " maxrows " + num2str(legend_->num_rows());
                }

                run_command(cmd);
            }
        }
    }

    void axes_type::run_background_command() {
        const bool saving_to_file = !parent_->backend_->output().empty();
        const bool using_default_color =
            color_ == std::array<float, 4>{0, 1.0, 1.0, 1.0};
        if (saving_to_file && using_default_color) {
            return;
        }
        include_comment("Axes background");
        // plot background
        if (visible_) {
            // rectangle behind the graph
            if (color_ != parent()->color()) {
                if (is_2d() || is_3d_map()) {
                    run_command("set object 2 rectangle from graph 0,0 to "
                                "graph 1,1 behind fillcolor rgb \"" +
                                to_string(color()) +
                                "\" fillstyle solid 1.0 noborder");
                    /* } else if (is_3d()) {
                       // we can uncomment this to do something with the "wall"
                       // option in the future.
                        auto v = backend::gnuplot::gnuplot_version();
                        const bool has_wall_option =
                            v < std::tuple<int, int, int>{5, 5, 0};
                        if (has_wall_option) {
                            run_command("set object 2 rectangle from graph 0,0
    to " "graph 1,1 behind fillcolor rgb \"" + to_string(parent_->color()) +
                                        "\" fillstyle solid 1.0 noborder");
                            run_command("set wall z0 fs transparent solid 0.5 "
                                        "border -1 fc rgb '" +
    to_string(color()) + "'"); run_command("set wall x0 fs transparent solid 0.5
    " "border -1 fc rgb '" + to_string(color()) + "'"); run_command("set wall y1
    fs transparent solid 0.5 " "border -1 fc rgb '" + to_string(color()) + "'");
                        }
                        */
                } else if (is_polar()) {
                    double m = children_[0]->xmax();
                    run_command("set object 2 ellipse at first 0,0 size " +
                                num2str(m * 2) + "," + num2str(m * 2) +
                                " angle 0 behind fillcolor rgb \"" +
                                to_string(color()) +
                                "\" fillstyle solid 1.0 noborder");
                }
            }
            // rectangle behind the whole area allocated for the xlim
            // we don't use it anymore
            /*
             * if (false && color_ != parent()->color()) {
             *  run_command("set object 2 rectangle from screen " +
             * std::to_string(x_origin()) + "," + std::to_string(y_origin()) + "
             * to screen " + std::to_string(x_origin() + width()) + "," +
             *              std::to_string(y_origin() + height()) + " behind
             * fillcolor rgb \"" + to_string(color_outside_) + "\" fillstyle
             * solid 1.0 noborder");
             * }
             */
        }
    }

    void axes_type::run_empty_plot_command() {
        include_comment("Empty xlim");
        if (x_axis().limits_mode_auto()) {
            run_command("set yrange [0:1]");
        }
        if (y_axis().limits_mode_auto()) {
            run_command("set yrange [0:1]");
        }
        run_command("set key off");
        if (y_axis().limits_mode_auto() ||
            !std::isfinite(y_axis().limits()[1])) {
            run_command("plot 2 with lines");
        } else {
            run_command("plot " + std::to_string(y_axis().limits_[1] + 1) +
                        " with lines");
        }
    }

    void axes_type::run_plot_objects_command() {
        include_comment("Axes objects");
        // Plot empty xlim if there are no objects
        if (children_.empty()) {
            run_empty_plot_command();
            return;
        }

        // Set variable commands
        std::string set_variables_command;
        for (const auto &child : children_) {
            set_variables_command += child->set_variables_string();
        }
        run_command(set_variables_command);

        // Plot command
        std::string plot_command = !is_3d() ? "plot " : "splot ";

        // Plot all children
        bool first = true;
        for (const auto &child : children_) {
            if (!first) {
                plot_command += ",\\\n         ";
            } else {
                first = false;
            }
            plot_command += child->plot_string();
        }

        // Keyentry commands (legends for each child)
        if (legend_ != nullptr && legend_->visible()) {
            bool ok = true;
            static bool msg_shown_once = false;
            // Gnuplot version needs to be 5.2.6+ for keyentry
            if (parent_->backend_->consumes_gnuplot_commands()) {
                std::tuple<int, int, int> v =
                    backend::gnuplot::gnuplot_version();
                if (v < std::tuple<int, int, int>{5, 2, 6}) {
                    if (!msg_shown_once) {
                        std::cerr
                            << "You need gnuplot 5.2.6+ to include legends"
                            << std::endl;
                        msg_shown_once = true;
                    }
                    ok = false;
                }
            }

            if (ok) {
                auto legend_it = legend_->begin();
                auto legend_end = legend_->end();
                for (auto child_it = children_.begin();
                     child_it != children_.end(); ++child_it) {
                    const bool no_legend = legend_it == legend_end &&
                                           (*child_it)->display_name().empty();
                    if (no_legend) {
                        continue;
                    }
                    const auto &child = *child_it;
                    if (!first) {
                        plot_command += ", ";
                    } else {
                        first = false;
                    }
                    // https://stackoverflow.com/a/60624922/2983585
                    // http://gnuplot.sourceforge.net/demo_svg_5.5/custom_key.html
                    plot_command += child->legend_string(legend_it, legend_end);
                }
            }
        }

        if (plot_command != "plot ") {
            run_command(plot_command);
        } else {
            run_empty_plot_command();
        }

        // data commands
        for (const auto &child : children_) {
            run_command(child->data_string());
        }

        // unset variables command
        std::string unset_variables_command;
        for (const auto &child : children_) {
            unset_variables_command += child->unset_variables_string();
        }
        run_command(unset_variables_command);
    }

    void axes_type::run_commands() {
        run_colormap_command();
        run_position_margin_command();
        run_title_command();
        run_box_command();
        run_grid_command();
        run_axes_command();
        run_labels_command();
        run_legend_command();
        run_background_command();
        run_plot_objects_command();
    }

    void axes_type::run_draw_commands() {
        run_background_draw_commands();
        run_grid_draw_commands();
        run_box_draw_commands();
        run_axes_draw_commands();
        run_labels_draw_commands();
        run_title_draw_commands();
        run_legend_draw_commands();
        run_plot_objects_draw_commands();
    }

    void axes_type::run_background_draw_commands() {
        auto [w, h, lm, rm, bm, tm] = calculate_margins();
        (void)w;
        (void)h;
        double view_width = parent_->backend_->width();
        double x1 = lm * view_width;
        double x2 = rm * view_width;
        double view_height = parent_->backend_->height();
        double y1 = bm * view_height;
        double y2 = tm * view_height;
        parent_->backend_->draw_rectangle(x1, x2, y1, y2, this->color_);
    }

    void axes_type::run_title_draw_commands() {
        // This should draw the title on top of the axes
    }

    void axes_type::run_box_draw_commands() {
        auto [w, h, lm, rm, bm, tm] = calculate_margins();
        (void)w;
        (void)h;
        double view_width = parent_->backend_->width();
        double x1 = lm * view_width;
        double x2 = rm * view_width;
        double view_height = parent_->backend_->height();
        double y1 = bm * view_height;
        double y2 = tm * view_height;
        const std::array<float, 4> color = {0., 0., 0., 0.};
        std::vector<double> box_xs = {x1, x2, x2, x1, x1};
        std::vector<double> box_ys = {y1, y1, y2, y2, y1};
        parent_->backend_->draw_path(box_xs, box_ys, color);
    }

    void axes_type::run_grid_draw_commands() {}

    void axes_type::run_axes_draw_commands() {
        auto [w, h, lm, rm, bm, tm] = calculate_margins();
        (void)w;
        (void)h;
        double view_width = parent_->backend_->width();
        double view_height = parent_->backend_->height();
        // to draw the x axis we
        // - normalize x values
        // - keep y as absolute values
        auto xlimits = xlim();
        auto ylimits = ylim();
        std::vector<double> cx;
        if (x_axis_.tick_values_manual()) {
            cx = x_axis_.tick_values();
        } else {
            ticks_results xticks_results =
                calcticks(xlimits[0], xlimits[1], true, 1.25, true, false);
            cx = xticks_results.ticks;
        }
        // clamp values outside x limits
        cx = transform(cx, [&](double x) {
            return std::clamp(x, xlimits[0], xlimits[1]);
        });

        // convert x values to viewport range
        double viewport_xmin = lm * view_width;
        double viewport_xmax = rm * view_width;
        double viewport_xrange = viewport_xmax - viewport_xmin;

        auto xrange = xlimits[1] - xlimits[0];
        for (auto &v : cx) {
            v -= xlimits[0];
            v /= xrange;
            v *= viewport_xrange;
            v += viewport_xmin;
        }

        // get viewport range for y axis
        double viewport_ymin = bm * view_height;
        double viewport_ymax = tm * view_height;
        double viewport_yrange = viewport_ymax - viewport_ymin;

        // The meaning of tick length is proportional
        // to the largest of x and y viewport size.
        // This unit makes the tick sizes look the same on x and y
        // while still proportional to the axes object.
        double tick_length_multiplier =
            std::max(viewport_xrange, viewport_yrange) * 0.015;

        // draw a path for each x tick
        double xtick_length_multiplier =
            std::min(tick_length_multiplier, viewport_xrange);
        for (auto &v : cx) {
            parent_->backend_->draw_path(
                {v, v},
                {viewport_ymin, viewport_ymin + x_axis_.tick_length() *
                                                    xtick_length_multiplier},
                x_axis_.color_);
            parent_->backend_->draw_path(
                {v, v},
                {viewport_ymax, viewport_ymax - x_axis_.tick_length() *
                                                    xtick_length_multiplier},
                x_axis_.color_);
        }

        // to draw the y axis we
        // - normalize y values
        // - keep x as absolute values
        std::vector<double> cy;
        if (y_axis_.tick_values_manual()) {
            cy = y_axis_.tick_values();
        } else {
            ticks_results yticks_results =
                calcticks(ylimits[0], ylimits[1], false, 1.25, true, false);
            cy = yticks_results.ticks;
        }
        // clamp values outside x limits
        cy = transform(cy, [&](double x) {
            return std::clamp(x, ylimits[0], ylimits[1]);
        });

        // convert y values to viewport range
        auto yrange = ylimits[1] - ylimits[0];
        for (auto &v : cy) {
            v -= ylimits[0];
            v /= yrange;
            v *= viewport_yrange;
            v += viewport_ymin;
        }

        // draw a path for each y tick
        double ytick_length_multiplier =
            std::min(tick_length_multiplier, viewport_yrange);
        for (auto &v : cy) {
            parent_->backend_->draw_path(
                {viewport_xmin, viewport_xmin + y_axis_.tick_length() *
                                                    ytick_length_multiplier},
                {v, v}, y_axis_.color_);
            parent_->backend_->draw_path(
                {viewport_xmax, viewport_xmax - y_axis_.tick_length() *
                                                    ytick_length_multiplier},
                {v, v}, y_axis_.color_);
        }
    }

    void axes_type::run_labels_draw_commands() {}

    void axes_type::run_legend_draw_commands() {}

    void axes_type::run_plot_objects_draw_commands() {
        for (const auto &child : children_) {
            child->run_draw_commands();
        }
    }

    void axes_type::run_command(const std::string &command) {
        parent_->run_command("    " + command);
    }

    void axes_type::include_comment(const std::string &command) {
        parent_->run_command("    # " + command);
    }

    bool axes_type::is_3d() {
        return children_.empty() ? false : children_[0]->is_3d();
    }

    bool axes_type::is_3d_map() {
        return children_.empty() ? false : children_[0]->is_3d_map();
    }

    bool axes_type::is_2d() {
        return children_.empty() ? true : children_[0]->is_2d();
    }

    bool axes_type::is_polar() {
        return children_.empty() ? false : children_[0]->is_polar();
    }

    axes_handle axes_type::copy(figure_handle parent) {
        axes_handle new_axes = std::make_shared<class axes_type>(*this);
        new_axes->parent_ = parent.get();
        new_axes->parent_->add_axes(new_axes, false, false);
        return new_axes;
    }

    std::array<double, 4> axes_type::child_limits() const {
        if (children_.empty()) {
            return {-10, +10, -10, +10};
        }
        auto xminit =
            std::min_element(children_.begin(), children_.end(),
                             [](axes_object_handle a, axes_object_handle b) {
                                 return a->xmin() < b->xmin();
                             });
        auto xmaxit =
            std::max_element(children_.begin(), children_.end(),
                             [](axes_object_handle a, axes_object_handle b) {
                                 return a->xmax() < b->xmax();
                             });
        auto yminit =
            std::min_element(children_.begin(), children_.end(),
                             [](axes_object_handle a, axes_object_handle b) {
                                 return a->ymin() < b->ymin();
                             });
        auto ymaxit =
            std::max_element(children_.begin(), children_.end(),
                             [](axes_object_handle a, axes_object_handle b) {
                                 return a->ymax() < b->ymax();
                             });
        return {(*xminit)->xmin(), (*xmaxit)->xmax(), (*yminit)->ymin(),
                (*ymaxit)->ymax()};
    }

    axes_type::axes_type() : axes_type(nullptr) {
        t_axis_.tick_label_format_ = "%gº";
        t_axis_.tick_values_ = iota(0, 30, 330);
        t_axis_.tick_values_automatic_ = false;
    }

    axes_type::axes_type(class figure_type *parent)
        : axes_type(parent, default_axes_position) {
        t_axis_.tick_label_format_ = "%gº";
        t_axis_.tick_values_ = iota(0, 30, 330);
        t_axis_.tick_values_automatic_ = false;
        font_ = parent->font();
    }

    axes_type::axes_type(class figure_type *parent,
                         std::array<float, 4> position)
        : x_axis_(this, -10, +10, true), x2_axis_(this, inf, inf, false),
          y_axis_(this, inf, inf, true), y2_axis_(this, inf, inf, false),
          z_axis_(this, inf, inf, true), position_(position), parent_(parent) {
        grid_line_style_.color({0.85f, 0.15f, 0.15f, 0.15f});
        minor_grid_line_style_.color({0.9f, 0.1f, 0.1f, 0.1f});
        t_axis_.tick_label_format_ = "%gº";
        t_axis_.tick_values_ = iota(0, 30, 330);
        t_axis_.tick_values_automatic_ = false;
        font_ = parent->font();
    }

    axes_type::axes_type(figure_handle parent) : axes_type(parent.get()) {
        t_axis_.tick_label_format_ = "%gº";
        t_axis_.tick_values_ = iota(0, 30, 330);
        t_axis_.tick_values_automatic_ = false;
        font_ = parent->font();
    }

    axes_type::axes_type(figure_handle parent, std::array<float, 4> position)
        : axes_type(parent.get(), position) {
        t_axis_.tick_label_format_ = "%gº";
        t_axis_.tick_values_ = iota(0, 30, 330);
        t_axis_.tick_values_automatic_ = false;
        font_ = parent->font();
    }

    const class axis_type &axes_type::x_axis() const { return x_axis_; }

    class axis_type &axes_type::x_axis() {
        return x_axis_;
    }

    const class axis_type &axes_type::x2_axis() const { return x2_axis_; }

    class axis_type &axes_type::x2_axis() {
        return x2_axis_;
    }

    const class axis_type &axes_type::y_axis() const { return y_axis_; }

    const std::string &axes_type::xlabel() const { return x_axis_.label(); }

    void axes_type::xlabel(std::string_view str) {
        x_axis_.label(str);
        touch();
    }

    const std::string &axes_type::x2label() const { return x2_axis_.label(); }

    void axes_type::x2label(std::string_view str) {
        x2_axis_.label(str);
        touch();
    }

    const std::string &axes_type::ylabel() const { return y_axis_.label(); }

    void axes_type::ylabel(std::string_view str) {
        y_axis_.label(str);
        touch();
    }

    const std::string &axes_type::xtickformat() const {
        return x_axis_.tick_label_format();
    }

    void axes_type::xtickformat(std::string_view str) {
        x_axis_.tick_label_format(str);
    }

    const std::string &axes_type::x2tickformat() const {
        return x2_axis_.tick_label_format();
    }

    void axes_type::x2tickformat(std::string_view str) {
        x2_axis_.tick_label_format(str);
    }

    const std::string &axes_type::ytickformat() const {
        return y_axis_.tick_label_format();
    }

    void axes_type::ytickformat(std::string_view str) {
        y_axis_.tick_label_format(str);
    }

    const std::string &axes_type::y2tickformat() const {
        return y2_axis_.tick_label_format();
    }

    void axes_type::y2tickformat(std::string_view str) {
        y2_axis_.tick_label_format(str);
    }

    const std::string &axes_type::ztickformat() const {
        return z_axis_.tick_label_format();
    }

    void axes_type::ztickformat(std::string_view str) {
        z_axis_.tick_label_format(str);
    }

    const std::string &axes_type::rtickformat() const {
        return r_axis_.tick_label_format();
    }

    void axes_type::rtickformat(std::string_view str) {
        r_axis_.tick_label_format(str);
    }

    const std::string &axes_type::y2label() const { return y2_axis_.label(); }

    void axes_type::y2label(std::string_view str) {
        y2_axis_.label(str);
        touch();
    }

    const std::string &axes_type::zlabel() const { return z_axis_.label(); }

    void axes_type::zlabel(std::string_view str) {
        z_axis_.label(str);
        touch();
    }

    const std::string &axes_type::rlabel() const { return r_axis_.label(); }

    void axes_type::rlabel(std::string_view str) {
        r_axis_.label(str);
        touch();
    }

    const std::vector<double> &axes_type::xticks() const {
        return x_axis_.tick_values();
    }

    void axes_type::xticks(const std::vector<double> &ticks) {
        x_axis_.tick_values(ticks);
    }

    const std::vector<double> &axes_type::x2ticks() const {
        return x2_axis_.tick_values();
    }

    void axes_type::x2ticks(const std::vector<double> &ticks) {
        x2_axis_.tick_values(ticks);
    }

    const std::vector<double> &axes_type::yticks() const {
        return y_axis_.tick_values();
    }

    void axes_type::yticks(const std::vector<double> &ticks) {
        y_axis_.tick_values(ticks);
    }

    const std::vector<double> &axes_type::y2ticks() const {
        return y2_axis_.tick_values();
    }

    void axes_type::y2ticks(const std::vector<double> &ticks) {
        y2_axis_.tick_values(ticks);
    }

    const std::vector<double> &axes_type::zticks() const {
        return z_axis_.tick_values();
    }

    void axes_type::zticks(const std::vector<double> &ticks) {
        z_axis_.tick_values(ticks);
    }

    const std::vector<double> &axes_type::rticks() const {
        return r_axis_.tick_values();
    }

    void axes_type::rticks(const std::vector<double> &ticks) {
        r_axis_.tick_values(ticks);
    }

    class axis_type &axes_type::y_axis() {
        return y_axis_;
    }

    const class axis_type &axes_type::y2_axis() const { return y2_axis_; }

    class axis_type &axes_type::y2_axis() {
        return y2_axis_;
    }

    const class axis_type &axes_type::z_axis() const { return z_axis_; }

    class axis_type &axes_type::z_axis() {
        return z_axis_;
    }

    const class axis_type &axes_type::r_axis() const { return r_axis_; }

    class axis_type &axes_type::r_axis() {
        return r_axis_;
    }

    const std::string &axes_type::font() const {
        if (font_) {
            return *font_;
        } else {
            return parent_->font();
        }
    }

    void axes_type::font(std::string_view font) {
        font_ = font;
        touch();
    }

    float axes_type::font_size() const {
        if (font_size_) {
            return *font_size_;
        } else {
            return parent_->font_size();
        }
    }

    void axes_type::font_size(float font_size) {
        font_size_ = font_size;
        touch();
    }

    const std::string &axes_type::font_weight() const { return font_weight_; }

    void axes_type::font_weight(std::string_view font_weight) {
        font_weight_ = font_weight;
        touch();
    }

    float axes_type::title_font_size_multiplier() const {
        return title_font_size_multiplier_;
    }

    void
    axes_type::title_font_size_multiplier(float title_font_size_multiplier) {
        title_font_size_multiplier_ = title_font_size_multiplier;
        touch();
    }

    const std::string &axes_type::title_font_weight() const {
        return title_font_weight_;
    }

    void axes_type::title_font_weight(std::string_view title_font_weight) {
        title_font_weight_ = title_font_weight;
        touch();
    }

    const color_array &axes_type::color() const { return color_; }

    void axes_type::color(const color_array &color) {
        color_ = color;
        touch();
    }

    void axes_type::color(const std::array<float, 3> &color) {
        color_ = {0, color[0], color[1], color[2]};
        touch();
    }

    void axes_type::color(std::string_view c) { color(string_to_color(c)); }

    void axes_type::color(const enum color &c) { color(to_array(c)); }

    const std::vector<color_array> &axes_type::colororder() const {
        return colororder_;
    }

    void axes_type::colororder(const std::vector<color_array> &colororder) {
        colororder_ = colororder;
        touch();
    }

    size_t axes_type::colororder_index() const { return colororder_index_; }

    void axes_type::colororder_index(size_t colororder_index) {
        colororder_index_ = colororder_index;
    }

    void axes_type::bump_colororder_index() {
        colororder_index_ = (colororder_index_ + 1) % colororder_.size();
    }

    color_array axes_type::get_color_and_bump() {
        color_array r = colororder_[colororder_index_];
        bump_colororder_index();
        return r;
    }

    const color_array axes_type::grid_color() const {
        return grid_line_style_.color();
    }

    void axes_type::grid_color(const color_array &grid_color) {
        grid_line_style_.color(grid_color);
        touch();
    }

    float axes_type::grid_alpha() const { return grid_line_style_.alpha(); }

    void axes_type::grid_alpha(float grid_alpha) {
        grid_line_style_.alpha(grid_alpha);
        touch();
    }

    bool axes_type::handle_visibility() const { return handle_visibility_; }

    void axes_type::handle_visibility(bool handle_visibility) {
        handle_visibility_ = handle_visibility;
        touch();
    }

    float axes_type::minor_grid_alpha() const {
        return minor_grid_line_style_.alpha();
    }

    void axes_type::minor_grid_alpha(float minor_grid_alpha) {
        minor_grid_line_style_.alpha(minor_grid_alpha);
        touch();
    }

    const color_array axes_type::minor_grid_color() const {
        return minor_grid_line_style_.color();
    }

    void axes_type::minor_grid_color(const color_array &minor_grid_color) {
        minor_grid_line_style_.color(minor_grid_color);
        touch();
    }

    line_spec axes_type::minor_grid_line_style() const {
        return minor_grid_line_style_;
    }

    void axes_type::minor_grid_line_style(line_spec minor_grid_line_style) {
        minor_grid_line_style_ = minor_grid_line_style;
        touch();
    }

    line_spec axes_type::grid_line_style() const { return grid_line_style_; }

    void axes_type::grid_line_style(line_spec grid_line_style) {
        grid_line_style_ = grid_line_style;
        touch();
    }

    const std::string &axes_type::line_style_order() const {
        return line_style_order_;
    }

    void axes_type::line_style_order(std::string_view line_style_order) {
        line_style_order_ = line_style_order;
        touch();
    }

    size_t axes_type::line_style_order_index() const {
        return line_style_order_index_;
    }

    void axes_type::line_style_order_index(size_t line_style_order_index) {
        line_style_order_index_ = line_style_order_index;
        touch();
    }

    float axes_type::line_width() const { return line_width_; }

    void axes_type::line_width(float line_width) {
        line_width_ = line_width;
        touch();
    }

    bool axes_type::next_plot_replace() const { return next_plot_replace_; }

    void axes_type::next_plot_replace(bool next_plot_replace) {
        if (next_plot_replace != next_plot_replace_) {
            next_plot_replace_ = next_plot_replace;
        }
    }

    bool axes_type::hold() const { return !next_plot_replace(); }

    void axes_type::hold(bool v) { next_plot_replace(!v); }

    const std::vector<std::shared_ptr<class axes_object>> &
    axes_type::children() const {
        return children_;
    }

    void axes_type::children(
        const std::vector<std::shared_ptr<axes_object>> &children) {
        children_ = children;
        touch();
    }

    const legend_handle &axes_type::legend() const { return legend_; }

    void axes_type::legend(const legend_handle &legend) {
        legend_ = legend;
        touch();
    }

    void axes_type::legend(const std::vector<std::string> &names) {
        if (legend_ == nullptr) {
            legend_.reset(new class legend(this, names));
        } else {
            legend_->strings(names);
        }
        touch();
    }

    const class figure_type *axes_type::parent() const { return parent_; }

    class figure_type *axes_type::parent() {
        return parent_;
    }

    void axes_type::parent(class figure_type *p) {
        parent_ = p;
        touch();
    }

    const std::string &axes_type::title() const { return title_; }

    void axes_type::title(std::string_view title) {
        title_ = title;
        title_visible_ = !title.empty();
        if (parent_->children_.size() == 1 && !parent_->quiet_mode()) {
            run_title_command();
            run_command("replot");
            parent_->flush_commands();
        } else {
            touch();
        }
    }

    const std::array<float, 4> &axes_type::position() const {
        return position_;
    }

    void axes_type::position(const std::array<float, 4> &position) {
        position_ = position;
        touch();
    }

    const float &axes_type::x_origin() const { return position_[0]; }

    void axes_type::x_origin(float x) {
        position_[0] = x;
        touch();
    }

    const float &axes_type::y_origin() const { return position_[1]; }

    void axes_type::y_origin(float y) {
        position_[1] = y;
        touch();
    }

    const float &axes_type::width() const { return position_[2]; }

    void axes_type::width(float w) {
        position_[2] = w;
        touch();
    }

    const float &axes_type::height() const { return position_[3]; }

    void axes_type::height(float h) {
        position_[3] = h;
        touch();
    }

    bool axes_type::box() const { return box_; }

    void axes_type::box(bool box) {
        if (box != box_) {
            box_ = box;
            touch();
        }
    }

    bool axes_type::box_full() const { return box_full_; }

    void axes_type::box_full(bool box_full) {
        if (box_full != box_full_) {
            box_full_ = box_full;
            if (box_full) {
                box_ = true;
            }
            touch();
        }
    }

    bool axes_type::title_visible() const { return title_visible_; }

    void axes_type::title_visible(bool title_visible) {
        title_visible_ = title_visible;
        touch();
    }

    bool axes_type::visible() const { return visible_; }

    void axes_type::visible(bool visible) {
        visible_ = visible;
        touch();
    }

    void axes_type::limits_mode(keyword_automatic_type) {
        limits_mode_automatic(true);
        touch();
    }

    void axes_type::limits_mode(keyword_manual_type) {
        limits_mode_automatic(false);
        touch();
    }

    void axes_type::limits_mode_automatic(bool v) {
        x_axis_.limits_mode_auto(v);
        y_axis_.limits_mode_auto(v);
        y2_axis_.limits_mode_auto(v);
        z_axis_.limits_mode_auto(v);
        touch();
    }

    bool axes_type::limits_mode_automatic() const {
        return x_axis_.limits_mode_auto() && y_axis_.limits_mode_auto() &&
               y2_axis_.limits_mode_auto() && z_axis_.limits_mode_auto();
    }

    void axes_type::limits_mode_manual(bool v) {
        x_axis_.limits_mode_auto(!v);
        y_axis_.limits_mode_auto(!v);
        y2_axis_.limits_mode_auto(!v);
        z_axis_.limits_mode_auto(!v);
        touch();
    }

    bool axes_type::limits_mode_manual() const {
        return x_axis_.limits_mode_manual() && y_axis_.limits_mode_manual() &&
               y2_axis_.limits_mode_manual() && z_axis_.limits_mode_manual();
    }

    bool axes_type::grid() const { return x_grid_ && y_grid_ && z_grid_; }

    void axes_type::grid(bool grid) {
        x_grid_ = grid;
        x_user_grid_ = true;
        y_grid_ = grid;
        y_user_grid_ = true;
        z_grid_ = grid;
        z_user_grid_ = true;
        touch();
    }

    bool axes_type::minor_grid() const {
        return x_minor_grid_ && y_minor_grid_ && z_minor_grid_;
    }

    void axes_type::minor_grid(bool minor_grid) {
        x_minor_grid_ = minor_grid;
        x_user_grid_ = true;
        y_minor_grid_ = minor_grid;
        y_user_grid_ = true;
        z_minor_grid_ = minor_grid;
        z_user_grid_ = true;
        touch();
    }

    bool axes_type::x_grid() const { return x_grid_; }

    void axes_type::x_grid(bool x_grid) {
        x_grid_ = x_grid;
        x_user_grid_ = true;
        touch();
    }

    bool axes_type::x_minor_grid() const { return x_minor_grid_; }

    void axes_type::x_minor_grid(bool x_minor_grid) {
        x_minor_grid_ = x_minor_grid;
    }

    bool axes_type::y_grid() const { return y_grid_; }

    void axes_type::y_grid(bool y_grid) {
        y_grid_ = y_grid;
        x_user_grid_ = true;
        touch();
    }

    bool axes_type::y_minor_grid() const { return y_minor_grid_; }

    void axes_type::y_minor_grid(bool y_minor_grid) {
        y_minor_grid_ = y_minor_grid;
    }

    bool axes_type::z_grid() const { return z_grid_; }

    void axes_type::z_grid(bool z_grid) {
        z_grid_ = z_grid;
        x_user_grid_ = true;
        touch();
    }

    bool axes_type::z_minor_grid() const { return z_minor_grid_; }

    void axes_type::z_minor_grid(bool z_minor_grid) {
        z_minor_grid_ = z_minor_grid;
    }

    bool axes_type::axes_aspect_ratio_auto() const {
        return axes_aspect_ratio_auto_;
    }

    void axes_type::axes_aspect_ratio_auto(bool axes_aspect_ratio_auto) {
        axes_aspect_ratio_auto_ = axes_aspect_ratio_auto;
        touch();
    }

    void axes_type::axes_aspect_ratio(float axes_aspect_ratio) {
        axes_aspect_ratio_ = axes_aspect_ratio;
        axes_aspect_ratio_auto_ = false;
        touch();
    }

    float axes_type::axes_aspect_ratio() const { return axes_aspect_ratio_; }

    float axes_type::azimuth() const { return azimuth_; }

    void axes_type::azimuth(float azimuth) {
        if (azimuth != azimuth_) {
            azimuth_ = azimuth;
            if (parent_->children_.size() == 1 && !parent_->quiet_mode()) {
                // instead of touch(), we just run the view command and replot
                float gnuplot_azimuth_ = azimuth_ + 360;
                while (gnuplot_azimuth_ < 0) {
                    gnuplot_azimuth_ += 360;
                }
                while (gnuplot_azimuth_ > 360) {
                    gnuplot_azimuth_ -= 360;
                }
                run_command("set view ," + num2str(gnuplot_azimuth_));
                run_command("replot");
                parent_->flush_commands();
            } else {
                touch();
            }
        }
    }

    float axes_type::elevation() const { return elevation_; }

    void axes_type::elevation(float elevation) {
        if (elevation != elevation_) {
            elevation_ = elevation;
            if (parent_->children_.size() == 1 && !parent_->quiet_mode()) {
                // instead of touch(), we just run the view command and replot
                float gnuplot_elevation_ = 90 - elevation_;
                while (gnuplot_elevation_ < 0) {
                    gnuplot_elevation_ += 180;
                }
                while (gnuplot_elevation_ > 180) {
                    gnuplot_elevation_ -= 180;
                }
                run_command("set view " + num2str(gnuplot_elevation_));
                run_command("replot");
                parent_->flush_commands();
            } else {
                touch();
            }
        }
    }

    std::pair<float, float> axes_type::view() const {
        return std::make_pair(azimuth_, elevation_);
    }

    void axes_type::view(float azimuth, float elevation) {
        if (azimuth != azimuth_ || elevation != elevation_) {
            azimuth_ = azimuth;
            elevation_ = elevation;
            if (parent_->children_.size() == 1 && !parent_->quiet_mode()) {
                // instead of touch(), we just run the view command and replot
                float gnuplot_azimuth_ = azimuth_ + 360;
                while (gnuplot_azimuth_ < 0) {
                    gnuplot_azimuth_ += 360;
                }
                while (gnuplot_azimuth_ > 360) {
                    gnuplot_azimuth_ -= 360;
                }

                float gnuplot_elevation_ = 90 - elevation_;
                while (gnuplot_elevation_ < 0) {
                    gnuplot_elevation_ += 180;
                }
                while (gnuplot_elevation_ > 180) {
                    gnuplot_elevation_ -= 180;
                }

                run_command("set view " + num2str(gnuplot_elevation_) + "," +
                            num2str(gnuplot_azimuth_));
                const bool is_map = azimuth_ == 0 && elevation_ == 90;
                if (is_map) {
                    touch();
                } else {
                    run_command("replot");
                    parent_->flush_commands();
                }
            } else {
                touch();
            }
        }
    }

    void axes_type::rotate(float azimuth, float elevation) {
        azimuth_ += azimuth;
        elevation_ += elevation;
        touch();
    }

    const std::vector<std::vector<double>> &axes_type::colormap() const {
        return colormap_;
    }

    color_array axes_type::colormap_interpolation(double value, double min,
                                                  double max) {
        return ::matplot::colormap_interpolation(value, min, max, colormap_);
    }

    void axes_type::colormap(const std::vector<std::vector<double>> &colormap) {
        colormap_ = colormap;
        touch();
    }

    bool axes_type::color_box() const { return cb_axis_.visible(); }

    void axes_type::color_box(bool v) {
        cb_axis_.visible(v);
        touch();
    }

    bool axes_type::color_box_log_scale() const {
        return cb_axis_.scale() == axis_type::axis_scale::log;
    }

    void axes_type::color_box_log_scale(bool v) {
        cb_axis_.scale(v ? axis_type::axis_scale::log
                         : axis_type::axis_scale::linear);
        touch();
    }

    bool axes_type::grid_front() const { return grid_front_; }

    void axes_type::grid_front(bool grid_front) {
        grid_front_ = grid_front;
        touch();
    }

    bool axes_type::r_grid() const { return r_grid_; }

    void axes_type::r_grid(bool r_grid) {
        r_grid_ = r_grid;
        r_user_grid_ = true;
        touch();
    }

    bool axes_type::r_minor_grid() const { return r_minor_grid_; }

    void axes_type::r_minor_grid(bool r_minor_grid) {
        r_minor_grid_ = r_minor_grid;
    }

    const class axis_type &axes_type::t_axis() const { return t_axis_; }

    class axis_type &axes_type::t_axis() {
        return t_axis_;
    }

    void axes_type::t_axis(const class axis_type &t_axis) {
        t_axis_ = t_axis;
        touch();
    }

    size_t axes_type::max_colors() const { return max_colors_; }

    void axes_type::max_colors(size_t max_colors) {
        max_colors_ = max_colors;
        touch();
    }

    const std::pair<double, double> axes_type::color_box_range() const {
        auto a = cb_axis_.limits();
        return std::make_pair(a[0], a[1]);
    }

    void axes_type::color_box_range(const std::pair<double, double> &r) {
        cb_axis_.limits({r.first, r.second});
        touch();
    }

    void axes_type::color_box_range(double cb_min, double cb_max) {
        color_box_range({cb_min, cb_max});
    }

    const std::vector<std::string> &axes_type::xticklabels() const {
        return x_axis().ticklabels();
    }

    void axes_type::xticklabels(const std::vector<std::string> &ticks) {
        x_axis().ticklabels(ticks);
        touch();
    }

    const std::vector<std::string> &axes_type::x2ticklabels() const {
        return x2_axis().ticklabels();
    }

    void axes_type::x2ticklabels(const std::vector<std::string> &ticks) {
        x2_axis().ticklabels(ticks);
        touch();
    }

    const std::vector<std::string> &axes_type::yticklabels() const {
        return y_axis().ticklabels();
    }

    void axes_type::yticklabels(const std::vector<std::string> &ticks) {
        y_axis().ticklabels(ticks);
        touch();
    }

    const std::vector<std::string> &axes_type::y2ticklabels() const {
        return y2_axis().ticklabels();
    }

    void axes_type::y2ticklabels(const std::vector<std::string> &ticks) {
        y2_axis().ticklabels(ticks);
        touch();
    }

    const std::vector<std::string> &axes_type::zticklabels() const {
        return z_axis().ticklabels();
    }

    void axes_type::zticklabels(const std::vector<std::string> &ticks) {
        z_axis().ticklabels(ticks);
        touch();
    }

    const std::vector<std::string> &axes_type::rticklabels() const {
        return r_axis().ticklabels();
    }

    void axes_type::rticklabels(const std::vector<std::string> &ticks) {
        r_axis().ticklabels(ticks);
        touch();
    }

    void axes_type::xtickangle(double degrees) {
        x_axis().tickangle(static_cast<float>(degrees));
    }

    void axes_type::x2tickangle(double degrees) {
        x2_axis().tickangle(static_cast<float>(degrees));
    }

    void axes_type::ytickangle(double degrees) {
        y_axis().tickangle(static_cast<float>(degrees));
    }

    void axes_type::y2tickangle(double degrees) {
        y2_axis().tickangle(static_cast<float>(degrees));
    }

    void axes_type::ztickangle(double degrees) {
        z_axis().tickangle(static_cast<float>(degrees));
    }

    void axes_type::rtickangle(double degrees) {
        r_axis().tickangle(static_cast<float>(degrees));
    }

    double axes_type::xtickangle() { return x_axis().tickangle(); }

    double axes_type::x2tickangle() { return x2_axis().tickangle(); }

    double axes_type::ytickangle() { return y_axis().tickangle(); }

    double axes_type::y2tickangle() { return y2_axis().tickangle(); }

    double axes_type::ztickangle() { return z_axis().tickangle(); }

    double axes_type::rtickangle() { return r_axis().tickangle(); }

    const color_array &axes_type::title_color() const { return title_color_; }

    void axes_type::title_color(const color_array &title_color) {
        title_color_ = title_color;
        touch();
    }

    bool axes_type::title_enhanced() const { return title_enhanced_; }

    void axes_type::title_enhanced(bool title_enhanced) {
        title_enhanced_ = title_enhanced;
        touch();
    }

    std::array<double, 2> axes_type::xlim() const {
        if (x_axis_.limits_mode_auto()) {
            auto [xmin, xmax, ymin, ymax] = this->child_limits();
            (void)ymin;
            (void)ymax;
            return std::array<double, 2>{xmin, xmax};
        } else {
            return x_axis().limits();
        }
    }

    void axes_type::xlim(const std::array<double, 2> &lim) {
        x_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::x2lim() const {
        return x2_axis().limits();
    }

    void axes_type::x2lim(const std::array<double, 2> &lim) {
        x2_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::ylim() const {
        if (y_axis_.limits_mode_auto()) {
            auto [xmin, xmax, ymin, ymax] = this->child_limits();
            (void)xmin;
            (void)xmax;
            return std::array<double, 2>{ymin, ymax};
        } else {
            return y_axis().limits();
        }
    }

    void axes_type::ylim(const std::array<double, 2> &lim) {
        y_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::y2lim() const {
        return y2_axis().limits();
    }

    void axes_type::y2lim(const std::array<double, 2> &lim) {
        y2_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::zlim() const { return z_axis().limits(); }

    void axes_type::zlim(const std::array<double, 2> &lim) {
        z_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::rlim() const { return r_axis().limits(); }

    void axes_type::rlim(const std::array<double, 2> &lim) {
        r_axis().limits(lim);
        touch();
    }

    std::array<double, 2> axes_type::tlim() const { return t_axis().limits(); }

    void axes_type::tlim(const std::array<double, 2> &lim) {
        t_axis().limits(lim);
        touch();
    }

    void axes_type::xlim(keyword_automatic_type) {
        x_axis().limits_mode_manual(false);
    }

    void axes_type::xlim(keyword_manual_type) {
        x_axis().limits_mode_manual(true);
    }

    void axes_type::x2lim(keyword_automatic_type) {
        x2_axis().limits_mode_manual(false);
    }

    void axes_type::x2lim(keyword_manual_type) {
        x2_axis().limits_mode_manual(true);
    }

    void axes_type::ylim(keyword_automatic_type) {
        y_axis().limits_mode_manual(false);
    }

    void axes_type::ylim(keyword_manual_type) {
        y_axis().limits_mode_manual(true);
    }

    void axes_type::y2lim(keyword_automatic_type) {
        y2_axis().limits_mode_manual(false);
    }

    void axes_type::y2lim(keyword_manual_type) {
        y2_axis().limits_mode_manual(true);
    }

    void axes_type::zlim(keyword_automatic_type) {
        z_axis().limits_mode_manual(false);
    }

    void axes_type::zlim(keyword_manual_type) {
        z_axis().limits_mode_manual(true);
    }

    void axes_type::rlim(keyword_automatic_type) {
        r_axis().limits_mode_manual(false);
    }

    void axes_type::rlim(keyword_manual_type) {
        r_axis().limits_mode_manual(true);
    }

    void axes_type::tlim(keyword_automatic_type) {
        t_axis().limits_mode_manual(false);
    }

    void axes_type::tlim(keyword_manual_type) {
        t_axis().limits_mode_manual(true);
    }

    void axes_type::clear() {
        children_.clear();
        legend_.reset();
        colororder_index_ = 0;
        x_axis_.limits_mode_auto(true);
        y_axis_.limits_mode_auto(true);
        y2_axis_.limits_mode_auto(true);
        z_axis_.limits_mode_auto(true);
        axes_type::x_axis().reverse(false);
        axes_type::y_axis().reverse(false);
        minor_grid(false);
        touch();
    }

    const class axis_type &axes_type::cb_axis() const { return cb_axis_; }

    class axis_type &axes_type::cb_axis() {
        return cb_axis_;
    }

    std::array<double, 2> axes_type::cblim() const { return cb_axis_.limits(); }

    void axes_type::cblim(const std::array<double, 2> &a) {
        cb_axis_.limits(a);
    }

    void axes_type::cblim(keyword_automatic_type) {
        cb_axis_.limits_mode_auto(true);
    }

    void axes_type::cblim(keyword_manual_type) {
        cb_axis_.limits_mode_manual(true);
    }

    const std::string &axes_type::cblabel() const { return cb_axis_.label(); }

    void axes_type::cblabel(std::string_view str) { cb_axis_.label(str); }

    const std::string &axes_type::cbtickformat() const {
        return cb_axis_.tick_label_format();
    }

    void axes_type::cbtickformat(std::string_view str) {
        cb_axis_.tick_label_format(str);
    }

    const std::vector<double> &axes_type::cbticks() const {
        return cb_axis_.tick_values();
    }

    void axes_type::cbticks(const std::vector<double> &ticks) {
        cb_axis_.tick_values(ticks);
    }

    const std::vector<std::string> &axes_type::cbticklabels() const {
        return cb_axis_.ticklabels();
    }

    void axes_type::cbticklabels(const std::vector<std::string> &labels) {
        cb_axis_.ticklabels(labels);
    }

    void axes_type::cbtickangle(double degrees) {
        cb_axis_.tickangle(static_cast<float>(degrees));
    }

    double axes_type::cbtickangle() { return cb_axis_.tickangle(); }

    bool axes_type::cb_vertical() const { return cb_vertical_; }

    void axes_type::cb_vertical(bool cb_vertical) {
        cb_vertical_ = cb_vertical;
        touch();
    }

    const std::array<float, 4> &axes_type::cb_position() const {
        return cb_position_;
    }

    void axes_type::cb_position(const std::array<float, 4> &cb_position) {
        cb_position_ = cb_position;
        cb_vertical_ = cb_position_[2] <= cb_position_[3];
        touch();
    }

    bool axes_type::cb_inside() const { return cb_inside_; }

    void axes_type::cb_inside(bool cb_inside) {
        cb_inside_ = cb_inside;
        touch();
    }

    class axes_silencer {
      public:
        explicit axes_silencer(class axes_type *ax)
            : ax_(ax), previous_quiet_mode_(ax->parent()->quiet_mode()) {
            ax_->parent()->quiet_mode(true);
        }

        ~axes_silencer() {
            ax_->parent()->quiet_mode(previous_quiet_mode_);
            if (!previous_quiet_mode_) {
                ax_->draw();
            }
        }

      private:
        class axes_type *ax_;

        bool previous_quiet_mode_;
    };

    line_handle axes_type::plot(const std::vector<double> &x,
                                const std::vector<double> &y,
                                std::string_view line_spec) {
        axes_silencer s{this};
        line_handle l = std::make_shared<class line>(this, x, y, line_spec);
        this->emplace_object(l);
        return l;
    }

    line_handle axes_type::plot(const std::vector<double> &y,
                                std::string_view line_spec) {
        axes_silencer s{this};
        line_handle l = std::make_shared<class line>(this, y, line_spec);
        this->emplace_object(l);
        return l;
    }

    std::vector<line_handle>
    axes_type::plot(const std::vector<double> &x,
                    const std::vector<std::vector<double>> &Y,
                    std::string_view line_spec) {
        axes_silencer s{this};
        std::vector<line_handle> hs;
        bool previous_replace_status = this->next_plot_replace();
        for (const auto &y : Y) {
            hs.emplace_back(this->plot(x, y, line_spec));
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_replace_status);
        return hs;
    }

    std::vector<line_handle>
    axes_type::plot(const std::vector<std::vector<double>> &Y,
                    std::string_view line_spec) {
        axes_silencer s{this};
        std::vector<line_handle> hs;
        bool previous_replace_status = this->next_plot_replace();
        for (const auto &y : Y) {
            hs.emplace_back(this->plot(y, line_spec));
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_replace_status);
        return hs;
    }

    std::vector<line_handle>
    axes_type::rgbplot(const std::vector<std::vector<double>> &map) {
        axes_silencer s{this};
        // Process data
        auto rgb = transpose(map);
        auto r = plot(rgb[0], "r-");
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);
        auto g = plot(rgb[1], "g-");
        auto b = plot(rgb[2], "b-");
        // If nothing uses the palette, we can't show it
        // Create markers using it on the first points
        double n = static_cast<double>(map.size());
        auto palette_enabler =
            plot(vector_1d{1, 1, 1, n, n, n},
                 vector_1d{rgb[0].front(), rgb[1].front(), rgb[2].front(),
                           rgb[0].back(), rgb[1].back(), rgb[2].back()},
                 ".");
        palette_enabler->marker_colors({0., 0., 0., 1., 1., 1.});
        this->next_plot_replace(p2);
        return {r, g, b, palette_enabler};
    }

    line_handle axes_type::plot3(const std::vector<double> &x,
                                 const std::vector<double> &y,
                                 const std::vector<double> &z,
                                 std::string_view line_spec) {
        axes_silencer s{this};
        line_handle l = std::make_shared<class line>(this, x, y, z, line_spec);
        this->emplace_object(l);
        return l;
    }

    std::vector<line_handle>
    axes_type::plot3(const std::vector<std::vector<double>> &X,
                     const std::vector<std::vector<double>> &Y,
                     const std::vector<double> &z, std::string_view line_spec) {
        axes_silencer s{this};
        auto it_x = X.begin();
        auto it_y = Y.begin();
        bool previous_status = this->next_plot_replace();
        this->next_plot_replace(false);
        std::vector<line_handle> ls;
        while (it_x != X.end() && it_y != Y.end()) {
            ls.emplace_back(this->plot3(*it_x, *it_y, z, line_spec));
            ++it_x;
            ++it_y;
        }
        this->next_plot_replace(previous_status);
        return ls;
    }

    std::vector<line_handle>
    axes_type::plot3(const std::vector<std::vector<double>> &X,
                     const std::vector<std::vector<double>> &Y,
                     const std::vector<std::vector<double>> &Z,
                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        auto it_x = X.begin();
        auto it_y = Y.begin();
        auto it_z = Z.begin();
        bool previous_status = this->next_plot_replace();
        this->next_plot_replace(false);
        std::vector<line_handle> ls;
        while (it_x != X.end() && it_y != Y.end() && it_z != Z.end()) {
            ls.emplace_back(this->plot3(*it_x, *it_y, *it_z, line_spec));
            ++it_x;
            ++it_y;
            ++it_z;
        }
        this->next_plot_replace(previous_status);
        return ls;
    }

    stair_handle axes_type::stairs(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        stair_handle l = std::make_shared<class stair>(this, x, y, line_spec);
        this->emplace_object(l);
        return l;
    }

    stair_handle axes_type::stairs(const std::vector<double> &y,
                                   std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        stair_handle l = std::make_shared<class stair>(this, y, line_spec);
        this->emplace_object(l);
        return l;
    }

    std::vector<stair_handle>
    axes_type::stairs(const std::vector<std::vector<double>> &X,
                      const std::vector<std::vector<double>> &Y,
                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        std::vector<stair_handle> hs;
        bool previous_replace_status = this->next_plot_replace();
        auto X_it = X.begin();
        for (const auto &y : Y) {
            hs.emplace_back(this->stairs(*X_it, y, line_spec));
            ++X_it;
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_replace_status);
        return hs;
    }

    std::vector<stair_handle>
    axes_type::stairs(const std::vector<std::vector<double>> &Y,
                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        std::vector<stair_handle> hs;
        bool previous_replace_status = this->next_plot_replace();
        for (const auto &y : Y) {
            hs.emplace_back(this->stairs(y, line_spec));
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_replace_status);
        return hs;
    }

    std::vector<stair_handle>
    axes_type::stairs(const std::vector<double> &x,
                      const std::vector<std::vector<double>> &Y,
                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        std::vector<stair_handle> hs;
        bool previous_replace_status = this->next_plot_replace();
        for (const auto &y : Y) {
            hs.emplace_back(this->stairs(x, y, line_spec));
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_replace_status);
        return hs;
    }

    error_bar_handle axes_type::errorbar(const std::vector<double> &x,
                                         const std::vector<double> &y,
                                         const std::vector<double> &error,
                                         error_bar::type type,
                                         std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        error_bar_handle l = std::make_shared<class error_bar>(
            this, x, y, error, type, line_spec);
        this->emplace_object(l);
        return l;
    }

    error_bar_handle axes_type::errorbar(const std::vector<double> &x,
                                         const std::vector<double> &y,
                                         const std::vector<double> &y_neg_delta,
                                         const std::vector<double> &y_pos_delta,
                                         const std::vector<double> &x_neg_delta,
                                         const std::vector<double> &x_pos_delta,
                                         std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        error_bar_handle l = std::make_shared<class error_bar>(
            this, x, y, y_neg_delta, y_pos_delta, x_neg_delta, x_pos_delta,
            line_spec);
        this->emplace_object(l);
        return l;
    }

    error_bar_handle axes_type::errorbar(const std::vector<double> &x,
                                         const std::vector<double> &y,
                                         const std::vector<double> &y_error,
                                         std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->errorbar(x, y, y_error, error_bar::type::vertical,
                              line_spec);
    }

    std::vector<filled_area_handle> axes_type::area(
        const std::vector<double> &x, const std::vector<std::vector<double>> &Y,
        double base_value, bool stacked, std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        std::vector<filled_area_handle> hs;
        bool previous_state = this->next_plot_replace();
        // we are going to add y_i in reverse, so we also get the colors
        // in reverse, as if we were getting the colors in order
        std::vector<color_array> colors;
        for (size_t i = 0; i < Y.size(); ++i) {
            colors.emplace_back(this->get_color_and_bump());
            if (!stacked) {
                // reduce opacity if not stacked
                colors.back()[0] = 1.f - (1.f - colors.back()[0]) * 0.7f;
            }
        }
        // use reverse iterator in this case because the first Ys should be
        // the ones in the background. The reverse happens for other object
        // types
        for (auto [Y_it, c_it] = std::make_pair(Y.rbegin(), colors.rbegin());
             Y_it != Y.rend(); ++Y_it, ++c_it) {
            const auto &y = *Y_it;
            auto h = std::make_shared<class filled_area>(
                this, x, y, std::vector({base_value}), stacked, line_spec);
            h->line_width(1.0);
            h->face_color(*c_it);
            this->emplace_object(h);
            hs.emplace_back(h);
            this->next_plot_replace(false);
        }
        this->next_plot_replace(previous_state);
        return hs;
    }

    filled_area_handle axes_type::area(const std::vector<double> &x,
                                       const std::vector<double> &y,
                                       double base_value, bool stacked,
                                       std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        auto as =
            this->area(x, std::vector({y}), base_value, stacked, line_spec);
        return as[0];
    }

    std::vector<filled_area_handle>
    axes_type::area(const std::vector<std::vector<double>> &Y,
                    double base_value, bool stacked,
                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->area(iota(1., static_cast<double>(Y.begin()->size())), Y,
                          base_value, stacked, line_spec);
    }

    std::vector<filled_area_handle>
    axes_type::area(const std::vector<std::vector<double>> &Y, bool stacked,
                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->area(iota(1., static_cast<double>(Y.begin()->size())), Y,
                          0., stacked, line_spec);
    }

    filled_area_handle axes_type::area(const std::vector<double> &y,
                                       double base_value, bool stacked,
                                       std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->area(iota(1., static_cast<double>(y.size())), {y},
                          base_value, stacked, line_spec);
    }

    line_handle axes_type::fimplicit(axes_type::implicit_function_type equation,
                                     const std::array<double, 4> &xy_interval,
                                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        auto [X, Y] = meshgrid(linspace(xy_interval[0], xy_interval[1]),
                               linspace(xy_interval[2], xy_interval[3]));
        auto Z = transform(X, Y, equation);
        std::vector<std::pair<vector_1d, vector_1d>> c =
            contourc(X, Y, Z, vector_1d{0.});
        if (!c.empty()) {
            auto l = this->plot(c[0].first, c[0].second, line_spec);
            return l;
        } else {
            return this->plot(vector_1d{0.}, vector_1d{0.}, line_spec);
        }
    }

    line_handle axes_type::fimplicit(axes_type::implicit_function_type equation,
                                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->fimplicit(equation, {-5, 5, -5, 5}, line_spec);
    }

    string_function_handle axes_type::fplot(std::string_view equation,
                                            std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        string_function_handle obj =
            std::make_shared<class string_function>(this, equation, line_spec);
        this->emplace_object(obj);
        obj->touch();
        return obj;
    }

    std::vector<string_function_handle>
    axes_type::fplot(std::vector<std::string> equations,
                     std::vector<std::string> line_specs) {
        axes_silencer temp_silencer_{this};
        std::vector<string_function_handle> res;
        auto it_line_specs = line_specs.begin();
        for (const auto &equation : equations) {
            if (it_line_specs != line_specs.end()) {
                res.emplace_back(this->fplot(equation, *it_line_specs));
            } else {
                res.emplace_back(this->fplot(equation));
            }
            ++it_line_specs;
        }
        return res;
    }

    function_line_handle axes_type::fplot(function_line::function_type equation,
                                          const std::array<double, 2> &x_range,
                                          std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        function_line_handle obj = std::make_shared<class function_line>(
            this, equation, x_range, line_spec);
        this->emplace_object(obj);
        obj->touch();
        return obj;
    }

    function_line_handle axes_type::fplot(function_line::function_type equation,
                                          std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        function_line_handle obj = std::make_shared<class function_line>(
            this, equation, std::array<double, 2>({-5, 5}), line_spec);
        this->emplace_object(obj);
        obj->touch();
        return obj;
    }

    function_line_handle
    axes_type::fplot(function_line::function_type function_x,
                     function_line::function_type function_y,
                     const std::array<double, 2> &t_range,
                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        function_line_handle obj = std::make_shared<class function_line>(
            this, function_x, function_y, t_range, line_spec);
        this->emplace_object(obj);
        obj->touch();
        return obj;
    }

    std::vector<function_line_handle>
    axes_type::fplot(std::vector<function_line::function_type> equations,
                     std::array<double, 2> x_range,
                     std::vector<std::string> line_specs) {
        axes_silencer temp_silencer_{this};
        std::vector<function_line_handle> res;
        auto it_line_specs = line_specs.begin();
        for (const auto &equation : equations) {
            if (it_line_specs != line_specs.end()) {
                res.emplace_back(
                    this->fplot(equation, x_range, *it_line_specs));
            } else {
                res.emplace_back(this->fplot(equation, x_range));
            }
            ++it_line_specs;
        }
        return res;
    }

    std::vector<function_line_handle>
    axes_type::fplot(std::vector<function_line::function_type> equations,
                     std::vector<double> x_range,
                     std::vector<std::string> line_specs) {
        return this->fplot(equations, to_array<2>(x_range), line_specs);
    }

    function_line_handle
    axes_type::fplot3(function_line::function_type function_x,
                      function_line::function_type function_y,
                      function_line::function_type function_z,
                      const std::array<double, 2> &t_range,
                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        function_line_handle obj = std::make_shared<class function_line>(
            this, function_x, function_y, function_z, t_range, line_spec);
        this->emplace_object(obj);
        obj->touch();
        return obj;
    }

    /// Histogram - Choose binning algorithm and normalization algorithm
    histogram_handle
    axes_type::hist(const std::vector<double> &data,
                    histogram::binning_algorithm algorithm,
                    enum histogram::normalization normalization_alg) {
        axes_silencer temp_silencer_{this};
        histogram_handle l = std::make_shared<class histogram>(
            this, data, algorithm, normalization_alg);
        this->emplace_object(l);
        return l;
    }

    /// Histogram - Fixed number of bins
    /// If n_bins = 0, automatic number of bins
    histogram_handle axes_type::hist(const std::vector<double> &data,
                                     size_t n_bins) {
        axes_silencer temp_silencer_{this};
        histogram_handle l =
            std::make_shared<class histogram>(this, data, n_bins);
        this->emplace_object(l);
        return l;
    }

    /// Histogram - Fixed edges
    histogram_handle axes_type::hist(const std::vector<double> &data,
                                     const std::vector<double> &edges) {
        axes_silencer temp_silencer_{this};
        histogram_handle l =
            std::make_shared<class histogram>(this, data, edges);
        this->emplace_object(l);
        return l;
    }

    /// Histogram - Normalization algorithm
    histogram_handle
    axes_type::hist(const std::vector<double> &data,
                    enum histogram::normalization normalization_alg) {
        axes_silencer temp_silencer_{this};
        return this->hist(data, histogram::binning_algorithm::automatic,
                          normalization_alg);
    }

    /// Histogram - Categorical histogram from strings
    histogram_handle
    axes_type::hist(const std::vector<std::string> &data,
                    enum histogram::normalization normalization_alg) {
        axes_silencer temp_silencer_{this};
        std::map<std::string, double> category_indexes;
        std::vector<double> numeric_data;
        for (const auto &category : data) {
            auto it = category_indexes.find(category);
            if (it != category_indexes.end()) {
                numeric_data.emplace_back(it->second);
            } else {
                double category_num =
                    static_cast<double>(category_indexes.size()) + 1;
                category_indexes[category] = category_num;
                numeric_data.emplace_back(category_num);
            }
        }
        std::vector<double> fixed_edges =
            iota(0.5, 1., 0.5 + category_indexes.size());
        histogram_handle l = std::make_shared<class histogram>(
            this, numeric_data, fixed_edges, normalization_alg);
        this->emplace_object(l);
        this->xticks(iota(1., static_cast<double>(category_indexes.size())));

        std::vector<std::string> unique_categories(category_indexes.size());
        for (const auto &[category, index] : category_indexes) {
            unique_categories[static_cast<size_t>(round(index) - 1)] = category;
        }
        this->x_axis().ticklabels(unique_categories);
        l->bar_width(0.9f);
        this->x_axis().limits(
            {0.5, static_cast<double>(category_indexes.size()) + 0.5});
        return l;
    }

    axes_object_handle axes_type::binscatter(
        const std::vector<double> &x, const std::vector<double> &y,
        const std::vector<double> &edges_x, const std::vector<double> &edges_y,
        enum bin_scatter_style scatter_style,
        enum histogram::normalization normalization_alg) {
        axes_silencer temp_silencer_{this};

        // count data in each bin combination
        std::vector<std::vector<double>> values =
            histcounts2(x, y, edges_x, edges_y, normalization_alg);
        double max_value = values[0][0];
        for (const auto &row : values) {
            for (const auto &value : row) {
                if (value > max_value) {
                    max_value = value;
                }
            }
        }

        // set automatic mode
        if (scatter_style == bin_scatter_style::automatic) {
            scatter_style = bin_scatter_style::point_size;
        }

        // value stats
        double sum_values = normalization_alg == histogram::normalization::count
                                ? x.size()
                                : 0.;
        if (normalization_alg != histogram::normalization::count) {
            for (const auto &row : values) {
                for (const auto &v : row) {
                    sum_values += v;
                }
            }
        }

        // stats for jitter style
        double n_squares =
            static_cast<double>(values.size() * values[0].size());
        double avg_square_value = sum_values / n_squares;
        double n_point_per_plot = 500.;
        double avg_pts_per_square = n_point_per_plot / n_squares;

        // the point sizes must be proportional to the max value and the
        // area available for each point
        double x_bin_width =
            edges_x[edges_x.size() / 2 + 1] - edges_x[edges_x.size() / 2];
        double y_bin_width =
            edges_y[edges_y.size() / 2 + 1] - edges_y[edges_y.size() / 2];
        double reference_width = std::max(x_bin_width, y_bin_width);

        // create bin positions and their sizes
        std::vector<double> bin_x;
        std::vector<double> bin_y;
        std::vector<double> point_sizes;
        std::vector<double> colors;
        for (size_t i = 0; i < edges_x.size() - 1; ++i) {
            for (size_t j = 0; j < edges_y.size() - 1; ++j) {
                if (values[i][j] != 0.0) {
                    if (scatter_style != bin_scatter_style::jitter) {
                        bin_x.emplace_back((edges_x[i] + edges_x[i + 1]) / 2.);
                        bin_y.emplace_back((edges_y[j] + edges_y[j + 1]) / 2.);
                    }
                    switch (scatter_style) {
                    case bin_scatter_style::point_size:
                        point_sizes.emplace_back((values[i][j] / max_value) *
                                                 reference_width * 20);
                        break;
                    case bin_scatter_style::point_alpha:
                    case bin_scatter_style::point_colormap:
                    case bin_scatter_style::heatmap:
                        colors.emplace_back(values[i][j]);
                        break;
                    case bin_scatter_style::jitter: {
                        double n_expected_points_in_bin = values[i][j] /
                                                          avg_square_value *
                                                          avg_pts_per_square;
                        double integer_part = floor(n_expected_points_in_bin);
                        double fractional_part =
                            n_expected_points_in_bin - integer_part;
                        size_t n_points_in_bin = static_cast<size_t>(
                            integer_part + (rand(0, 1) < fractional_part));
                        for (size_t k = 0; k < n_points_in_bin; ++k) {
                            bin_x.emplace_back(
                                rand(edges_x[i], edges_x[i + 1]));
                            bin_y.emplace_back(
                                rand(edges_y[j], edges_y[j + 1]));
                        }
                        break;
                    }
                    case bin_scatter_style::automatic:
                        break;
                    }
                }
            }
        }
        // create scatter plot with this data
        axes_object_handle r;
        line_handle s;
        matrix_handle m;
        switch (scatter_style) {
        case bin_scatter_style::point_size: {
            s = this->scatter(bin_x, bin_y, point_sizes);
            auto c = s->marker_color();
            c[0] = 0.2f;
            s->marker_color(c);
            s->marker_face(true);
            r = s;
            break;
        }
        case bin_scatter_style::point_alpha: {
            s = this->scatter(bin_x, bin_y, std::vector<double>{}, colors);
            auto c = s->marker_color();
            std::vector<double> alpha_100 = {1, 1, 1};
            std::vector<double> alpha_0 = {c[1] - c[1] / 3., c[2] - c[2] / 3.,
                                           c[3] - c[3] / 3.};
            this->colormap({alpha_100, alpha_0});
            s->marker_face(true);
            this->color_box(true);
            this->color_box_log_scale(true);
            r = s;
            break;
        }
        case bin_scatter_style::heatmap:
        case bin_scatter_style::automatic:
            this->colormap(palette::blues());
            m = std::make_shared<class matrix>(this, values);
            m->always_hide_labels(true);
            m->x(edges_x.front());
            m->w(edges_x.back() - edges_x.front());
            m->y(edges_y.front());
            m->h(edges_y.back() - edges_y.front());
            this->emplace_object(m);
            this->color_box(true);
            this->y_axis().reverse(false);
            this->colormap(palette::blues());
            r = m;
            break;
        case bin_scatter_style::point_colormap:
            s = this->scatter(bin_x, bin_y, std::vector<double>{}, colors);
            s->marker_face(true);
            this->color_box(true);
            this->color_box_log_scale(true);
            r = s;
            break;
        case bin_scatter_style::jitter:
            s = this->scatter(bin_x, bin_y);
            r = s;
            break;
        }

        return r;
    }

    axes_object_handle
    axes_type::binscatter(const std::vector<double> &x,
                          const std::vector<double> &y,
                          histogram::binning_algorithm algorithm,
                          enum bin_scatter_style scatter_style,
                          enum histogram::normalization normalization_alg) {
        auto [min_x, max_x] = std::minmax_element(x.begin(), x.end());
        auto x_edges =
            histogram::histogram_edges(x, *min_x, *max_x, algorithm, false);

        auto [min_y, max_y] = std::minmax_element(y.begin(), y.end());
        auto y_edges =
            histogram::histogram_edges(y, *min_y, *max_y, algorithm, false);

        return this->binscatter(x, y, x_edges, y_edges, scatter_style,
                                normalization_alg);
    }

    axes_object_handle
    axes_type::binscatter(const std::vector<double> &x,
                          const std::vector<double> &y,
                          enum bin_scatter_style scatter_style,
                          histogram::binning_algorithm algorithm,
                          enum histogram::normalization normalization_alg) {
        return this->binscatter(x, y, algorithm, scatter_style,
                                normalization_alg);
    }

    axes_object_handle
    axes_type::binscatter(const std::vector<double> &x,
                          const std::vector<double> &y, size_t nbins_x,
                          size_t nbins_y, enum bin_scatter_style scatter_style,
                          enum histogram::normalization normalization_alg) {
        auto [minx, maxx] = std::minmax_element(x.begin(), x.end());
        double xrange = *maxx - *minx;
        auto x_edges =
            histogram::bin_picker(*minx, *maxx, nbins_x, xrange / nbins_x);

        auto [miny, maxy] = std::minmax_element(y.begin(), y.end());
        double yrange = *maxy - *miny;
        auto y_edges =
            histogram::bin_picker(*miny, *maxy, nbins_y, yrange / nbins_y);

        return this->binscatter(x, y, x_edges, y_edges, scatter_style,
                                normalization_alg);
    }

    bars_handle axes_type::bar(const std::vector<double> &x,
                               const std::vector<double> &y) {
        axes_silencer temp_silencer_{this};
        bars_handle l = std::make_shared<class bars>(this, x, y);
        this->emplace_object(l);
        return l;
    }

    bars_handle axes_type::bar(const std::vector<double> &y) {
        axes_silencer temp_silencer_{this};
        bars_handle l = std::make_shared<class bars>(this, y);
        this->emplace_object(l);
        return l;
    }

    bars_handle axes_type::bar(const std::vector<std::vector<double>> &Y) {
        axes_silencer temp_silencer_{this};
        bars_handle l = std::make_shared<class bars>(this, Y);
        this->emplace_object(l);
        return l;
    }

    bars_handle axes_type::bar(const std::vector<double> &x,
                               const std::vector<std::vector<double>> &Y) {
        axes_silencer temp_silencer_{this};
        bars_handle l = std::make_shared<class bars>(this, x, Y);
        this->emplace_object(l);
        return l;
    }

    bars_handle axes_type::bar(const std::vector<double> &y, double width) {
        axes_silencer temp_silencer_{this};
        bars_handle l = std::make_shared<class bars>(this, y);
        l->bar_width(static_cast<float>(width));
        this->emplace_object(l);
        return l;
    }

    /// Bar stacked - Many Y - Automatic x
    std::vector<bars_handle>
    axes_type::barstacked(const std::vector<std::vector<double>> &Y) {
        axes_silencer temp_silencer_{this};
        std::vector<bars_handle> hs;

        auto stacked_positive_values = Y[0];
        for (size_t i = 0; i < Y.size(); ++i) {
            if (stacked_positive_values.size() < Y[i].size()) {
                stacked_positive_values.resize(Y[i].size());
            }
            for (std::size_t j = 0; j < Y[i].size(); ++j) {
                if (Y[i][j] < 0) {
                    stacked_positive_values[j] = 0;
                }
            }
        }

        auto stacked_negative_values = Y[0];
        for (size_t i = 0; i < Y.size(); ++i) {
            if (stacked_negative_values.size() < Y[i].size()) {
                stacked_negative_values.resize(Y[i].size());
            }
            for (std::size_t j = 0; j < Y[i].size(); ++j) {
                if (Y[i][j] > 0) {
                    stacked_negative_values[j] = 0;
                }
            }
        }

        for (size_t i = 0; i < Y.size(); ++i) {
            auto current_stack = stacked_positive_values;
            for (size_t j = 0; j < Y[i].size(); ++j) {
                if (Y[i][j] < 0) {
                    current_stack[j] = stacked_negative_values[j];
                }
            }
            bars_handle h = std::make_shared<class bars>(this, current_stack);
            h->face_color(this->get_color_and_bump());
            hs.emplace_back(h);
            if (i != Y.size() - 1) {
                for (size_t j = 0; j < Y[i + 1].size(); ++j) {
                    if (Y[i + 1][j] > 0) {
                        stacked_positive_values[j] += Y[i + 1][j];
                    } else {
                        stacked_negative_values[j] += Y[i + 1][j];
                    }
                }
            }
        }

        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);
        for (auto it = hs.rbegin(); it != hs.rend(); ++it) {
            this->emplace_object(*it);
        }
        this->next_plot_replace(p2);
        return hs;
    }

    /// Bar stacked - One x - Many Y
    std::vector<bars_handle>
    axes_type::barstacked(const std::vector<double> &x,
                          const std::vector<std::vector<double>> &Y) {
        axes_silencer temp_silencer_{this};
        std::vector<bars_handle> hs;
        auto stacked_positive_values = Y[0];
        for (size_t i = 0; i < Y.size(); ++i) {
            if (Y[0][i] < 0) {
                stacked_positive_values[i] = 0;
            }
        }
        auto stacked_negative_values = Y[0];
        for (size_t i = 0; i < Y.size(); ++i) {
            if (Y[0][i] > 0) {
                stacked_negative_values[i] = 0;
            }
        }
        for (size_t i = 0; i < Y.size(); ++i) {
            auto current_stack = stacked_positive_values;
            for (size_t j = 0; j < Y[i].size(); ++j) {
                if (Y[i][j] < 0) {
                    current_stack[j] = stacked_negative_values[j];
                }
            }
            bars_handle h =
                std::make_shared<class bars>(this, x, current_stack);
            h->face_color(this->get_color_and_bump());
            hs.emplace_back(h);
            if (i != Y.size() - 1) {
                for (size_t j = 0; j < Y[i + 1].size(); ++j) {
                    if (Y[i + 1][j] > 0) {
                        stacked_positive_values[j] += Y[i + 1][j];
                    } else {
                        stacked_negative_values[j] += Y[i + 1][j];
                    }
                }
            }
        }
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);
        for (auto it = hs.rbegin(); it != hs.rend(); ++it) {
            this->emplace_object(*it);
        }
        this->next_plot_replace(p2);
        return hs;
    }

    matrix_handle
    axes_type::heatmap(const std::vector<std::vector<double>> &m) {
        axes_silencer temp_silencer_{this};
        // create matrix in the xlim
        matrix_handle heatmap = std::make_shared<class matrix>(this, m);
        this->emplace_object(heatmap);
        this->y_axis().reverse(true);
        this->color_box(true);
        auto [h_, w_] = size(m);
        this->y_axis().tick_values(iota(1., static_cast<double>(h_)));
        this->y_axis().tick_length(0.);
        this->x_axis().tick_values(iota(1., static_cast<double>(w_)));
        this->x_axis().tick_length(0.);
        this->colormap(palette::blues());
        this->box(true);
        this->grid(false);
        this->minor_grid(true);
        this->grid_front(true);
        this->minor_grid_line_style(line_spec("-k"));
        return heatmap;
    }

    matrix_handle axes_type::pcolor(const std::vector<std::vector<double>> &m) {
        axes_silencer temp_silencer_{this};
        // create matrix in the xlim
        matrix_handle pcolor = std::make_shared<class matrix>(this, m);
        pcolor->x(1.5);
        pcolor->y(1.5);
        pcolor->always_hide_labels(true);
        this->emplace_object(pcolor);

        auto [h, w] = size(m);
        this->ylim({1, static_cast<double>(h + 1)});
        this->y_axis().tick_values(
            iota(static_cast<double>(1), static_cast<double>(h)));
        this->xlim({1, static_cast<double>(w + 1)});
        this->x_axis().tick_values(
            iota(static_cast<double>(1), static_cast<double>(w)));
        this->y_axis().reverse(false);
        this->box(true);
        this->grid(true);
        this->grid_color({0., 0., 0., 0.});
        this->minor_grid(false);
        this->grid_front(true);
        this->minor_grid_line_style(line_spec("-k"));
        this->color_box(false);

        return pcolor;
    }

    /// Core parallel plot function
    parallel_lines_handle
    axes_type::parallelplot(const std::vector<std::vector<double>> &X,
                            const std::vector<double> &colors,
                            std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        parallel_lines_handle l =
            std::make_shared<class parallel_lines>(this, X, line_spec);
        l->line_colors(colors);
        this->colormap(palette::paired());
        this->emplace_object(l);
        if (children_.size() == 1) {
            xlim({0.5, static_cast<double>(X.size()) + 0.5});
            ylim({0.0, 1.});
            this->y_axis_.visible(false);
        }
        return l;
    }

    /// Parallel plot - default line colors
    parallel_lines_handle
    axes_type::parallelplot(const std::vector<std::vector<double>> &X,
                            std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->parallelplot(X, std::vector<double>({}), line_spec);
    }

    circles_handle axes_type::pie(const std::vector<double> &x,
                                  const std::vector<double> &explode,
                                  const std::vector<std::string> &labels) {
        // axes_silencer temp_silencer_{this};

        // pie position
        constexpr double pie_center_x = 0.0;
        constexpr double pie_center_y = 0.0;
        constexpr double pie_radius = 1.0;
        std::vector<double> x_pos(x.size(), pie_center_x);
        std::vector<double> y_pos(x.size(), pie_center_y);
        std::vector<double> radius(x.size(), pie_radius);

        // explode
        double real_total_x = std::accumulate(x.begin(), x.end(), 0.);
        double pie_total_x = std::max(1., real_total_x);
        if (!explode.empty()) {
            double theta_sum = 0.;
            for (size_t i = 0; i < x.size(); ++i) {
                const double theta = theta_sum + (x[i] / pie_total_x) * pi;
                const double explosion_radius = pie_radius * 0.1 * explode[i];
                x_pos[i] = explosion_radius * cos(theta + pi / 2);
                y_pos[i] = explosion_radius * sin(theta + pi / 2);
                theta_sum += (x[i] / pie_total_x) * 2 * pi;
            }
        }

        // set angles
        std::vector<double> start_angle(x.size(), 0);
        std::vector<double> end_angle(x.size(), 360);
        start_angle[0] = 90;
        for (size_t i = 0; i < x.size() - 1; ++i) {
            end_angle[i] = start_angle[i] + (x[i] / pie_total_x) * 360;
            while (end_angle[i] > 360) {
                end_angle[i] -= 360;
            }
            start_angle[i + 1] = end_angle[i];
        }
        end_angle[x.size() - 1] =
            real_total_x >= 1.0 ? 90 : (real_total_x * 360) + 90;
        while (end_angle[x.size() - 1] > 360) {
            end_angle[x.size() - 1] -= 360;
        }

        // pie colors
        std::vector<double> color =
            iota(static_cast<double>(1), static_cast<double>(x.size()));

        // create circles in the xlim
        circles_handle pie = std::make_shared<class circles>(
            this, x_pos, y_pos, radius, start_angle, end_angle, color);
        this->emplace_object(pie);

        // create labels around the circles
        // convert polar coordinate to cartesian coordinates
        // t=0:0.01:2*pi;
        // x = r * cos(t);
        // y = r * sin(t);
        // bool r = this->next_plot_replace();
        this->next_plot_replace(false);
        std::vector<double> label_x(x.size());
        std::vector<double> label_y(x.size());
        std::vector<std::string> pie_labels(x.size());
        double theta_sum = 0.; // [0;2pi]
        std::stringstream stream;
        stream.precision(5);
        stream << std::fixed;
        for (size_t i = 0; i < x.size(); ++i) {
            if (labels.size() > i) {
                stream << labels[i];
            } else {
                stream << std::fixed << std::setprecision(2)
                       << x[i] * 100 / pie_total_x << "%";
            }
            pie_labels[i] = stream.str();
            stream.str("");
            double theta = theta_sum + (x[i] / pie_total_x) * pi;
            const double explosion_radius =
                explode.size() > i ? 0.1 * explode[i] : 0;
            const double label_radius = pie_radius * (1.3 + explosion_radius);
            label_x[i] = label_radius * cos(theta + pi / 2);
            label_y[i] = label_radius * sin(theta + pi / 2);
            theta_sum += (x[i] / pie_total_x) * 2 * pi;
        }
        labels_handle ls =
            std::make_shared<class labels>(this, label_x, label_y, pie_labels);
        pie->labels(ls);

        // make xlim more pleasant for the pie
        if (this->x_axis().limits_mode_auto() ||
            this->y_axis().limits_mode_auto()) {
            this->axis(equal);
            this->x_axis().visible(false);
            this->y_axis().visible(false);
        }
        this->color(this->parent()->color());
        this->box(false);

        return pie;
    }

    circles_handle axes_type::pie(const std::vector<double> &x,
                                  const std::vector<std::string> &labels) {
        return this->pie(x, {}, labels);
    }

    line_handle axes_type::scatter(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   const std::vector<double> &sizes,
                                   const std::vector<double> &colors) {
        axes_silencer temp_silencer_{this};
        line_handle l = this->plot(x, y, "o");
        if (l->line_spec().marker_style() == line_spec::marker_style::none) {
            l->line_spec().marker_style(line_spec::marker_style::circle);
            l->line_spec().marker_face(false);
        }
        l->line_spec().line_style(line_spec::line_style::none);
        if (!sizes.empty()) {
            l->marker_size(sizes);
        }
        if (!colors.empty()) {
            l->marker_colors(colors);
        }
        return l;
    }

    line_handle axes_type::scatter(const std::vector<double> &x,
                                   const std::vector<double> &y, double sizes,
                                   const std::vector<double> &colors) {
        if (sizes == 0) {
            return scatter(x, y, std::vector<double>{}, colors);
        } else {
            return scatter(x, y, std::vector<double>(x.size(), sizes), colors);
        }
    }

    /// Core scatter3 function
    line_handle axes_type::scatter3(const std::vector<double> &x,
                                    const std::vector<double> &y,
                                    const std::vector<double> &z,
                                    const std::vector<double> &sizes,
                                    const std::vector<double> &colors,
                                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        line_handle l = this->plot3(x, y, z, line_spec);
        if (l->line_spec().marker_style() == line_spec::marker_style::none) {
            l->line_spec().marker_style(line_spec::marker_style::circle);
            l->line_spec().marker_face(false);
        }
        l->line_spec().line_style(line_spec::line_style::none);
        if (!sizes.empty()) {
            l->marker_size(sizes);
        }
        if (!colors.empty()) {
            l->marker_colors(colors);
        }
        return l;
    }

    /// Scatter3 - Default sizes and colors
    line_handle axes_type::scatter3(const std::vector<double> &x,
                                    const std::vector<double> &y,
                                    const std::vector<double> &z,
                                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        return this->scatter3(x, y, z, {}, {}, line_spec);
    }

    labels_handle
    axes_type::wordcloud(const std::vector<std::string> &words,
                         const std::vector<double> &sizes,
                         const std::vector<double> &custom_colors) {
        axes_silencer temp_silencer_{this};

        // set labels, their colors, and sizes give words a random position
        std::vector<std::string> labels;
        std::vector<double> colors;
        std::vector<double> float_sizes;
        double max_size = *std::max_element(sizes.begin(), sizes.end());
        constexpr double largest_word_size = 70;
        for (size_t i = 0; i < words.size(); ++i) {
            const std::string &word = words[i];
            // label is the word itself
            labels.emplace_back(word);
            // color is proportional to the frequency
            colors.emplace_back(custom_colors.empty() ? sizes[i]
                                                      : custom_colors[i]);
            // size is proportional to log(word size)
            float_sizes.emplace_back((static_cast<double>(sizes[i])) *
                                     largest_word_size / (max_size));
        }

        // set a position 0,0 for each label and spin until there is no overlap
        constexpr double width_factor = labels::width_factor;
        constexpr double height_factor = labels::height_factor;
        std::vector<double> x;
        std::vector<double> y;
        for (size_t i = 0; i < labels.size(); ++i) {
            x.emplace_back(0.0);
            y.emplace_back(0.0);

            // check if current label overlaps with previous labels
            auto overlap = [&]() {
                const double label_width =
                    round(float_sizes[i]) * labels[i].size() * width_factor;
                const double label_height =
                    round(float_sizes[i]) * height_factor;
                for (size_t j = 0; j < i; ++j) {
                    const double b_label_width =
                        round(float_sizes[j]) * labels[j].size() * width_factor;
                    const double b_label_height =
                        round(float_sizes[j]) * height_factor;
                    const double axmin = x[i] - label_width / 2;
                    const double axmax = x[i] + label_width / 2;
                    const double bxmin = x[j] - b_label_width / 2;
                    const double bxmax = x[j] + b_label_width / 2;
                    const double aymin = y[i] - label_height / 2;
                    const double aymax = y[i] + label_height / 2;
                    const double bymin = y[j] - b_label_height / 2;
                    const double bymax = y[j] + b_label_height / 2;
                    if (axmin > bxmax || bxmin > axmax || aymin > bymax ||
                        bymin > aymax) {
                        // no overlap
                        continue;
                    } else {
                        // label[i] overlaps with label[j]
                        return true;
                    }
                }
                return false;
            };

            // spin the label position
            auto spin = [&]() {
                double label_radius = sqrt(pow(x[i], 2) + pow(y[i], 2));
                double label_theta = atan2(y[i], x[i]);
                label_radius += 0.01;
                label_theta += 0.1;
                x[i] = label_radius * cos(label_theta);
                y[i] = label_radius * sin(label_theta);
            };

            while (overlap()) {
                spin();
            }
        }

        // attribute random size and color to each word
        // in the real word cloud
        // - position in the middle of screen
        // - rotate the word until we find a place for it
        // - create labels in the xlim
        labels_handle wordcloud = std::make_shared<class labels>(
            this, x, y, labels, colors, float_sizes);
        wordcloud->absolute_size(false);
        wordcloud->rectangles(false);
        this->color_box(true);
        this->color_box_log_scale(true);
        this->colormap(palette::default_colors_map());
        this->emplace_object(wordcloud);

        // make xlim more appropriate for the cloud
        this->color(this->parent()->color());
        this->box(false);
        this->x_axis().visible(false);
        this->y_axis().visible(false);

        return wordcloud;
    }

    labels_handle
    axes_type::wordcloud(const std::vector<std::string> &words,
                         const std::vector<size_t> &sizes,
                         const std::vector<double> &custom_colors) {
        std::vector<double> sizes_d(sizes.size());
        std::transform(sizes.begin(), sizes.end(), sizes_d.begin(),
                       [](const size_t &s) { return static_cast<double>(s); });
        return this->wordcloud(words, sizes_d, custom_colors);
    }

    labels_handle
    axes_type::wordcloud(std::string_view text,
                         const std::vector<std::string> &black_list,
                         std::string_view delimiters, size_t max_cloud_size,
                         const std::vector<double> &custom_colors) {
        auto [tokens, count] =
            wordcount(text, black_list, delimiters, max_cloud_size);
        return this->wordcloud(tokens, to_vector_1d(count), custom_colors);
    }

    /// Core pareto function
    std::pair<bars_handle, line_handle>
    axes_type::pareto(const std::vector<double> &y,
                      const std::vector<std::string> &names, double p_threshold,
                      size_t item_threshold) {
        axes_silencer temp_silencer_{this};
        std::vector<size_t> rankings(y.size());
        rank_elements(y.begin(), y.end(), rankings.begin(),
                      std::greater<double>());
        double total_y = std::accumulate(y.begin(), y.end(), 0.);
        std::vector<std::string> x_ticklabels;
        std::vector<double> largest_y;
        std::vector<double> cummulative_y;
        for (size_t i = 0; i < rankings.size(); ++i) {
            if (names.size() > i) {
                x_ticklabels.emplace_back(names[rankings[i]]);
            } else {
                x_ticklabels.emplace_back(num2str(rankings[i] + 1));
            }
            largest_y.emplace_back(y[rankings[i]]);
            if (i == 0) {
                cummulative_y.emplace_back(largest_y[i]);
            } else {
                cummulative_y.emplace_back(cummulative_y[i - 1] + largest_y[i]);
            }
            if (cummulative_y[i] >= p_threshold * total_y ||
                i + 1 >= item_threshold) {
                if (i < rankings.size() - 1) {
                    cummulative_y.emplace_back(cummulative_y[i] +
                                               y[rankings[i + 1]]);
                }
                break;
            }
        }
        cummulative_y = transform(
            cummulative_y, [total_y](double x) { return x * 100. / total_y; });

        // plot line
        line_handle l = plot(cummulative_y);
        l->line_width(1.);
        this->y2_axis_.visible(true);
        l->use_y2(true);

        // plot bars
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);
        bars_handle b = this->bar(largest_y);
        b->face_color(l->color());
        this->next_plot_replace(p2);

        // set range
        this->y_axis().limits({0, total_y});

        // set ticks
        this->x_axis().tick_values(
            iota(1., static_cast<double>(largest_y.size())));
        this->x_axis().ticklabels(x_ticklabels);
        this->x_axis().limits({0.5, double(x_ticklabels.size()) + 0.5});

        // set y2 axis
        this->y2_axis().visible(true);
        this->y2_axis().limits({0, inf});
        this->y2_axis().tick_label_format("%g%%");

        this->box(true);

        return std::make_pair(b, l);
    }

    /// Core plot function
    line_handle axes_type::stem(const std::vector<double> &x,
                                const std::vector<double> &y,
                                std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        line_handle l = this->plot(x, y, line_spec);
        l->impulse(true);
        this->x_axis().zero_axis(true);
        return l;
    }

    /// Basic plot function with automatic x
    line_handle axes_type::stem(const std::vector<double> &y,
                                std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        line_handle l =
            this->stem(iota(1., static_cast<double>(y.size())), y, line_spec);
        l->impulse(true);
        this->x_axis().zero_axis(true);

        return l;
    }

    /// Stem - Plot lists of lists
    std::vector<line_handle>
    axes_type::stem(const std::vector<double> &x,
                    const std::vector<std::vector<double>> &Y,
                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        std::vector<line_handle> hs = this->plot(x, Y, line_spec);
        for (auto &h : hs) {
            h->impulse(true);
        }
        this->x_axis().zero_axis(true);

        return hs;
    }

    /// Plot lists of lists with automatic x
    std::vector<line_handle>
    axes_type::stem(const std::vector<std::vector<double>> &Y,
                    std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        std::vector<line_handle> hs = this->plot(Y, line_spec);
        for (auto &h : hs) {
            h->impulse(true);
        }
        this->x_axis().zero_axis(true);

        return hs;
    }

    /// Stem 3d - Core function
    line_handle axes_type::stem3(const std::vector<double> &x,
                                 const std::vector<double> &y,
                                 const std::vector<double> &z,
                                 std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        line_handle l = this->plot3(x, y, z, line_spec);
        l->impulse(true);
        return l;
    }

    /// 3d-stem lists of Xs and Ys
    std::vector<line_handle>
    axes_type::stem3(const std::vector<std::vector<double>> &X,
                     const std::vector<std::vector<double>> &Y,
                     const std::vector<double> &z, std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        auto hs = this->plot3(X, Y, z, line_spec);
        for (auto &h : hs) {
            h->impulse(true);
        }
        return hs;
    }

    /// Stem 3d - Automatic x and y
    line_handle axes_type::stem3(const std::vector<double> &z,
                                 std::string_view line_spec) {
        return this->stem3(iota(1., static_cast<double>(z.size())),
                           std::vector<double>(z.size(), 1.), z, line_spec);
    }

    /// Stem 3d - Automatic x and y - Many Zs
    line_handle axes_type::stem3(const std::vector<std::vector<double>> &Z,
                                 std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> z;
        for (size_t i = 0; i < Z.size(); ++i) {
            auto tmp_x = iota(1., static_cast<double>(Z[i].size()));
            x.insert(x.end(), tmp_x.begin(), tmp_x.end());
            std::vector<double> tmp_y(Z[i].size(), static_cast<double>(i));
            y.insert(y.end(), tmp_y.begin(), tmp_y.end());
            z.insert(z.end(), Z[i].begin(), Z[i].end());
        }
        return this->stem3(x, y, z, line_spec);
    }

    /// Core geoplot function
    circles_handle axes_type::geobubble(const std::vector<double> &latitude,
                                        const std::vector<double> &longitude,
                                        const std::vector<double> &sizes,
                                        const std::vector<double> &colors) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();

        // create map if we need
        this->geoplot();

        // normalize sizes
        constexpr double max_bubble_size = 10.;
        std::vector<double> normalized_sizes(sizes.begin(), sizes.end());
        auto mit = std::max_element(sizes.begin(), sizes.end());
        if (mit != sizes.end()) {
            double max_size = *mit;
            for (auto &size : normalized_sizes) {
                size /= max_size;
                size *= max_bubble_size;
            }
        }

        this->next_plot_replace(false);
        circles_handle h = std::make_shared<circles>(
            this, longitude, latitude, normalized_sizes, std::vector<double>(),
            std::vector<double>(), colors);
        h->line_width(1.);
        auto fc = this->get_color_and_bump();
        // const double alpha = fc[0];
        const float opacity = (1.f - fc[0]);
        const float new_opacity = opacity * 0.7f;
        const float new_alpha = 1 - new_opacity;
        fc[0] = new_alpha;
        h->face_color(fc);
        h->line_color(to_array("white"));
        this->emplace_object(h);

        this->next_plot_replace(p2);
        return h;
    }

    /// Core geoplot function
    line_handle axes_type::geodensityplot(const std::vector<double> &latitude,
                                          const std::vector<double> &longitude,
                                          const std::vector<double> &weights) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();

        // create map if we need
        this->geoplot();

        // weighted bin count
        auto [min_x, max_x] =
            std::minmax_element(longitude.begin(), longitude.end());
        auto x_edges = histogram::bin_picker(*min_x, *max_x, 200, 0);

        auto [min_y, max_y] =
            std::minmax_element(latitude.begin(), latitude.end());
        auto y_edges = histogram::bin_picker(*min_y, *max_y, 200, 0);

        std::vector<std::vector<double>> bin_counts(
            x_edges.size() - 1, std::vector<double>(y_edges.size() - 1, 0.));
        for (size_t i = 0; i < longitude.size(); ++i) {
            double xi = longitude[i];
            double yi = latitude[i];
            double wi = weights.size() > i ? weights[i] : 1.;
            // find first edge that does not compare less than v
            auto itx = std::lower_bound(x_edges.begin(), x_edges.end(), xi);
            bool xout_of_range = itx == x_edges.begin() || itx == x_edges.end();
            if (!xout_of_range) {
                auto ity = std::lower_bound(y_edges.begin(), y_edges.end(), yi);
                bool yout_of_range =
                    ity == y_edges.begin() || ity == y_edges.end();
                if (!yout_of_range) {
                    bin_counts[itx - x_edges.begin() - 1]
                              [ity - y_edges.begin() - 1] += wi;
                }
            }
        }

        // create bin positions and their sizes
        std::vector<double> bin_x;
        std::vector<double> bin_y;
        std::vector<double> colors;
        for (size_t i = 0; i < x_edges.size() - 1; ++i) {
            for (size_t j = 0; j < y_edges.size() - 1; ++j) {
                if (bin_counts[i][j] != 0.0) {
                    bin_x.emplace_back((x_edges[i] + x_edges[i + 1]) / 2.);
                    bin_y.emplace_back((y_edges[j] + y_edges[j + 1]) / 2.);
                    colors.emplace_back(bin_counts[i][j]);
                }
            }
        }

        this->next_plot_replace(false);
        line_handle s = this->scatter(bin_x, bin_y, {}, colors);
        s->marker_face(true);
        s->marker_size(1.5);
        this->color_box(true);

        this->next_plot_replace(p2);
        return s;
    }

    /// Plot world map on the axes
    line_handle axes_type::geoplot() {
        axes_silencer temp_silencer_{this};

        for (const auto &child : this->children()) {
            if (child->tag() == "map") {
                return std::dynamic_pointer_cast<class line>(child);
            }
        }

        auto [x, y] = world_map_110m();
        line_handle a = this->plot(x, y);
        a->tag("map");
        color_array land_color = {0.f, 0.9294f, 0.9294f, 0.9294f};
        a->color(land_color);
        a->fill(true);
        color_array bg = {0.f, 0.7882f, 0.7882f, 0.7882f};
        this->color(bg);

        this->x_axis().geographic(true);
        this->x_axis().tick_label_format("%Dº%E");
        this->x_axis().label("Longitude");
        this->y_axis().geographic(true);
        this->y_axis().tick_label_format("%Dº%N");
        this->y_axis().label("Latitude");

        return a;
    }

    /// Core geoplot function - Plot lines on world map
    line_handle axes_type::geoplot(const std::vector<double> &latitude,
                                   const std::vector<double> &longitude,
                                   std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->geoplot();
        this->next_plot_replace(false);
        line_handle l = this->plot(longitude, latitude, line_spec);
        l->line_width(1.0);
        this->next_plot_replace(p2);
        return l;
    }

    /// Adjust limits and reload that region of the world map in the proper
    /// resolution
    void axes_type::geolimits(const std::array<double, 2> &latitude,
                              const std::array<double, 2> &longitude) {
        axes_silencer temp_silencer_{this};

        // look for map object in the xlim
        line_handle map = nullptr;
        for (const axes_object_handle &c : this->children()) {
            if (c->tag() == "map") {
                map = std::dynamic_pointer_cast<class line>(c);
            }
        }

        // If there is no map in the axes
        if (!map) {
            map = this->geoplot();
        }

        // if we found the map
        if (map) {
            // calculate the perimeter of the new limits
            double latitude_kms = 111.12 * std::abs(latitude[1] - latitude[0]);
            double longitude_kms =
                111.12 * std::abs(longitude[1] - longitude[0]);
            float w_pixels = this->width() * this->parent()->width();
            float h_pixels = this->height() * this->parent()->height();
            double w_km_per_pixel = longitude_kms / w_pixels;
            double h_km_per_pixel = latitude_kms / h_pixels;
            double min_km_per_pixel = std::min(w_km_per_pixel, h_km_per_pixel);

            auto &[map_x, map_y] = (min_km_per_pixel <= 10) ? world_map_10m()
                                   : (min_km_per_pixel <= 50)
                                       ? world_map_50m()
                                       : world_map_110m();
            std::vector<double> limits_map_x;
            std::vector<double> limits_map_y;

            // for each submap
            for (size_t i = 0; i < map_x.size(); ++i) {
                // copy next submap
                std::vector<double> submap_x;
                std::vector<double> submap_y;
                while (i < map_x.size() && std::isfinite(map_x[i]) &&
                       std::isfinite(map_y[i])) {
                    submap_x.emplace_back(map_x[i]);
                    submap_y.emplace_back(map_y[i]);
                    ++i;
                }

                // fn to check if a point is inside the limits we want
                auto inside_the_map = [&](double x, double y) {
                    return std::isfinite(x) && std::isfinite(y) &&
                           x <= longitude[1] && x >= longitude[0] &&
                           y <= latitude[1] && y >= latitude[0];
                };

                // fn to check in what quadrant a point falls around our limits
                auto quadrant = [&](double x, double y) {
                    if (y > latitude[1]) {
                        if (x < longitude[0]) {
                            return 1;
                        } else if (x > longitude[1]) {
                            return 3;
                        } else {
                            return 2;
                        }
                    } else if (y < latitude[0]) {
                        if (x < longitude[0]) {
                            return 7;
                        } else if (x > longitude[1]) {
                            return 9;
                        } else {
                            return 8;
                        }
                    } else {
                        if (x < longitude[0]) {
                            return 4;
                        } else if (x > longitude[1]) {
                            return 6;
                        } else {
                            return 5;
                        }
                    }
                };

                // check if submap has points inside the limits
                bool add_submap = false;
                for (size_t j = 0; j < submap_x.size(); ++j) {
                    if (inside_the_map(submap_x[j], submap_y[j])) {
                        add_submap = true;
                        break;
                    }
                }

                // copy submap if it has points we need
                if (add_submap) {
                    int last_q = quadrant(submap_x[0], submap_y[0]);
                    int q = last_q;
                    // for each point
                    for (size_t j = 0; j < submap_x.size(); ++j) {
                        // if point is inside the limit
                        if (inside_the_map(submap_x[j], submap_y[j])) {
                            // copy point
                            q = 5;
                            limits_map_x.emplace_back(submap_x[j]);
                            limits_map_y.emplace_back(submap_y[j]);
                        } else {
                            // else, if changed the quadrant around the map
                            q = quadrant(submap_x[j], submap_y[j]);
                            if (q != last_q) {
                                // copy point
                                limits_map_x.emplace_back(submap_x[j]);
                                limits_map_y.emplace_back(submap_y[j]);
                            }
                        }
                        last_q = q;
                    }
                    // new submap divider
                    if (i != map_x.size() - 1) {
                        limits_map_x.emplace_back(NaN);
                        limits_map_y.emplace_back(NaN);
                    }
                }
            }

            map->x_data(limits_map_x);
            map->y_data(limits_map_y);
        }

        this->x_axis().limits(longitude);
        this->y_axis().limits(latitude);
    }

    void axes_type::geolimits(double latitude_x, double latitude_y,
                              double longitude_x, double longitude_y) {
        geolimits(std::array<double, 2>{latitude_x, latitude_y},
                  std::array<double, 2>{longitude_x, longitude_y});
    }

    void axes_type::geolimits(const std::vector<double> &latitude,
                              const std::vector<double> &longitude) {
        this->geolimits(to_array<2>(latitude), to_array<2>(longitude));
    }

    /// Core geoscatter function
    line_handle axes_type::geoscatter(const std::vector<double> &latitude,
                                      const std::vector<double> &longitude,
                                      const std::vector<double> &sizes,
                                      const std::vector<double> &colors) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();

        // create map if we need
        this->geoplot();

        this->next_plot_replace(false);
        line_handle l = this->scatter(longitude, latitude, sizes, colors);

        this->next_plot_replace(p2);
        return l;
    }

    void axes_type::axis(const std::array<double, 4> &limits_x_y) {
        this->x_axis().limits({limits_x_y[0], limits_x_y[1]});
        this->x_axis().limits_mode_auto(false);
        this->y_axis().limits({limits_x_y[2], limits_x_y[3]});
        this->y_axis().limits_mode_auto(false);
        this->touch();
    }

    void axes_type::axis(bool v) {
        this->visible(v);
        if (!v) {
            this->title_visible(true);
        }
    }

    /// \brief Make x/y/z-axis limits automatic
    void axes_type::axis(keyword_automatic_type v) { this->limits_mode(v); }

    /// \brief Make x/y/z-axis limits manual
    void axes_type::axis(keyword_manual_type v) { this->limits_mode(v); }

    /// \brief Reverse y-axis
    void axes_type::axis(keyword_ij_type) { this->y_axis().reverse(true); }

    /// \brief Make x/y-axis ratio = 1 by changing the axis limits
    void axes_type::axis(keyword_equal_type) {
        if (this->children().empty()) {
            return;
        }
        float w = this->width() * this->parent()->width();
        float h = this->height() * this->parent()->height();
        auto [xmin, xmax, ymin, ymax] = this->child_limits();
        double xrange = xmax - xmin;
        double yrange = ymax - ymin;
        // new ranges that could make it proportional
        // new xrange = w / factor that makes current xrange fit in there
        // w / factor == xrange -> factor = w / xrange
        double new_xrange = w * (yrange / h);
        double new_yrange = h * (xrange / w);
        if (new_xrange >= xrange) {
            double size_increase = (new_xrange - xrange) / 2;
            this->x_axis().limits({xmin - size_increase, xmax + size_increase});
            this->y_axis().limits({ymin, ymax});
        } else if (new_yrange >= yrange) {
            double size_increase = (new_yrange - yrange) / 2;
            this->x_axis().limits({xmin, xmax});
            this->y_axis().limits({ymin - size_increase, ymax + size_increase});
        }
    }

    /// \brief Make x/y/z-axis limits tight from x/y/z-min to x/y/z-max
    void axes_type::axis(keyword_tight_type) {
        if (this->children().empty()) {
            return;
        }
        auto [xmin, xmax, ymin, ymax] = this->child_limits();
        this->x_axis().limits({xmin, xmax});
        this->y_axis().limits({ymin, ymax});
        if (this->is_3d() && !this->is_3d_map()) {
            auto zminit = std::min_element(
                this->children().begin(), this->children().end(),
                [](axes_object_handle a, axes_object_handle b) {
                    return a->zmin() < b->zmin();
                });
            auto zmaxit = std::max_element(
                this->children().begin(), this->children().end(),
                [](axes_object_handle a, axes_object_handle b) {
                    return a->zmax() < b->zmax();
                });
            this->z_axis().limits({(*zminit)->zmin(), (*zmaxit)->zmax()});
        }
    }

    /// \brief Make x/y-axis square with ratio 1 by changing the axes size
    void axes_type::axis(keyword_square_type) {
        axis(equal);
        float w = this->width() * this->parent()->width();
        float h = this->height() * this->parent()->height();
        // make w = h
        if (w > h) {
            double old_width = this->width();
            double new_width = h / this->parent()->width();
            this->width(static_cast<float>(new_width));
            this->x_origin(this->x_origin() +
                           static_cast<float>(old_width - new_width) / 2.f);
        } else if (h > w) {
            double old_height = this->height();
            double new_height = h / this->parent()->height();
            this->height(static_cast<float>(new_height));
            this->y_origin(this->y_origin() +
                           static_cast<float>(old_height - new_height) / 2.f);
        }
    }

    /// Compass plot function
    vectors_handle axes_type::compass(const std::vector<double> &x,
                                      const std::vector<double> &y,
                                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        auto theta = transform(
            x, y, [](double x, double y) { return vector_radians(x, y); });
        auto rho = transform(
            x, y, [](double x, double y) { return vector_magnitude(x, y); });

        vectors_handle l =
            std::make_shared<class vectors>(this, theta, rho, line_spec);
        this->emplace_object(l);

        l->polar(true);

        this->axis(equal);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return l;
    }

    /// Ezpolar - Plot function on polar plot
    string_function_handle axes_type::ezpolar(std::string_view equation,
                                              std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        string_function_handle obj =
            std::make_shared<class string_function>(this, equation, line_spec);

        this->emplace_object(obj);
        obj->polar(true);

        this->axis(equal);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return obj;
    }

    std::vector<string_function_handle>
    axes_type::ezpolar(std::vector<std::string> equations,
                       std::vector<std::string> line_specs) {
        std::vector<string_function_handle> res;
        auto it_line_specs = line_specs.begin();
        for (const auto &equation : equations) {
            if (it_line_specs != line_specs.end()) {
                res.emplace_back(this->ezpolar(equation, *it_line_specs));
            } else {
                res.emplace_back(this->ezpolar(equation));
            }
            ++it_line_specs;
        }
        return res;
    }

    function_line_handle
    axes_type::ezpolar(function_line::function_type equation,
                       const std::array<double, 2> &t_range,
                       std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        function_line_handle obj = std::make_shared<class function_line>(
            this, equation, t_range, line_spec);
        this->emplace_object(obj);
        obj->polar(true);

        this->axis(equal);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return obj;
    }

    function_line_handle
    axes_type::ezpolar(function_line::function_type equation,
                       std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        function_line_handle obj = std::make_shared<class function_line>(
            this, equation, std::array<double, 2>({0, 2 * pi}), line_spec);
        this->emplace_object(obj);
        obj->polar(true);
        this->axis(equal);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return obj;
    }

    function_line_handle
    axes_type::ezpolar(function_line::function_type equation,
                       std::vector<double> x_range,
                       std::string_view line_spec) {
        return this->ezpolar(equation, to_array<2>(x_range), line_spec);
    }

    function_line_handle
    axes_type::ezpolar(function_line::function_type function_x,
                       function_line::function_type function_y,
                       const std::array<double, 2> &t_range,
                       std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        function_line_handle obj = std::make_shared<class function_line>(
            this, function_x, function_y, t_range, line_spec);
        this->emplace_object(obj);
        obj->polar(true);
        this->axis(equal);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return obj;
    }

    function_line_handle
    axes_type::ezpolar(function_line::function_type function_x,
                       function_line::function_type function_y,
                       std::vector<double> t_range,
                       std::string_view line_spec) {
        return this->ezpolar(function_x, function_y, to_array<2>(t_range),
                             line_spec);
    }

    std::vector<function_line_handle>
    axes_type::ezpolar(std::vector<function_line::function_type> equations,
                       std::array<double, 2> x_range,
                       std::vector<std::string> line_specs) {
        axes_silencer temp_silencer_{this};
        std::vector<function_line_handle> res;
        auto it_line_specs = line_specs.begin();
        for (const auto &equation : equations) {
            if (it_line_specs != line_specs.end()) {
                res.emplace_back(
                    this->ezpolar(equation, x_range, *it_line_specs));
            } else {
                res.emplace_back(this->ezpolar(equation, x_range));
            }
            ++it_line_specs;
        }
        return res;
    }

    std::vector<function_line_handle>
    axes_type::ezpolar(std::vector<function_line::function_type> equations,
                       std::vector<double> x_range,
                       std::vector<std::string> line_specs) {
        return this->ezpolar(equations, to_array<2>(x_range), line_specs);
    }

    /// Polar histogram - Core function
    histogram_handle axes_type::polarhistogram(const std::vector<double> &theta,
                                               size_t nbins) {
        axes_silencer temp_silencer_{this};

        auto reflect_theta =
            transform(theta, [](double t) { return to_positive_radian(t); });
        auto edges = linspace(0., 2 * pi, nbins + 1);
        histogram_handle h = this->hist(reflect_theta, edges);
        h->polar(true);
        this->axis(equal);

        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return h;
    }

    /// Polar plot - Core function
    line_handle axes_type::polarplot(const std::vector<double> &theta,
                                     const std::vector<double> &rho,
                                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        line_handle l = this->plot(theta, rho, line_spec);
        l->polar(true);
        this->axis(equal);

        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return l;
    }

    /// Polar plot - Automatic theta
    line_handle axes_type::polarplot(const std::vector<double> &rho,
                                     std::string_view line_spec) {
        return this->polarplot(linspace(0, 2 * pi, rho.size()), rho, line_spec);
    }

    /// Polar plot - Complex numbers
    line_handle axes_type::polarplot(const std::vector<std::complex<double>> &z,
                                     std::string_view line_spec) {
        std::vector<double> theta;
        std::vector<double> rho;
        for (size_t i = 0; i < z.size(); ++i) {
            rho.emplace_back(sqrt(pow(z[i].real(), 2) + pow(z[i].imag(), 2)));
            theta.emplace_back(atan2(z[i].imag(), z[i].real()));
        }
        return this->polarplot(theta, rho, line_spec);
    }

    /// Polar scatter - Core function
    line_handle axes_type::polarscatter(const std::vector<double> &theta,
                                        const std::vector<double> &rho,
                                        const std::vector<double> &sizes,
                                        const std::vector<double> &colors,
                                        std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        line_handle l = this->scatter(theta, rho, sizes, colors);
        l->line_style(line_spec);
        l->polar(true);
        this->axis(equal);

        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->r_axis().visible(false);
        this->r_axis().tick_length(0.);
        this->t_axis().visible(true);
        this->t_axis().tick_length(0);

        return l;
    }

    /// Polar scatter - Single size - Default colors
    line_handle axes_type::polarscatter(const std::vector<double> &theta,
                                        const std::vector<double> &rho,
                                        double size,
                                        std::string_view line_spec) {
        return this->polarscatter(theta, rho, std::vector(theta.size(), size),
                                  {}, line_spec);
    }

    /// Polar scatter - Default size and colors
    line_handle axes_type::polarscatter(const std::vector<double> &theta,
                                        const std::vector<double> &rho,
                                        std::string_view line_spec) {
        return this->polarscatter(theta, rho, {}, {}, line_spec);
    }

    /// Contour - Core function - Manual levels
    contours_handle
    axes_type::contour(const std::vector<std::vector<double>> &X,
                       const std::vector<std::vector<double>> &Y,
                       const std::vector<std::vector<double>> &Z,
                       std::vector<double> levels, std::string_view line_spec,
                       size_t n_levels) {
        axes_silencer temp_silencer_{this};

        contours_handle l =
            std::make_shared<class contours>(this, X, Y, Z, line_spec);
        if (n_levels) {
            l->n_levels(n_levels);
        }
        if (!levels.empty()) {
            l->levels(levels);
        }
        this->x_axis().limits({l->xmin(), l->xmax()});
        this->y_axis().limits({l->ymin(), l->ymax()});
        this->emplace_object(l);
        this->color_box(true);

        return l;
    }

    /// Contour - Manual number of levels (or 0 for automatic number of levels)
    contours_handle
    axes_type::contour(const std::vector<std::vector<double>> &X,
                       const std::vector<std::vector<double>> &Y,
                       const std::vector<std::vector<double>> &Z,
                       size_t n_levels, std::string_view line_spec) {
        return this->contour(X, Y, Z, {}, line_spec, n_levels);
    }

    /// Contour - Automatic levels and number of levels
    contours_handle
    axes_type::contour(const std::vector<std::vector<double>> &X,
                       const std::vector<std::vector<double>> &Y,
                       const std::vector<std::vector<double>> &Z,
                       std::string_view line_spec) {
        return this->contour(X, Y, Z, 0, line_spec);
    }

    /// Contour filled - Manual levels
    contours_handle
    axes_type::contourf(const std::vector<std::vector<double>> &X,
                        const std::vector<std::vector<double>> &Y,
                        const std::vector<std::vector<double>> &Z,
                        std::vector<double> levels, std::string_view line_spec,
                        size_t n_levels) {
        axes_silencer temp_silencer_{this};

        contours_handle l = this->contour(X, Y, Z, levels, line_spec, n_levels);
        l->filled(true);
        l->line_style("k");
        this->emplace_object(l);

        return l;
    }

    /// Contour filled - Manual number of levels
    contours_handle
    axes_type::contourf(const std::vector<std::vector<double>> &X,
                        const std::vector<std::vector<double>> &Y,
                        const std::vector<std::vector<double>> &Z,
                        size_t n_levels, std::string_view line_spec) {
        return this->contourf(X, Y, Z, {}, line_spec, n_levels);
    }

    /// Contour filled - Automatic number of levels
    contours_handle
    axes_type::contourf(const std::vector<std::vector<double>> &X,
                        const std::vector<std::vector<double>> &Y,
                        const std::vector<std::vector<double>> &Z,
                        std::string_view line_spec) {
        return this->contourf(X, Y, Z, 0, line_spec);
    }

    using fcontour_function_type = std::function<double(double, double)>;

    /// Lambda function contour - Manual levels (or empty list for automatic) /
    /// Manual number of levels (or 0 for automatic)
    contours_handle axes_type::fcontour(fcontour_function_type fn,
                                        const std::array<double, 4> &xy_range,
                                        std::vector<double> levels,
                                        std::string_view line_spec,
                                        size_t n_levels) {
        axes_silencer temp_silencer_{this};

        std::vector<double> x = linspace(xy_range[0], xy_range[1]);
        std::vector<double> y = linspace(xy_range[2], xy_range[3]);
        auto [X, Y] = meshgrid(x, y);
        vector_2d Z(y.size(), vector_1d(x.size(), 0.));
        for (size_t i = 0; i < y.size(); ++i) {
            for (size_t j = 0; j < x.size(); ++j) {
                Z[i][j] = fn(X[i][j], Y[i][j]);
            }
        }

        contours_handle l = this->contour(X, Y, Z, levels, line_spec, n_levels);

        return l;
    }

    /// Lambda function contour - Manual number of levels (default = 9)
    contours_handle axes_type::fcontour(fcontour_function_type fn,
                                        size_t n_levels,
                                        std::string_view line_spec) {
        return this->fcontour(fn, std::array<double, 4>{-5, +5, -5, +5},
                              std::vector<double>{}, line_spec, n_levels);
    }

    /// Lambda function contour - Automatic number of levels and levels
    contours_handle axes_type::fcontour(fcontour_function_type fn,
                                        std::string_view line_spec) {
        return this->fcontour(fn, 9, line_spec);
    }

    /// Feather - Core function
    vectors_handle axes_type::feather(const std::vector<double> &u,
                                      const std::vector<double> &v,
                                      std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        vectors_handle l =
            std::make_shared<class vectors>(this, u, v, line_spec);
        this->emplace_object(l);

        if (!l->line_spec().user_color()) {
            l->line_spec().color(this->get_color_and_bump());
        }

        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);
        std::vector<double> base_line(u.size(), 0);
        auto b = this->plot(base_line, line_spec);
        b->line_spec().color(l->line_spec().color());
        this->next_plot_replace(p2);

        return l;
    }

    /// Quiver - Core function
    vectors_handle axes_type::quiver(const std::vector<double> &x,
                                     const std::vector<double> &y,
                                     const std::vector<double> &u,
                                     const std::vector<double> &v,
                                     const std::vector<double> &c, double scale,
                                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        auto x_copy = x;
        std::sort(x_copy.begin(), x_copy.end());
        x_copy.resize(std::distance(x_copy.begin(),
                                    std::unique(x_copy.begin(), x_copy.end())));
        std::vector<double> x_diff(x_copy.size());
        std::adjacent_difference(x_copy.begin(), x_copy.end(), x_diff.begin());
        auto xdiffmin = std::min_element(x_diff.begin() + 1, x_diff.end());
        double u_max = *std::max_element(u.begin(), u.end());
        double x_max = xdiffmin != x_diff.end() ? *xdiffmin : u_max;

        auto y_copy = y;
        std::sort(y_copy.begin(), y_copy.end());
        y_copy.resize(std::distance(y_copy.begin(),
                                    std::unique(y_copy.begin(), y_copy.end())));
        std::vector<double> y_diff(y_copy.size());
        std::adjacent_difference(y_copy.begin(), y_copy.end(), y_diff.begin());
        auto ydiffmin = std::min_element(y_diff.begin() + 1, y_diff.end());
        double v_max = *std::max_element(v.begin(), v.end());
        double y_max = ydiffmin != y_diff.end() ? *ydiffmin : v_max;

        // scale u, v with same ratio to retain direction
        double mag_max = 1;
        for (size_t i = 0; i < u.size(); ++i) {
            double mag = sqrt(u[i] * u[i] + v[i] * v[i]);
            mag_max = mag > mag_max ? mag : mag_max;
        }
        double val_max = std::min(x_max, y_max);

        auto u_scaled =
            (scale != 0.)
                ? transform(
                      u, [&](double u) { return (u / mag_max) * scale * val_max; })
                : u;
        auto v_scaled =
            (scale != 0.)
                ? transform(
                      v, [&](double v) { return (v / mag_max) * scale * val_max; })
                : v;

        vectors_handle l = std::make_shared<class vectors>(
            this, x, y, u_scaled, v_scaled, c, line_spec);
        l->scale(scale);
        this->emplace_object(l);
        return l;
    }

    /// Quiver - 2d x,y,u,v
    vectors_handle axes_type::quiver(const std::vector<std::vector<double>> &x,
                                     const std::vector<std::vector<double>> &y,
                                     const std::vector<std::vector<double>> &u,
                                     const std::vector<std::vector<double>> &v,
                                     const std::vector<std::vector<double>> &c,
                                     double scale, std::string_view line_spec) {
        return this->quiver(flatten(x), flatten(y), flatten(u), flatten(v),
                            (c.empty()) ? std::vector<double>{} : flatten(c),
                            scale, line_spec);
    }

    /// Quiver - 2d x,y,u,v with no color mapping
    vectors_handle axes_type::quiver(const std::vector<std::vector<double>> &x,
                                     const std::vector<std::vector<double>> &y,
                                     const std::vector<std::vector<double>> &u,
                                     const std::vector<std::vector<double>> &v,
                                     double scale, std::string_view line_spec) {
        return this->quiver(x, y, u, v, std::vector<std::vector<double>>{},
                            scale, line_spec);
    }

    /// Quiver - x,y,u,v
    vectors_handle axes_type::quiver(const std::vector<double> &x,
                                     const std::vector<double> &y,
                                     const std::vector<double> &u,
                                     const std::vector<double> &v, double scale,
                                     std::string_view line_spec) {
        return this->quiver(x, y, u, v, std::vector<double>{}, scale,
                            line_spec);
    }

    /// Quiver 3d - Core function
    vectors_handle axes_type::quiver3(
        const std::vector<double> &x, const std::vector<double> &y,
        const std::vector<double> &z, const std::vector<double> &u,
        const std::vector<double> &v, const std::vector<double> &w,
        const std::vector<double> &c, double scale,
        std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        auto x_copy = x;
        std::sort(x_copy.begin(), x_copy.end());
        x_copy.resize(std::distance(x_copy.begin(),
                                    std::unique(x_copy.begin(), x_copy.end())));
        std::vector<double> x_diff(x_copy.size());
        std::adjacent_difference(x_copy.begin(), x_copy.end(), x_diff.begin());
        auto xdiffmin = std::min_element(x_diff.begin() + 1, x_diff.end());
        double u_max = *std::max_element(u.begin(), u.end());
        double x_max = xdiffmin != x_diff.end() ? *xdiffmin : u_max;

        auto y_copy = y;
        std::sort(y_copy.begin(), y_copy.end());
        y_copy.resize(std::distance(y_copy.begin(),
                                    std::unique(y_copy.begin(), y_copy.end())));
        std::vector<double> y_diff(y_copy.size());
        std::adjacent_difference(y_copy.begin(), y_copy.end(), y_diff.begin());
        auto ydiffmin = std::min_element(y_diff.begin() + 1, y_diff.end());
        double v_max = *std::max_element(v.begin(), v.end());
        double y_max = ydiffmin != y_diff.end() ? *ydiffmin : v_max;

        auto z_copy = z;
        std::sort(z_copy.begin(), z_copy.end());
        z_copy.resize(std::distance(z_copy.begin(),
                                    std::unique(z_copy.begin(), z_copy.end())));
        std::vector<double> z_diff(z_copy.size());
        std::adjacent_difference(z_copy.begin(), z_copy.end(), z_diff.begin());
        auto zdiffmin = std::min_element(z_diff.begin() + 1, z_diff.end());
        double w_max = *std::max_element(w.begin(), w.end());
        double z_max = zdiffmin != z_diff.end() ? *zdiffmin : w_max;

        // scale u, v, w with same ratio to retain direction
        double mag_max = 1;
        for (size_t i = 0; i < u.size(); ++i) {
            double mag = sqrt(u[i] * u[i] + v[i] * v[i] + w[i] * w[i]);
            mag_max = mag > mag_max ? mag : mag_max;
        }
        double val_max = std::min(std::min(x_max, y_max), z_max);

        auto u_scaled =
            (scale != 0.)
                ? transform(
                      u, [&](double u) { return (u / mag_max) * scale * val_max; })
                : u;
        auto v_scaled =
            (scale != 0.)
                ? transform(
                      v, [&](double v) { return (v / mag_max) * scale * val_max; })
                : v;
        auto w_scaled =
            (scale != 0.)
                ? transform(
                      w, [&](double w) { return (w / mag_max) * scale * val_max; })
                : w;

        vectors_handle l = std::make_shared<class vectors>(
            this, x, y, z, u_scaled, v_scaled, w_scaled, c, line_spec);
        l->scale(scale);
        this->emplace_object(l);

        return l;
    }

    /// Quiver 3d - 2d vectors
    vectors_handle axes_type::quiver3(const std::vector<std::vector<double>> &x,
                                      const std::vector<std::vector<double>> &y,
                                      const std::vector<std::vector<double>> &z,
                                      const std::vector<std::vector<double>> &u,
                                      const std::vector<std::vector<double>> &v,
                                      const std::vector<std::vector<double>> &w,
                                      const std::vector<std::vector<double>> &c,
                                      double scale,
                                      std::string_view line_spec) {
        return this->quiver3(flatten(x), flatten(y), flatten(z), flatten(u),
                             flatten(v), flatten(w),
                             (c.empty()) ? std::vector<double>{} : flatten(c),
                             scale, line_spec);
    }

    /// Quiver 3d - 2d vectors with no color mapping
    vectors_handle axes_type::quiver3(const std::vector<std::vector<double>> &x,
                                      const std::vector<std::vector<double>> &y,
                                      const std::vector<std::vector<double>> &z,
                                      const std::vector<std::vector<double>> &u,
                                      const std::vector<std::vector<double>> &v,
                                      const std::vector<std::vector<double>> &w,
                                      double scale,
                                      std::string_view line_spec) {
        return this->quiver3(flatten(x), flatten(y), flatten(z), flatten(u),
                             flatten(v), flatten(w), scale, line_spec);
    }

    /// Quiver 3d - Automatic x and y - 2d vectors
    vectors_handle axes_type::quiver3(const std::vector<std::vector<double>> &z,
                                      const std::vector<std::vector<double>> &u,
                                      const std::vector<std::vector<double>> &v,
                                      const std::vector<std::vector<double>> &w,
                                      const std::vector<std::vector<double>> &c,
                                      double scale,
                                      std::string_view line_spec) {
        auto [n, p] = size(z);
        vector_1d x = iota(1., static_cast<double>(p));
        vector_1d y = iota(1., static_cast<double>(n));
        auto [xx, yy] = meshgrid(x, y);
        return this->quiver3(xx, yy, z, u, v, w, c, scale, line_spec);
    }

    /// Quiver 3d - Automatic x and y - 2d vectors no color mapping
    vectors_handle axes_type::quiver3(const std::vector<std::vector<double>> &z,
                                      const std::vector<std::vector<double>> &u,
                                      const std::vector<std::vector<double>> &v,
                                      const std::vector<std::vector<double>> &w,
                                      double scale,
                                      std::string_view line_spec) {
        return this->quiver3(z, u, v, w, std::vector<std::vector<double>>{},
                             scale, line_spec);
    }

    /// Quiver 3d - no color mapping
    vectors_handle axes_type::quiver3(
        const std::vector<double> &x, const std::vector<double> &y,
        const std::vector<double> &z, const std::vector<double> &u,
        const std::vector<double> &v, const std::vector<double> &w,
        double scale, std::string_view line_spec) {
        auto l = this->quiver3(x, y, z, u, v, w, std::vector<double>{}, scale,
                               line_spec);
        return l;
    }

    /// Fence - Core function
    surface_handle axes_type::fence(const std::vector<std::vector<double>> &X,
                                    const std::vector<std::vector<double>> &Y,
                                    const std::vector<std::vector<double>> &Z,
                                    const std::vector<double> &c) {
        axes_silencer temp_silencer_{this};

        surface_handle l =
            this->mesh(X, Y, Z, !c.empty() ? vector_2d{c} : vector_2d{});
        l->fences(true);

        return l;
    }

    // z = f(x,y)
    using fmesh_function_type = std::function<double(double, double)>;

    /// Lambda function mesh - Core function
    surface_handle axes_type::fmesh(fcontour_function_type fn,
                                    const std::array<double, 2> &x_range,
                                    const std::array<double, 2> &y_range,
                                    double mesh_density) {
        axes_silencer temp_silencer_{this};

        std::vector<double> x =
            linspace(x_range[0], x_range[1], static_cast<size_t>(mesh_density));
        std::vector<double> y =
            linspace(y_range[0], y_range[1], static_cast<size_t>(mesh_density));
        auto [X, Y] = meshgrid(x, y);
        auto Z = transform(X, Y, fn);
        surface_handle l = this->mesh(X, Y, Z, {});

        return l;
    }

    /// Lambda function mesh - parametric function
    surface_handle axes_type::fmesh(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 2> &u_range,
                                    const std::array<double, 2> &v_range,
                                    double mesh_density) {
        axes_silencer temp_silencer_{this};

        std::vector<double> u =
            linspace(u_range[0], u_range[1], static_cast<size_t>(mesh_density));
        std::vector<double> v =
            linspace(v_range[0], v_range[1], static_cast<size_t>(mesh_density));
        auto [U, V] = meshgrid(u, v);
        auto X = transform(U, V, funx);
        auto Y = transform(U, V, funy);
        auto Z = transform(U, V, funz);
        surface_handle l = this->mesh(X, Y, Z, {});

        return l;
    }

    /// Lambda function mesh
    /// Grid / Both ranges in the same array size 4
    surface_handle axes_type::fmesh(fcontour_function_type fn,
                                    const std::array<double, 4> &xy_range,
                                    double mesh_density) {
        return this->fmesh(fn, {xy_range[0], xy_range[1]},
                           {xy_range[2], xy_range[3]}, mesh_density);
    }

    /// Lambda function mesh - Parametric
    /// Parametric / Both ranges in the same array size 4
    surface_handle axes_type::fmesh(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 4> &uv_range,
                                    double mesh_density) {
        return this->fmesh(funx, funy, funz, {uv_range[0], uv_range[1]},
                           {uv_range[2], uv_range[3]}, mesh_density);
    }

    /// Function mesh
    /// Grid / Both ranges in the same array size 2
    surface_handle axes_type::fmesh(fcontour_function_type fn,
                                    const std::array<double, 2> &xy_range,
                                    double mesh_density) {
        return this->fmesh(fn, xy_range, xy_range, mesh_density);
    }

    /// Function mesh
    /// Parametric / Both ranges in the same array size 2
    surface_handle axes_type::fmesh(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 2> &uv_range,
                                    double mesh_density) {
        return this->fmesh(funx, funy, funz, uv_range, uv_range, mesh_density);
    }

    // z = f(x,y)
    using fsurf_function_type = std::function<double(double, double)>;

    /// Function surf - Core function
    surface_handle axes_type::fsurf(fcontour_function_type fn,
                                    const std::array<double, 2> &x_range,
                                    const std::array<double, 2> &y_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        axes_silencer temp_silencer_{this};

        std::vector<double> x =
            linspace(x_range[0], x_range[1], static_cast<size_t>(mesh_density));
        std::vector<double> y =
            linspace(y_range[0], y_range[1], static_cast<size_t>(mesh_density));
        auto [X, Y] = meshgrid(x, y);
        auto Z = transform(X, Y, fn);
        surface_handle l = this->surf(X, Y, Z, {}, line_spec);

        return l;
    }

    /// Function surf - Parametric
    /// Core parametric function
    surface_handle axes_type::fsurf(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 2> &u_range,
                                    const std::array<double, 2> &v_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        axes_silencer temp_silencer_{this};

        std::vector<double> u =
            linspace(u_range[0], u_range[1], static_cast<size_t>(mesh_density));
        std::vector<double> v =
            linspace(v_range[0], v_range[1], static_cast<size_t>(mesh_density));
        auto [U, V] = meshgrid(u, v);
        auto X = transform(U, V, funx);
        auto Y = transform(U, V, funy);
        auto Z = transform(U, V, funz);
        surface_handle l = this->surf(X, Y, Z, {}, line_spec);

        return l;
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 4
    surface_handle axes_type::fsurf(fcontour_function_type fn,
                                    const std::array<double, 4> &xy_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        return this->fsurf(fn, {xy_range[0], xy_range[1]},
                           {xy_range[2], xy_range[3]}, line_spec, mesh_density);
    }

    /// Parametric / Both ranges in the same array size 4
    surface_handle axes_type::fsurf(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 4> &uv_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        return this->fsurf(funx, funy, funz, {uv_range[0], uv_range[1]},
                           {uv_range[2], uv_range[3]}, line_spec, mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 2
    surface_handle axes_type::fsurf(fcontour_function_type fn,
                                    const std::array<double, 2> &xy_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        return this->fsurf(fn, xy_range, xy_range, line_spec, mesh_density);
    }

    /// Function surf
    /// Parametric / Both ranges in the same array size 2
    surface_handle axes_type::fsurf(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    const std::array<double, 2> &uv_range,
                                    std::string_view line_spec,
                                    double mesh_density) {
        return this->fsurf(funx, funy, funz, uv_range, uv_range, line_spec,
                           mesh_density);
    }

    /// Mesh - Core function
    surface_handle axes_type::mesh(const std::vector<std::vector<double>> &X,
                                   const std::vector<std::vector<double>> &Y,
                                   const std::vector<std::vector<double>> &Z,
                                   const std::vector<std::vector<double>> &C) {
        axes_silencer temp_silencer_{this};

        surface_handle l = std::make_shared<class surface>(this, X, Y, Z, C);
        l->palette_map_at_surface(false);
        l->hidden_3d(true);
        this->emplace_object(l);

        return l;
    }

    /// Mesh with contour
    surface_handle axes_type::meshc(const std::vector<std::vector<double>> &X,
                                    const std::vector<std::vector<double>> &Y,
                                    const std::vector<std::vector<double>> &Z,
                                    const std::vector<std::vector<double>> &C) {
        axes_silencer temp_silencer_{this};

        surface_handle l = std::make_shared<class surface>(this, X, Y, Z, C);
        l->palette_map_at_surface(false);
        l->contour_base(true);
        l->hidden_3d(true);
        this->emplace_object(l);

        return l;
    }

    /// Mesh with curtain
    /// Core function
    surface_handle axes_type::meshz(const std::vector<std::vector<double>> &X,
                                    const std::vector<std::vector<double>> &Y,
                                    const std::vector<std::vector<double>> &Z,
                                    const std::vector<std::vector<double>> &C) {
        axes_silencer temp_silencer_{this};

        surface_handle l = this->mesh(X, Y, Z, C);
        l->curtain(true);

        return l;
    }

    /// Ribbon
    surface_handle axes_type::ribbon(const std::vector<std::vector<double>> &X,
                                     const std::vector<std::vector<double>> &Y,
                                     const std::vector<std::vector<double>> &Z,
                                     const std::vector<std::vector<double>> &C,
                                     double width) {
        axes_silencer temp_silencer_{this};

        surface_handle l = this->surf(X, Y, Z, C);
        l->ribbons(true);
        l->ribbon_width(width);
        l->depthorder(true);

        return l;
    }

    /// Surf - Core function
    surface_handle axes_type::surf(const std::vector<std::vector<double>> &X,
                                   const std::vector<std::vector<double>> &Y,
                                   const std::vector<std::vector<double>> &Z,
                                   const std::vector<std::vector<double>> &C,
                                   std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        surface_handle l =
            std::make_shared<class surface>(this, X, Y, Z, C, line_spec);
        this->emplace_object(l);

        return l;
    }

    /// Surf with contour - Core function
    surface_handle axes_type::surfc(const std::vector<std::vector<double>> &X,
                                    const std::vector<std::vector<double>> &Y,
                                    const std::vector<std::vector<double>> &Z,
                                    const std::vector<std::vector<double>> &C) {
        axes_silencer temp_silencer_{this};

        surface_handle l = std::make_shared<class surface>(this, X, Y, Z, C);
        l->contour_base(true);
        l->hidden_3d(C.empty());
        this->emplace_object(l);

        return l;
    }

    /// Waterfall - Core function
    surface_handle
    axes_type::waterfall(const std::vector<std::vector<double>> &X,
                         const std::vector<std::vector<double>> &Y,
                         const std::vector<std::vector<double>> &Z,
                         const std::vector<std::vector<double>> &C) {
        axes_silencer temp_silencer_{this};

        surface_handle l = this->mesh(X, Y, Z, C);
        l->waterfall(true);

        return l;
    }

    /// Graph - Undirected - Core function
    network_handle
    axes_type::graph(const std::vector<std::pair<size_t, size_t>> &edges,
                     const std::vector<double> &weights, size_t n_vertices,
                     std::string_view line_spec) {
        axes_silencer temp_silencer_{this};

        network_handle l = std::make_shared<class network>(
            this, edges, weights, n_vertices, line_spec);
        this->emplace_object(l);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        if (this->x_axis().limits_mode_auto()) {
            this->x_axis().limits({l->xmin(), l->xmax()});
        }
        if (this->y_axis().limits_mode_auto()) {
            this->y_axis().limits({l->ymin(), l->ymax()});
        }

        return l;
    }

    network_handle
    axes_type::graph(const std::vector<std::pair<size_t, size_t>> &edges,
                     std::string_view line_spec) {
        return this->graph(edges, vector_1d{}, 0, line_spec);
    }

    /// B&W image show - Core function
    matrix_handle axes_type::imshow(
        const std::vector<std::vector<unsigned char>> &gray_scale_img) {
        axes_silencer temp_silencer_{this};

        // create matrix in the xlim
        matrix_handle img =
            std::make_shared<class matrix>(this, gray_scale_img);
        img->always_hide_labels(true);
        this->emplace_object(img);
        this->axis(equal);
        this->color(this->parent()->color());
        this->colormap(palette::gray());
        this->box(false);
        this->grid(false);
        this->minor_grid(false);
        this->grid_front(false);
        this->color_box(false);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->y_axis().reverse(true);
        this->color_box_range(0, 255);

        // plot if not quiet
        return img;
    }

    /// Core RGB / RGBA imshow function
    matrix_handle axes_type::imshow(
        const std::vector<std::vector<unsigned char>> &r_channel,
        const std::vector<std::vector<unsigned char>> &g_channel,
        const std::vector<std::vector<unsigned char>> &b_channel,
        const std::vector<std::vector<unsigned char>> &a_channel) {
        axes_silencer temp_silencer_{this};
        // Create matrix in the xlim
        matrix_handle img = std::make_shared<class matrix>(
            this, r_channel, g_channel, b_channel, a_channel);
        this->emplace_object(img);
        this->axis(equal);
        this->color(this->parent()->color());
        this->colormap(palette::gray());
        this->box(false);
        this->grid(false);
        this->minor_grid(false);
        this->grid_front(false);
        this->color_box(false);
        this->x_axis().visible(false);
        this->y_axis().visible(false);
        this->y_axis().reverse(true);
        this->color_box_range(0, 255);
        return img;
    }

    /// Image show from 2d vectors with image channels
    matrix_handle axes_type::imshow(
        const std::vector<std::vector<std::vector<unsigned char>>> &img) {
        axes_silencer temp_silencer_{this};
        if (img.size() == 1) {
            return this->imshow(img[0]);
        } else if (img.size() == 3) {
            return this->imshow(img[0], img[1], img[2]);
        } else if (img.size() == 4) {
            return this->imshow(img[0], img[1], img[2], img[3]);
        } else {
            return matrix_handle{nullptr};
        }
    }

    /// Image show from filename
    matrix_handle axes_type::imshow(const std::string &filename) {
        image_channels_t image = imread(filename);
        return this->imshow(image);
    }

    /// Display array as image
    matrix_handle axes_type::image(const std::vector<std::vector<double>> &C,
                                   bool scaled_colorbar) {
        axes_silencer temp_silencer_{this};

        // create matrix in the xlim
        matrix_handle img = std::make_shared<class matrix>(this, C);
        img->always_hide_labels(true);
        this->emplace_object(img);
        this->color(this->parent()->color());
        this->box(true);
        this->color_box(false);
        if (this->children().size() == 1) {
            this->x_axis().limits({img->xmin(), img->xmax()});
            this->y_axis().limits({img->ymin(), img->ymax()});
        }
        this->y_axis().reverse(true);
        if (!scaled_colorbar) {
            this->color_box_range(0, 255);
        }

        // plot if not quiet
        return img;
    }

    /// Display 3 arrays as image
    matrix_handle
    axes_type::image(const std::vector<std::vector<double>> &r_channel,
                     const std::vector<std::vector<double>> &g_channel,
                     const std::vector<std::vector<double>> &b_channel,
                     bool scaled_colorbar) {
        axes_silencer temp_silencer_{this};

        // create matrix in the xlim
        matrix_handle img = std::make_shared<class matrix>(
            this, r_channel, g_channel, b_channel);
        img->always_hide_labels(true);
        this->emplace_object(img);
        this->color(this->parent()->color());
        this->box(true);
        this->color_box(false);
        if (this->children().size() == 1) {
            this->x_axis().limits({img->xmin(), img->xmax()});
            this->y_axis().limits({img->ymin(), img->ymax()});
        }
        if (!scaled_colorbar) {
            this->color_box_range(0, 255);
        }

        // plot if not quiet
        return img;
    }

    /// Show 2d-array as image
    matrix_handle axes_type::image(double x_min, double x_max, double y_min,
                                   double y_max,
                                   const std::vector<std::vector<double>> &C,
                                   bool scaled_colorbar) {
        axes_silencer temp_silencer_{this};

        matrix_handle img = this->image(C, scaled_colorbar);
        img->x(x_min);
        img->w(x_max - x_min + 1);
        img->y(y_min);
        img->h(y_max - y_min + 1);
        this->x_axis()

            .limits({img->

                     xmin(),
                     img

                         ->xmax()

            });
        this->y_axis()

            .limits({img->

                     ymin(),
                     img

                         ->ymax()

            });

        // plot if not quiet
        return img;
    }

    /// Show image from tuple of rgb 2d vectors
    matrix_handle
    axes_type::image(const std::tuple<std::vector<std::vector<double>>,
                                      std::vector<std::vector<double>>,
                                      std::vector<std::vector<double>>> &img) {
        return this->image(std::get<0>(img), std::get<1>(img), std::get<2>(img),
                           false);
    }

    /// Annotate plot with text
    /// \param x Positions
    /// \param y Positions
    /// \param texts Texts
    /// \return
    labels_handle axes_type::text(const vector_1d &x, const vector_1d &y,
                                  const std::vector<std::string> &texts) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();

        this->next_plot_replace(false);
        labels_handle l = std::make_shared<class labels>(this, x, y, texts);
        l->alignment(labels::alignment::left);
        this->emplace_object(l);

        this->next_plot_replace(p2);
        return l;
    }

    /// Annotate plot with single text
    labels_handle axes_type::text(double x, double y, std::string_view str) {
        return this->text(std::vector{x}, std::vector{y},
                          std::vector{std::string(str)});
    }

    /// Annotate plot with same text at many positions
    labels_handle axes_type::text(const vector_1d &x, const vector_1d &y,
                                  std::string_view str) {
        return this->text(x, y,
                          std::vector<std::string>(x.size(), std::string(str)));
    }

    /// Annotate plot with arrow
    vectors_handle axes_type::arrow(double x1, double y1, double x2,
                                    double y2) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        double u = x2 - x1;
        double v = y2 - y1;
        vectors_handle q =
            std::make_shared<class vectors>(this, vector_1d{x1}, vector_1d{y1},
                                            vector_1d{u}, vector_1d{v}, "-k");
        q->line_width(1.5);
        this->emplace_object(q);

        this->next_plot_replace(p2);
        return q;
    }

    /// Annotate plot with single line between two points
    line_handle axes_type::line(double x1, double y1, double x2, double y2) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        auto l = this->plot(std::vector{x1, x2}, std::vector{y1, y2}, "-k");
        l->line_width(1.5);

        this->next_plot_replace(p2);
        return l;
    }

    /// Annotate plot with text and arrow
    std::pair<labels_handle, vectors_handle>
    axes_type::textarrow(double x1, double y1, double x2, double y2,
                         std::string_view str) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        auto t = this->text(x1, y1, str);
        double u = x2 - x1;
        if (u > 0) {
            t->alignment(labels::alignment::right);
        }
        auto q = this->arrow(x1, y1, x2, y2);

        this->next_plot_replace(p2);
        return std::make_pair(t, q);
    }

    /// Annotate plot with rectangle
    line_handle axes_type::rectangle(double x, double y, double w, double h,
                                     double curvature) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        line_handle l = nullptr;
        if (curvature == 0.) {
            std::vector<double> x_ = {x, x + w, x + w, x, x};
            std::vector<double> y_ = {y, y, y + h, y + h, y};
            l = plot(x_, y_, "-k");
        } else {
            // Radius of the round section of the rectangle
            double radius = min(w, h) * 0.5 * curvature;

            // Rounded corners
            auto [xne, yne] = pol2cart(linspace(0, pi / 2), radius);
            auto [xnw, ynw] = pol2cart(linspace(pi / 2, pi), radius);
            auto [xsw, ysw] = pol2cart(linspace(pi, 3 * pi / 2), radius);
            auto [xse, yse] = pol2cart(linspace(3 * pi / 2, 2 * pi), radius);

            // Use corners
            std::vector<double> x_;
            x_ = concat(x_, transform(xne, [&](double c) {
                            return x + w - radius + c;
                        }));
            x_ = concat(
                x_, transform(xnw, [&](double c) { return x + radius + c; }));
            x_ = concat(
                x_, transform(xsw, [&](double c) { return x + radius + c; }));
            x_ = concat(x_, transform(xse, [&](double c) {
                            return x + w - radius + c;
                        }));
            x_.emplace_back(x_.front());

            std::vector<double> y_;
            y_ = concat(y_, transform(yne, [&](double c) {
                            return y + h - radius + c;
                        }));
            y_ = concat(y_, transform(ynw, [&](double c) {
                            return y + h - radius + c;
                        }));
            y_ = concat(
                y_, transform(ysw, [&](double c) { return y + radius + c; }));
            y_ = concat(
                y_, transform(yse, [&](double c) { return y + radius + c; }));
            y_.emplace_back(y_.front());

            l = plot(x_, y_, "-k");
        }

        this->next_plot_replace(p2);
        return l;
    }

    std::pair<labels_handle, line_handle>
    axes_type::textbox(double x, double y, double w, double h,
                       std::string_view str) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        auto r = rectangle(x, y, w, h);
        auto t = text(x + w / 2, y + h / 2, str);
        t->alignment(labels::alignment::center);

        this->next_plot_replace(p2);
        return std::make_pair(t, r);
    }

    line_handle axes_type::fill(const vector_1d &x, const vector_1d &y,
                                std::string_view line_spec) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        auto l = this->plot(x, y, line_spec);
        l->fill(true);

        this->next_plot_replace(p2);
        return l;
    }

    line_handle axes_type::ellipse(double x, double y, double w, double h) {
        axes_silencer temp_silencer_{this};
        bool p2 = this->next_plot_replace();
        this->next_plot_replace(false);

        auto t = linspace(0, 2 * pi);
        auto [x_, y_] = elliptic2cart(t, w / 2., h / 2.);
        x_ = transform(x_, [&](double x_) { return x_ + w / 2 + x; });
        y_ = transform(y_, [&](double y_) { return y_ + h / 2 + y; });
        auto l = plot(x_, y_, "-k");

        this->next_plot_replace(p2);
        return l;
    }

    /// Create boxplot from vector 1d
    box_chart_handle axes_type::boxplot(const std::vector<double> &data) {
        box_chart_handle l = std::make_shared<class box_chart>(this, data);
        this->emplace_object(l);
        return l;
    }

    /// Create boxplot from vector 2d
    box_chart_handle
    axes_type::boxplot(const std::vector<std::vector<double>> &data) {
        std::vector<double> y_data;
        std::vector<double> x_data;
        size_t index = 1;
        for (auto it_i = data.begin(); it_i != data.end(); ++it_i) {
            for (auto it_j = it_i->begin(); it_j != it_i->end(); ++it_j) {
                y_data.emplace_back(*it_j);
                x_data.emplace_back(static_cast<double>(index));
            }
            ++index;
        }
        box_chart_handle l =
            std::make_shared<class box_chart>(this, y_data, x_data);
        this->emplace_object(l);
        this->x_axis().tick_values(
            iota(1., 1., static_cast<double>(data.size())));
        return l;
    }

    /// Create boxplot with groups
    box_chart_handle axes_type::boxplot(const std::vector<double> &data,
                                        const std::vector<double> &groups) {
        box_chart_handle l =
            std::make_shared<class box_chart>(this, data, groups);
        this->emplace_object(l);
        return l;
    }

    /// Create boxplot with group strings
    box_chart_handle
    axes_type::boxplot(const std::vector<double> &y_data,
                       const std::vector<std::string> &groups) {
        // map<group,index>
        std::map<std::string, double> category_indexes;
        std::vector<double> group_indexes;
        for (const std::string &group : groups) {
            // look for group
            auto it = category_indexes.find(group);
            // if already there
            if (it != category_indexes.end()) {
                // keep its numeric equivalent
                group_indexes.emplace_back(it->second);
            } else {
                // put in the set and get number
                double group_num =
                    static_cast<double>(category_indexes.size()) + 1.;
                category_indexes[group] = group_num;
                group_indexes.emplace_back(group_num);
            }
        }
        box_chart_handle h = this->boxplot(y_data, group_indexes);
        this->xticks(iota(1., static_cast<double>(category_indexes.size())));

        std::vector<std::string> unique_categories(category_indexes.size());
        for (const auto &[category, index] : category_indexes) {
            unique_categories[static_cast<size_t>(round(index) - 1)] = category;
        }
        this->x_axis().ticklabels(unique_categories);
        this->x_axis().limits(
            {0.5, static_cast<double>(category_indexes.size()) + 0.5});
        return h;
    }

    void axes_type::draw_path(const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::array<float, 4> &color) {
        // we still have to make limits calculate and return the
        // automatic limits rather than the default limits
        auto xlimits = xlim();
        auto ylimits = ylim();
        // clamp
        std::vector<double> cx = transform(
            x, [&](double x) { return std::clamp(x, xlimits[0], xlimits[1]); });
        std::vector<double> cy = transform(
            y, [&](double y) { return std::clamp(y, ylimits[0], ylimits[1]); });
        // normalize
        auto [w, h, lm, rm, bm, tm] = calculate_margins();
        (void)w;
        (void)h;
        double view_width = parent_->backend_->width();
        double view_xmin = lm * view_width;
        double view_xmax = rm * view_width;
        double view_height = parent_->backend_->height();
        double view_ymin = bm * view_height;
        double view_ymax = tm * view_height;
        auto xrange = xlimits[1] - xlimits[0];
        auto yrange = ylimits[1] - ylimits[0];
        for (auto &v : cx) {
            v -= xlimits[0];
            v /= xrange;
            v *= view_xmax - view_xmin;
            v += view_xmin;
        }
        for (auto &v : cy) {
            v -= ylimits[0];
            v /= yrange;
            v *= view_ymax - view_ymin;
            v += view_ymin;
        }
        // draw the normalized path to the backend
        parent_->backend_->draw_path(cx, cy, color);
    }

} // namespace matplot
