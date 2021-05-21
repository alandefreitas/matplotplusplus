//
// Created by Alan Freitas on 2020-07-04.
//

#ifndef MATPLOTPLUSPLUS_AXES_TYPE_H
#define MATPLOTPLUSPLUS_AXES_TYPE_H

#include <optional>

#include <matplot/util/colors.h>
#include <matplot/util/handle_types.h>
#include <matplot/util/keywords.h>

#include <matplot/core/axis_type.h>
#include <matplot/core/legend.h>
#include <matplot/core/line_spec.h>

#include <matplot/axes_objects/error_bar.h>
#include <matplot/axes_objects/function_line.h>
#include <matplot/axes_objects/histogram.h>
#include <matplot/axes_objects/line.h>

namespace matplot {
    class axes_type : public std::enable_shared_from_this<class axes_type> {
      public:
        // {left bottom right top}
        static constexpr std::array<float, 4> default_subplot_inset{.2f, .18f,
                                                                    .04f, .1f};
        // {x, y, width, height}
        static constexpr std::array<float, 4> default_axes_position{
            .13f, .11f, .775f, .815f};

      public:
        axes_type();

        explicit axes_type(class figure_type *parent);

        axes_type(class figure_type *parent, std::array<float, 4> position);

        explicit axes_type(figure_handle parent);

        axes_type(figure_handle parent, std::array<float, 4> position);

        virtual ~axes_type() = default;

      public /* functions that operate the axes */:
        /// Plot parent figure
        void draw();

        /// Touch parent figure
        void touch();

        /// Put an object in the axes
        void emplace_object(axes_object_handle obj);

        /// Put object derived from axes_object_handle in the axes
        template <class T> void emplace_object(std::shared_ptr<T> obj) {
            std::enable_if_t<std::is_base_of_v<axes_object, T>,
                             axes_object_handle>
                ah = std::dynamic_pointer_cast<axes_object>(obj);
            emplace_object(ah);
        }

        /// Run commands to plot axes on the parent figure
        void run_commands();
        void run_draw_commands();

        /// Run command on the parent figure
        void run_command(const std::string &command);

        /// Include a comment on the parent figure (command with #)
        void include_comment(const std::string &command);

        /// Check the type of axes
        bool is_3d();

        bool is_3d_map();

        bool is_2d();

        bool is_polar();

        /// Copy clone the axes and put it in another figure
        axes_handle copy(figure_handle parent);

        /// Child limits (x_min, x_max, y_min, y_max)
        std::array<double, 4> child_limits() const;

      public /* axes limits and axis ratios */:
        /// \brief Sets limits on x-axis and y-axis
        void axis(const std::array<double, 4> &limits_x_y);

        /// \brief Make axes visible / invisible
        void axis(bool v);

        /// \brief Make x/y/z-axis limits automatic
        void axis(keyword_automatic_type automatic);

        /// \brief Make x/y/z-axis limits manual
        void axis(keyword_manual_type manual);

        /// \brief Reverse y-axis
        void axis(keyword_ij_type);

        /// \brief Make x/y-axis ratio = 1 by changing the axis limits
        void axis(keyword_equal_type);

        /// \brief Make x/y/z-axis limits tight from x/y/z-min to x/y/z-max
        void axis(keyword_tight_type);

        /// \brief Make x/y-axis square with ratio 1 by changing the axes size
        void axis(keyword_square_type);

      public /* properties */:
        const std::array<float, 4> &position() const;

        void position(const std::array<float, 4> &position);

        float azimuth() const;

        void azimuth(float azimuth);

        float elevation() const;

        void elevation(float elevation);

        std::pair<float, float> view() const;

        void view(float azimuth, float elevation);

        void rotate(float azimuth, float elevation);

        const float &x_origin() const;

        void x_origin(float x);

        const float &y_origin() const;

        void y_origin(float y);

        const float &width() const;

        void width(float w);

        const float &height() const;

        void height(float h);

        const class axis_type &x_axis() const;

        class axis_type &x_axis();

        std::array<double, 2> xlim() const;

        void xlim(const std::array<double, 2> &);

        void xlim(keyword_automatic_type);

        void xlim(keyword_manual_type);

        const std::string &xlabel() const;

        void xlabel(std::string_view str);

        const std::string &xtickformat() const;

        void xtickformat(std::string_view str);

        const std::vector<double> &xticks() const;

        void xticks(const std::vector<double> &ticks);

        const std::vector<std::string> &xticklabels() const;

        void xticklabels(const std::vector<std::string> &labels);

        void xtickangle(double degrees);

        double xtickangle();

        const class axis_type &x2_axis() const;

        class axis_type &x2_axis();

        std::array<double, 2> x2lim() const;

        void x2lim(const std::array<double, 2> &);

        void x2lim(keyword_automatic_type);

        void x2lim(keyword_manual_type);

        const std::string &x2label() const;

        void x2label(std::string_view str);

        const std::string &x2tickformat() const;

        void x2tickformat(std::string_view str);

        const std::vector<double> &x2ticks() const;

        void x2ticks(const std::vector<double> &ticks);

        const std::vector<std::string> &x2ticklabels() const;

        void x2ticklabels(const std::vector<std::string> &labels);

        void x2tickangle(double degrees);

        double x2tickangle();

        const class axis_type &y_axis() const;

        class axis_type &y_axis();

        std::array<double, 2> ylim() const;

        void ylim(const std::array<double, 2> &);

        void ylim(keyword_automatic_type);

        void ylim(keyword_manual_type);

        const std::string &ylabel() const;

        void ylabel(std::string_view str);

        const std::string &ytickformat() const;

        void ytickformat(std::string_view str);

        const std::vector<double> &yticks() const;

        void yticks(const std::vector<double> &ticks);

        const std::vector<std::string> &yticklabels() const;

        void yticklabels(const std::vector<std::string> &labels);

        void ytickangle(double degrees);

        double ytickangle();

        const class axis_type &y2_axis() const;

        class axis_type &y2_axis();

        std::array<double, 2> y2lim() const;

        void y2lim(const std::array<double, 2> &);

        void y2lim(keyword_automatic_type);

        void y2lim(keyword_manual_type);

        const std::string &y2label() const;

        void y2label(std::string_view str);

        const std::string &y2tickformat() const;

        void y2tickformat(std::string_view str);

        const std::vector<double> &y2ticks() const;

        void y2ticks(const std::vector<double> &ticks);

        const std::vector<std::string> &y2ticklabels() const;

        void y2ticklabels(const std::vector<std::string> &labels);

        void y2tickangle(double degrees);

        double y2tickangle();

        const class axis_type &z_axis() const;

        class axis_type &z_axis();

        std::array<double, 2> zlim() const;

        void zlim(const std::array<double, 2> &);

        void zlim(keyword_automatic_type);

        void zlim(keyword_manual_type);

        const std::string &zlabel() const;

        void zlabel(std::string_view str);

        const std::string &ztickformat() const;

        void ztickformat(std::string_view str);

        const std::vector<double> &zticks() const;

        void zticks(const std::vector<double> &ticks);

        const std::vector<std::string> &zticklabels() const;

        void zticklabels(const std::vector<std::string> &labels);

        void ztickangle(double degrees);

        double ztickangle();

        const class axis_type &cb_axis() const;

        class axis_type &cb_axis();

        std::array<double, 2> cblim() const;

        void cblim(const std::array<double, 2> &);

        void cblim(keyword_automatic_type);

        void cblim(keyword_manual_type);

        const std::string &cblabel() const;

        void cblabel(std::string_view str);

        const std::string &cbtickformat() const;

        void cbtickformat(std::string_view str);

        const std::vector<double> &cbticks() const;

        void cbticks(const std::vector<double> &ticks);

        const std::vector<std::string> &cbticklabels() const;

        void cbticklabels(const std::vector<std::string> &labels);

        void cbtickangle(double degrees);

        double cbtickangle();

        bool cb_vertical() const;

        void cb_vertical(bool cb_vertical);

        bool cb_inside() const;

        void cb_inside(bool cb_inside);

        const std::array<float, 4> &cb_position() const;

        void cb_position(const std::array<float, 4> &cb_position);

        const class axis_type &r_axis() const;

        class axis_type &r_axis();

        std::array<double, 2> rlim() const;

        void rlim(const std::array<double, 2> &);

        void rlim(keyword_automatic_type);

        void rlim(keyword_manual_type);

        const std::string &rlabel() const;

        void rlabel(std::string_view str);

        const std::string &rtickformat() const;

        void rtickformat(std::string_view str);

        const std::vector<double> &rticks() const;

        void rticks(const std::vector<double> &ticks);

        const std::vector<std::string> &rticklabels() const;

        void rticklabels(const std::vector<std::string> &labels);

        void rtickangle(double degrees);

        double rtickangle();

        const class axis_type &t_axis() const;

        class axis_type &t_axis();

        std::array<double, 2> tlim() const;

        void tlim(const std::array<double, 2> &);

        void tlim(keyword_automatic_type);

        void tlim(keyword_manual_type);

        void t_axis(const class axis_type &t_axis);

        bool title_visible() const;

        void title_visible(bool title_visible);

        bool visible() const;

        void visible(bool visible);

        void limits_mode(keyword_automatic_type automatic);

        void limits_mode(keyword_manual_type manual);

        void limits_mode_automatic(bool automatic);

        bool limits_mode_automatic() const;

        void limits_mode_manual(bool automatic);

        bool limits_mode_manual() const;

        const std::string &title() const;

        void title(std::string_view title);

        bool title_enhanced() const;

        void title_enhanced(bool title_enhanced);

        bool axes_aspect_ratio_auto() const;

        void axes_aspect_ratio_auto(bool axes_aspect_ratio_auto);

        float axes_aspect_ratio() const;

        void axes_aspect_ratio(float x);

        const std::string &font() const;

        void font(std::string_view font);

        float font_size() const;

        void font_size(float font_size);

        const std::string &font_weight() const;

        void font_weight(std::string_view font_weight);

        bool clipping() const;

        void clipping(bool clipping);

        float label_font_size_multiplier() const;

        void label_font_size_multiplier(float label_font_size_multiplier);

        float title_font_size_multiplier() const;

        void title_font_size_multiplier(float title_font_size_multiplier);

        const std::string &title_font_weight() const;

        void title_font_weight(std::string_view title_font_weight);

        const color_array &title_color() const;

        void title_color(const color_array &title_color);

        const color_array &color() const;

        void color(const color_array &color);

        void color(const std::array<float, 3> &color);

        void color(std::string_view c);

        void color(const enum color &c);

        const std::vector<color_array> &colororder() const;

        void colororder(const std::vector<color_array> &colororder);

        template <class T> void colororder(const std::vector<T> &order) {
            std::vector<color_array> cs;
            for (const auto &c : order) {
                cs.emplace_back(to_array(c));
            }
            colororder(cs);
        }

        size_t colororder_index() const;

        void colororder_index(size_t colororder_index);

        void bump_colororder_index();

        color_array get_color_and_bump();

        const color_array grid_color() const;

        void grid_color(const color_array &grid_color);

        float grid_alpha() const;

        void grid_alpha(float grid_alpha);

        bool handle_visibility() const;

        void handle_visibility(bool handle_visibility);

        float minor_grid_alpha() const;

        void minor_grid_alpha(float minor_grid_alpha);

        bool minor_grid_alpha_mode() const;

        void minor_grid_alpha_mode(bool minor_grid_alpha_mode);

        const color_array minor_grid_color() const;

        void minor_grid_color(const color_array &minor_grid_color);

        line_spec minor_grid_line_style() const;

        void minor_grid_line_style(line_spec minor_grid_line_style);

        bool grid() const;

        void grid(bool grid);

        bool minor_grid() const;

        void minor_grid(bool minor_grid);

        bool x_grid() const;

        void x_grid(bool x_grid);

        bool x_minor_grid() const;

        void x_minor_grid(bool x_minor_grid);

        bool y_grid() const;

        void y_grid(bool y_grid);

        bool y_minor_grid() const;

        void y_minor_grid(bool y_minor_grid);

        bool z_grid() const;

        void z_grid(bool z_grid);

        bool z_minor_grid() const;

        void z_minor_grid(bool z_minor_grid);

        bool r_grid() const;

        void r_grid(bool r_grid);

        bool r_minor_grid() const;

        void r_minor_grid(bool r_minor_grid);

        line_spec grid_line_style() const;

        void grid_line_style(line_spec grid_line_style);

        const std::string &line_style_order() const;

        void line_style_order(std::string_view line_style_order);

        size_t line_style_order_index() const;

        void line_style_order_index(size_t line_style_order_index);

        float line_width() const;

        void line_width(float line_width);

        bool next_plot_replace() const;

        void next_plot_replace(bool next_plot_replace);

        bool hold() const;

        void hold(bool v);

        const std::vector<std::shared_ptr<axes_object>> &children() const;

        void
        children(const std::vector<std::shared_ptr<axes_object>> &children);

        void clear();

        const legend_handle &legend() const;

        void legend(const legend_handle &legend);

        void legend(const std::vector<std::string> &names);

        bool box() const;

        void box(bool box);

        bool box_full() const;

        void box_full(bool box);

        const std::vector<std::vector<double>> &colormap() const;

        void colormap(const std::vector<std::vector<double>> &colormap);

        color_array colormap_interpolation(double value, double min,
                                           double max);

        size_t max_colors() const;

        void max_colors(size_t max_colors);

        bool color_box() const;

        void color_box(bool color_box);

        const std::pair<double, double> color_box_range() const;

        void color_box_range(const std::pair<double, double> &color_box_range);

        void color_box_range(double cb_min, double cb_max);

        bool color_box_log_scale() const;

        void color_box_log_scale(bool color_box_log_scale);

        bool grid_front() const;

        void grid_front(bool grid_front);

        const class figure_type *parent() const;

        class figure_type *parent();

        void parent(class figure_type *);

      public /* create plots on the axes */:
        /// Create simple line plot
        line_handle plot(const std::vector<double> &x,
                         const std::vector<double> &y,
                         std::string_view line_spec = "");

        /// Create line plot with automatic x = 1,2,...,n
        line_handle plot(const std::vector<double> &y,
                         std::string_view line_spec = "");

        /// \brief Create many line plots at once with parameter pack
        /// First two parameters are always 1) x and y or 2) y and line spec
        /// If first two parameters are x and y, third parameter might be:
        ///     1a) the line spec or 1b) the x2 for the next plot
        /// If first two parameters are y and line spec, third parameter might
        /// be:
        ///     2a) the x2 for next plot (case 1 for next plot) or
        ///     2b) the y for next plot (case 2 for next plot)
        /// This function represents the case 1a, where 3rd parameter line spec
        /// is a divider between plots
        template <class... Args>
        auto plot(const std::vector<double> &x, const std::vector<double> &y,
                  std::string_view line_spec, Args &&...args) {
            std::vector<line_handle> result;
            std::vector<line_handle> result_a =
                vectorize(this->plot(x, y, line_spec));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<line_handle> result_b =
                vectorize(this->plot(std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// \brief Create many line plots at once with parameter pack
        /// First two parameters are always 1) x and y or 2) y and line spec
        /// If first two parameters are x and y, third parameter might be:
        ///     1a) the line spec or 1b) the x2 for the next plot
        /// If first two parameters are y and line spec, third parameter might
        /// be:
        ///     2a) the x2 for next plot (case 1 for next plot) or
        ///     2b) the y for next plot (case 2 for next plot)
        /// This function represents the case 1b, where 3rd parameter x2 is a
        /// divider between plots
        template <class... Args>
        auto plot(const std::vector<double> &x, const std::vector<double> &y,
                  const std::vector<double> &x2, Args &&...args) {
            std::vector<line_handle> result;
            std::vector<line_handle> result_a = vectorize(this->plot(x, y));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<line_handle> result_b =
                vectorize(this->plot(x2, std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// \brief Create many line plots at once with parameter pack
        /// First two parameters are always 1) x and y or 2) y and line spec
        /// If first two parameters are x and y, third parameter might be:
        ///     1a) the line spec or 1b) the x2 for the next plot
        /// If first two parameters are y and line spec, third parameter might
        /// be:
        ///     2a) the x2 for next plot (case 1 for next plot) or
        ///     2b) the y for next plot (case 2 for next plot)
        /// This function represents the case 2, where 2nd parameter line spec
        /// is a divider between plots
        template <class... Args>
        auto plot(const std::vector<double> &y, std::string_view line_spec,
                  Args &&...args) {
            std::vector<line_handle> result;
            std::vector<line_handle> result_a =
                vectorize(this->plot(y, line_spec));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<line_handle> result_b =
                vectorize(this->plot(std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// Plot lists of lists
        std::vector<line_handle> plot(const std::vector<double> &x,
                                      const std::vector<std::vector<double>> &Y,
                                      std::string_view line_spec = "");

        /// Plot lists of lists with automatic x
        std::vector<line_handle> plot(const std::vector<std::vector<double>> &Y,
                                      std::string_view line_spec = "");

        /// Plot lines representing a colormap
        std::vector<line_handle>
        rgbplot(const std::vector<std::vector<double>> &colormap);

        /// Plot 3d line plot
        line_handle plot3(const std::vector<double> &x,
                          const std::vector<double> &y,
                          const std::vector<double> &z,
                          std::string_view line_spec = "");

        /// Plot 3d line plot - lists of Xs and Ys
        std::vector<line_handle>
        plot3(const std::vector<std::vector<double>> &X,
              const std::vector<std::vector<double>> &Y,
              const std::vector<double> &z, std::string_view line_spec = "");

        /// 3d-plot lists of Xs, Ys, and Zs
        std::vector<line_handle>
        plot3(const std::vector<std::vector<double>> &X,
              const std::vector<std::vector<double>> &Y,
              const std::vector<std::vector<double>> &Z,
              std::string_view line_spec = "");

        /// \brief Create many 3d line plots at once with parameter pack
        /// The logic is the analogous to the plot function:
        ///     Case 1a: x, y, z, line_spec, next_x, ...
        ///     Case 1b: x, y, z, next_x, ...
        ///     Case 2:  z, line_spec, next_x, ...
        /// This function represents case 1a
        template <class... Args>
        auto plot3(const std::vector<double> &x, const std::vector<double> &y,
                   const std::vector<double> &z, std::string_view line_spec,
                   Args &&...args) {
            std::vector<line_handle> result;
            std::vector<line_handle> result_a =
                vectorize(this->plot3(x, y, z, line_spec));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<line_handle> result_b =
                vectorize(this->plot3(std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// \brief Create many 3d line plots at once with parameter pack
        /// The logic is the analogous to the plot function:
        ///     Case 1a: x, y, z, line_spec, next_x, ...
        ///     Case 1b: x, y, z, next_x, ...
        ///     Case 2:  z, line_spec, next_x, ...
        /// This function represents case 1b
        template <class... Args>
        auto plot3(const std::vector<double> &x, const std::vector<double> &y,
                   const std::vector<double> &z, const std::vector<double> &x2,
                   Args &&...args) {
            std::vector<line_handle> result;
            std::vector<line_handle> result_a = vectorize(this->plot3(x, y, z));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<line_handle> result_b =
                vectorize(this->plot3(x2, std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// Create stairs line plot
        stair_handle stairs(const std::vector<double> &x,
                            const std::vector<double> &y,
                            std::string_view line_spec = "");

        /// Basic stairs function with automatic x
        stair_handle stairs(const std::vector<double> &y,
                            std::string_view line_spec = "");

        /// Stairs with lists of lists (same x for all Y)
        std::vector<stair_handle>
        stairs(const std::vector<double> &x,
               const std::vector<std::vector<double>> &Y,
               std::string_view line_spec = "");

        /// Stairs with lists of lists (one X per Y)
        std::vector<stair_handle>
        stairs(const std::vector<std::vector<double>> &X,
               const std::vector<std::vector<double>> &Y,
               std::string_view line_spec = "");

        /// Stairs with lists of lists with automatic x
        std::vector<stair_handle>
        stairs(const std::vector<std::vector<double>> &Y,
               std::string_view line_spec = "");

        /// \brief Create many stairs at once with parameter pack
        /// The logic is analogous to the plot function:
        ///     Case 1a: x, y, line_spec, next_x
        ///     Case 1b: x, y, next_x
        ///     Case 2 : y, line_spec, next_x
        /// This function represents case 1a
        template <class... Args>
        auto stairs(const std::vector<double> &x, const std::vector<double> &y,
                    std::string_view line_spec, Args &&...args) {
            std::vector<stair_handle> result;
            std::vector<stair_handle> result_a =
                vectorize(this->stairs(x, y, line_spec));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<stair_handle> result_b =
                vectorize(this->stairs(std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// \brief Create many stairs at once with parameter pack
        /// The logic is analogous to the plot function:
        ///     Case 1a: x, y, line_spec, next_x
        ///     Case 1b: x, y, next_x
        ///     Case 2 : y, line_spec, next_x
        /// This function represents case 1b
        template <class... Args>
        auto stairs(const std::vector<double> &x, const std::vector<double> &y,
                    const std::vector<double> &x2, Args &&...args) {
            std::vector<stair_handle> result;
            std::vector<stair_handle> result_a = vectorize(this->stairs(x, y));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<stair_handle> result_b =
                vectorize(this->stairs(x2, std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// \brief Create many stairs at once with parameter pack
        /// The logic is analogous to the plot function:
        ///     Case 1a: x, y, line_spec, next_x
        ///     Case 1b: x, y, next_x
        ///     Case 2 : y, line_spec, next_x
        /// This function represents case 2
        template <class... Args>
        auto stairs(const std::vector<double> &y, std::string_view line_spec,
                    Args &&...args) {
            std::vector<stair_handle> result;
            std::vector<stair_handle> result_a =
                vectorize(this->stairs(y, line_spec));
            bool p = this->next_plot_replace();
            this->next_plot_replace(false);
            std::vector<stair_handle> result_b =
                vectorize(this->stairs(std::forward<Args>(args)...));
            this->next_plot_replace(p);
            result.insert(result.end(), result_a.begin(), result_a.end());
            result.insert(result.end(), result_b.begin(), result_b.end());
            return result;
        }

        /// Core errorbar function
        error_bar_handle
        errorbar(const std::vector<double> &x, const std::vector<double> &y,
                 const std::vector<double> &error,
                 error_bar::type type = error_bar::type::vertical,
                 std::string_view line_spec = "");

        /// Core errorbar function with different values on both directions
        error_bar_handle errorbar(const std::vector<double> &x,
                                  const std::vector<double> &y,
                                  const std::vector<double> &y_neg_delta,
                                  const std::vector<double> &y_pos_delta,
                                  const std::vector<double> &x_neg_delta,
                                  const std::vector<double> &x_pos_delta,
                                  std::string_view line_spec = "");

        /// If there is a string instead of a type the first version, we default
        /// to vertical
        error_bar_handle errorbar(const std::vector<double> &x,
                                  const std::vector<double> &y,
                                  const std::vector<double> &y_error,
                                  std::string_view line_spec);

        /// Core area function (x is a vector, y is a matrix)
        std::vector<filled_area_handle>
        area(const std::vector<double> &x,
             const std::vector<std::vector<double>> &Y, double base_value = 0.,
             bool stacked = true, std::string_view line_spec = "k-");

        /// Area: x is a vector, y is a vector
        filled_area_handle area(const std::vector<double> &x,
                                const std::vector<double> &y,
                                double base_value = 0., bool stacked = true,
                                std::string_view line_spec = "k-");

        /// Area: Automatic x, y is a matrix
        std::vector<filled_area_handle>
        area(const std::vector<std::vector<double>> &Y, double base_value = 0.,
             bool stacked = true, std::string_view line_spec = "k-");

        /// Area: Skip the base value
        std::vector<filled_area_handle>
        area(const std::vector<std::vector<double>> &Y, bool stacked,
             std::string_view line_spec = "k-");

        /// Area: Automatic x, y is a vector
        filled_area_handle area(const std::vector<double> &y,
                                double base_value = 0., bool stacked = true,
                                std::string_view line_spec = "k-");

        /// String function line plot
        string_function_handle fplot(std::string_view equation,
                                     std::string_view line_spec = "");

        /// String function line plots
        std::vector<string_function_handle>
        fplot(std::vector<std::string> equations,
              std::vector<std::string> line_specs = {});

        /// Lambda function line plot
        function_line_handle fplot(function_line::function_type equation,
                                   const std::array<double, 2> &x_range = {-5,
                                                                           5},
                                   std::string_view line_spec = "");

        /// Lambda function line plot - automatic limits
        function_line_handle fplot(function_line::function_type equation,
                                   std::string_view line_spec);

        /// Lambda function line plot - two functions (x/y)
        function_line_handle fplot(function_line::function_type function_x,
                                   function_line::function_type function_y,
                                   const std::array<double, 2> &t_range = {-5,
                                                                           5},
                                   std::string_view line_spec = "");

        /// Lambda function line plot - list of functions
        std::vector<function_line_handle>
        fplot(std::vector<function_line::function_type> equations,
              std::array<double, 2> x_range = {-5, 5},
              std::vector<std::string> line_specs = {});

        /// Lambda function line plot - list of functions and line specs
        std::vector<function_line_handle>
        fplot(std::vector<function_line::function_type> equations,
              std::vector<double> x_range,
              std::vector<std::string> line_specs = {});

        using implicit_function_type = std::function<double(double, double)>;

        /// Implicit lambda function line plot (one-level={0} contour)
        line_handle
        fimplicit(implicit_function_type equation,
                  const std::array<double, 4> &xy_interval = {-5, 5, -5, 5},
                  std::string_view line_spec = "");

        /// Implicit lambda function line plot - automatic xy_interval
        line_handle fimplicit(implicit_function_type equation,
                              std::string_view line_spec);

        /// Lambda function plot 3D
        function_line_handle fplot3(function_line::function_type function_x,
                                    function_line::function_type function_y,
                                    function_line::function_type function_z,
                                    const std::array<double, 2> &t_range = {-5,
                                                                            5},
                                    std::string_view line_spec = "");

        /// Histogram - Choose binning algorithm and normalization algorithm
        histogram_handle hist(const std::vector<double> &data,
                              histogram::binning_algorithm algorithm =
                                  histogram::binning_algorithm::automatic,
                              enum histogram::normalization normalization_alg =
                                  histogram::normalization::count);

        /// Histogram - Fixed number of bins
        /// If n_bins = 0, automatic number of bins
        histogram_handle hist(const std::vector<double> &data, size_t n_bins);

        /// Histogram - Fixed edges
        histogram_handle hist(const std::vector<double> &data,
                              const std::vector<double> &edges);

        /// Histogram - Normalization algorithm
        histogram_handle hist(const std::vector<double> &data,
                              enum histogram::normalization normalization_alg);

        /// Histogram - Categorical histogram from strings
        histogram_handle hist(const std::vector<std::string> &data,
                              enum histogram::normalization normalization_alg =
                                  histogram::normalization::count);

        /// Core binscatter function for a given set of edges
        /// This function returns a generic axes_object_handle because
        /// it might be a scatter plot or a histogram plot
        axes_object_handle binscatter(
            const std::vector<double> &x, const std::vector<double> &y,
            const std::vector<double> &edges_x,
            const std::vector<double> &edges_y,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count);

        /// Binscatter for a given algorithm
        axes_object_handle binscatter(
            const std::vector<double> &x, const std::vector<double> &y,
            histogram::binning_algorithm algorithm =
                histogram::binning_algorithm::automatic,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count);

        /// Binscatter for a given bin scatter style
        axes_object_handle
        binscatter(const std::vector<double> &x, const std::vector<double> &y,
                   enum bin_scatter_style scatter_style,
                   histogram::binning_algorithm algorithm =
                       histogram::binning_algorithm::automatic,
                   enum histogram::normalization normalization_alg =
                       histogram::normalization::count);

        /// Binscatter for a given number of bins
        axes_object_handle binscatter(
            const std::vector<double> &x, const std::vector<double> &y,
            size_t nbins_x, size_t nbins_y,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count);

        /// Create boxplot from vector 1d
        box_chart_handle boxplot(const std::vector<double> &data);

        /// Create boxplot from vector 2d
        box_chart_handle boxplot(const std::vector<std::vector<double>> &data);

        /// Create boxplot with groups
        box_chart_handle boxplot(const std::vector<double> &data,
                                 const std::vector<double> &groups);

        /// Create boxplot with group strings
        box_chart_handle boxplot(const std::vector<double> &y_data,
                                 const std::vector<std::string> &groups);

        /// Core bar function
        bars_handle bar(const std::vector<double> &x,
                        const std::vector<double> &y);

        /// Bar - Automatic x
        bars_handle bar(const std::vector<double> &y);

        /// Bar - Many Y at once, automatic X
        bars_handle bar(const std::vector<std::vector<double>> &Y);

        /// Bar - One X, many Y
        bars_handle bar(const std::vector<double> &x,
                        const std::vector<std::vector<double>> &Y);

        /// Bar - Automatic x - Custom width
        bars_handle bar(const std::vector<double> &y, double width);

        /// Bar stacked - Many Y - Automatic x
        std::vector<bars_handle>
        barstacked(const std::vector<std::vector<double>> &Y);

        /// Bar stacked - One x - Many Y
        std::vector<bars_handle>
        barstacked(const std::vector<double> &x,
                   const std::vector<std::vector<double>> &Y);

        /// Core heatmap function
        matrix_handle heatmap(const std::vector<std::vector<double>> &m);

        /// Pseudocolor plot
        matrix_handle pcolor(const std::vector<std::vector<double>> &m);

        /// Core parallel plot function
        parallel_lines_handle
        parallelplot(const std::vector<std::vector<double>> &X,
                     const std::vector<double> &colors,
                     std::string_view line_spec = "");

        /// Parallel plot - default line colors
        parallel_lines_handle
        parallelplot(const std::vector<std::vector<double>> &X,
                     std::string_view line_spec = "");

        /// Core pie function with explode values and labels
        circles_handle pie(const std::vector<double> &x,
                           const std::vector<double> &explode = {},
                           const std::vector<std::string> &labels = {});

        /// Pie function with labels
        circles_handle pie(const std::vector<double> &x,
                           const std::vector<std::string> &labels);

        /// Core scatter function
        line_handle scatter(const std::vector<double> &x,
                            const std::vector<double> &y,
                            const std::vector<double> &sizes = {},
                            const std::vector<double> &colors = {});

        /// Scatter function with same size for all points
        line_handle scatter(const std::vector<double> &x,
                            const std::vector<double> &y, double sizes,
                            const std::vector<double> &colors = {});

        /// Core scatter3 function
        line_handle scatter3(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &z,
                             const std::vector<double> &sizes = {},
                             const std::vector<double> &colors = {},
                             std::string_view line_spec = "o");

        /// Scatter3 - Default sizes and colors
        line_handle scatter3(const std::vector<double> &x,
                             const std::vector<double> &y,
                             const std::vector<double> &z,
                             std::string_view line_spec);

        /// Core wordcloud function - Wordcloud from words
        labels_handle wordcloud(const std::vector<std::string> &words,
                                const std::vector<double> &sizes,
                                const std::vector<double> &custom_colors = {});

        /// Wordcloud function - Sizes as size_t
        labels_handle wordcloud(const std::vector<std::string> &words,
                                const std::vector<size_t> &sizes,
                                const std::vector<double> &custom_colors = {});

        /// Wordcloud from text
        labels_handle
        wordcloud(std::string_view text,
                  const std::vector<std::string> &black_list,
                  std::string_view delimiters = " ',\n\r\t\".!?:;",
                  size_t max_cloud_size = 100,
                  const std::vector<double> &custom_colors = {});

        /// Core pareto function
        std::pair<bars_handle, line_handle>
        pareto(const std::vector<double> &y,
               const std::vector<std::string> &names = {},
               double p_threshold = 0.95, size_t item_threshold = 10);

        /// Core plot function
        line_handle stem(const std::vector<double> &x,
                         const std::vector<double> &y,
                         std::string_view line_spec = "-o");

        /// Basic plot function with automatic x
        line_handle stem(const std::vector<double> &y,
                         std::string_view line_spec = "-o");

        /// Stem - Plot lists of lists
        std::vector<line_handle> stem(const std::vector<double> &x,
                                      const std::vector<std::vector<double>> &Y,
                                      std::string_view line_spec = "-o");

        /// Plot lists of lists with automatic x
        std::vector<line_handle> stem(const std::vector<std::vector<double>> &Y,
                                      std::string_view line_spec = "-o");

        /// Stem 3d - Core function
        line_handle stem3(const std::vector<double> &x,
                          const std::vector<double> &y,
                          const std::vector<double> &z,
                          std::string_view line_spec = "-o");

        /// 3d-stem lists of Xs and Ys
        std::vector<line_handle>
        stem3(const std::vector<std::vector<double>> &X,
              const std::vector<std::vector<double>> &Y,
              const std::vector<double> &z, std::string_view line_spec = "-o");

        /// Stem 3d - Automatic x and y
        line_handle stem3(const std::vector<double> &z,
                          std::string_view line_spec = "-o");

        /// Stem 3d - Automatic x and y - Many Zs
        line_handle stem3(const std::vector<std::vector<double>> &Z,
                          std::string_view line_spec = "-o");

        /// Core geoplot function
        circles_handle geobubble(const std::vector<double> &latitude,
                                 const std::vector<double> &longitude,
                                 const std::vector<double> &sizes = {},
                                 const std::vector<double> &colors = {});

        /// Core geoplot function
        line_handle geodensityplot(const std::vector<double> &latitude,
                                   const std::vector<double> &longitude,
                                   const std::vector<double> &weights = {});

        /// Plot world map on the axes
        line_handle geoplot();

        /// Core geoplot function - Plot lines on world map
        line_handle geoplot(const std::vector<double> &latitude,
                            const std::vector<double> &longitude,
                            std::string_view line_spec = "");

        /// Adjust limits and reload that region of the world map in the proper
        /// resolution
        void geolimits(const std::array<double, 2> &latitude,
                       const std::array<double, 2> &longitude);

        /// Adjust limits and reload that region of the world map in the proper
        /// resolution
        void geolimits(double latitude_min, double latitude_max,
                       double longitude_min, double longitude_max);

        /// Adjust limits and reload that region of the world map in the proper
        /// resolution
        void geolimits(const std::vector<double> &latitude,
                       const std::vector<double> &longitude);

        /// Core geoscatter function
        line_handle geoscatter(const std::vector<double> &latitude,
                               const std::vector<double> &longitude,
                               const std::vector<double> &sizes = {},
                               const std::vector<double> &colors = {});

        /// Compass plot function
        vectors_handle compass(const std::vector<double> &x,
                               const std::vector<double> &y,
                               std::string_view line_spec = "");

        /// Ezpolar - Plot function on polar plot
        string_function_handle ezpolar(std::string_view equation,
                                       std::string_view line_spec = "");

        std::vector<string_function_handle>
        ezpolar(std::vector<std::string> equations,
                std::vector<std::string> line_specs);

        function_line_handle ezpolar(function_line::function_type equation,
                                     const std::array<double, 2> &t_range,
                                     std::string_view line_spec = "");

        function_line_handle ezpolar(function_line::function_type equation,
                                     std::string_view line_spec = "");

        function_line_handle ezpolar(function_line::function_type equation,
                                     std::vector<double> x_range,
                                     std::string_view line_spec = "");

        function_line_handle ezpolar(function_line::function_type function_x,
                                     function_line::function_type function_y,
                                     const std::array<double, 2> &t_range,
                                     std::string_view line_spec = "");

        function_line_handle ezpolar(function_line::function_type function_x,
                                     function_line::function_type function_y,
                                     std::vector<double> t_range,
                                     std::string_view line_spec = "");

        std::vector<function_line_handle>
        ezpolar(std::vector<function_line::function_type> equations,
                std::array<double, 2> x_range,
                std::vector<std::string> line_specs);

        std::vector<function_line_handle>
        ezpolar(std::vector<function_line::function_type> equations,
                std::vector<double> x_range,
                std::vector<std::string> line_specs);

        /// Polar histogram - Core function
        histogram_handle polarhistogram(const std::vector<double> &theta,
                                        size_t nbins);

        /// Polar plot - Core function
        line_handle polarplot(const std::vector<double> &theta,
                              const std::vector<double> &rho,
                              std::string_view line_spec = "");

        /// Polar plot - Automatic theta
        line_handle polarplot(const std::vector<double> &rho,
                              std::string_view line_spec = "");

        /// Polar plot - Complex numbers
        line_handle polarplot(const std::vector<std::complex<double>> &z,
                              std::string_view line_spec = "*");

        /// Polar scatter - Core function
        line_handle
        polarscatter(const std::vector<double> &theta,
                     const std::vector<double> &rho,
                     const std::vector<double> &sizes = std::vector<double>{},
                     const std::vector<double> &colors = std::vector<double>{},
                     std::string_view line_spec = "o");

        /// Polar scatter - Single size - Default colors
        line_handle polarscatter(const std::vector<double> &theta,
                                 const std::vector<double> &rho, double size,
                                 std::string_view line_spec = "o");

        /// Polar scatter - Default size and colors
        line_handle polarscatter(const std::vector<double> &theta,
                                 const std::vector<double> &rho,
                                 std::string_view line_spec);

        /// Contour - Core function - Manual levels
        contours_handle contour(const std::vector<std::vector<double>> &X,
                                const std::vector<std::vector<double>> &Y,
                                const std::vector<std::vector<double>> &Z,
                                std::vector<double> levels,
                                std::string_view line_spec = "",
                                size_t n_levels = 0);

        /// Contour - Manual number of levels (or 0 for automatic number of
        /// levels)
        contours_handle contour(const std::vector<std::vector<double>> &X,
                                const std::vector<std::vector<double>> &Y,
                                const std::vector<std::vector<double>> &Z,
                                size_t n_levels = 0,
                                std::string_view line_spec = "");

        /// Contour - Automatic levels and number of levels
        contours_handle contour(const std::vector<std::vector<double>> &X,
                                const std::vector<std::vector<double>> &Y,
                                const std::vector<std::vector<double>> &Z,
                                std::string_view line_spec);

        /// Contour filled - Manual levels
        contours_handle contourf(const std::vector<std::vector<double>> &X,
                                 const std::vector<std::vector<double>> &Y,
                                 const std::vector<std::vector<double>> &Z,
                                 std::vector<double> levels,
                                 std::string_view line_spec = "",
                                 size_t n_levels = 0);

        /// Contour filled - Manual number of levels
        contours_handle contourf(const std::vector<std::vector<double>> &X,
                                 const std::vector<std::vector<double>> &Y,
                                 const std::vector<std::vector<double>> &Z,
                                 size_t n_levels = 0,
                                 std::string_view line_spec = "");

        /// Contour filled - Automatic number of levels
        contours_handle contourf(const std::vector<std::vector<double>> &X,
                                 const std::vector<std::vector<double>> &Y,
                                 const std::vector<std::vector<double>> &Z,
                                 std::string_view line_spec);

        using fcontour_function_type = std::function<double(double, double)>;

        /// Lambda function contour - Manual levels (or empty list for
        /// automatic) / Manual number of levels (or 0 for automatic)
        contours_handle fcontour(fcontour_function_type fn,
                                 const std::array<double, 4> &xy_range,
                                 std::vector<double> levels = {},
                                 std::string_view line_spec = "",
                                 size_t n_levels = 0);

        /// Lambda function contour - Manual number of levels (default = 9)
        contours_handle fcontour(fcontour_function_type fn, size_t n_levels = 9,
                                 std::string_view line_spec = "");

        /// Lambda function contour - Automatic number of levels and levels
        contours_handle fcontour(fcontour_function_type fn,
                                 std::string_view line_spec);

        /// Feather - Core function
        vectors_handle feather(const std::vector<double> &u,
                               const std::vector<double> &v,
                               std::string_view line_spec = "");

        /// Quiver - Core function
        vectors_handle quiver(const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::vector<double> &u,
                              const std::vector<double> &v,
                              const std::vector<double> &c, double scale = 1.0,
                              std::string_view line_spec = "");

        /// Quiver - 2d x,y,u,v
        vectors_handle quiver(const std::vector<std::vector<double>> &x,
                              const std::vector<std::vector<double>> &y,
                              const std::vector<std::vector<double>> &u,
                              const std::vector<std::vector<double>> &v,
                              const std::vector<std::vector<double>> &c,
                              double scale = 1.0,
                              std::string_view line_spec = "");

        /// Quiver - 2d x,y,u,v with no color mapping
        vectors_handle quiver(const std::vector<std::vector<double>> &x,
                              const std::vector<std::vector<double>> &y,
                              const std::vector<std::vector<double>> &u,
                              const std::vector<std::vector<double>> &v,
                              double scale = 1.0,
                              std::string_view line_spec = "");

        /// Quiver - x,y,u,v with no color mapping
        vectors_handle quiver(const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::vector<double> &u,
                              const std::vector<double> &v, double scale = 1.0,
                              std::string_view line_spec = "");

        /// Quiver 3d - Core function
        vectors_handle
        quiver3(const std::vector<double> &x, const std::vector<double> &y,
                const std::vector<double> &z, const std::vector<double> &u,
                const std::vector<double> &v, const std::vector<double> &w,
                const std::vector<double> &c, double scale = 1.0,
                std::string_view line_spec = "");

        /// Quiver 3d - 2d vectors
        vectors_handle quiver3(const std::vector<std::vector<double>> &x,
                               const std::vector<std::vector<double>> &y,
                               const std::vector<std::vector<double>> &z,
                               const std::vector<std::vector<double>> &u,
                               const std::vector<std::vector<double>> &v,
                               const std::vector<std::vector<double>> &w,
                               const std::vector<std::vector<double>> &c,
                               double scale = 1.0,
                               std::string_view line_spec = "");

        /// Quiver 3d - 2d vectors no color mapping
        vectors_handle quiver3(const std::vector<std::vector<double>> &x,
                               const std::vector<std::vector<double>> &y,
                               const std::vector<std::vector<double>> &z,
                               const std::vector<std::vector<double>> &u,
                               const std::vector<std::vector<double>> &v,
                               const std::vector<std::vector<double>> &w,
                               double scale = 1.0,
                               std::string_view line_spec = "");

        /// Quiver 3d - Automatic x and y - 2d vectors
        vectors_handle quiver3(const std::vector<std::vector<double>> &z,
                               const std::vector<std::vector<double>> &u,
                               const std::vector<std::vector<double>> &v,
                               const std::vector<std::vector<double>> &w,
                               const std::vector<std::vector<double>> &c,
                               double scale = 1.0,
                               std::string_view line_spec = "");

        /// Quiver 3d - Automatic x and y - 2d vectors no color mapping
        vectors_handle quiver3(const std::vector<std::vector<double>> &z,
                               const std::vector<std::vector<double>> &u,
                               const std::vector<std::vector<double>> &v,
                               const std::vector<std::vector<double>> &w,
                               double scale = 1.0,
                               std::string_view line_spec = "");

        /// Quiver 3d - no color mapping
        vectors_handle
        quiver3(const std::vector<double> &x, const std::vector<double> &y,
                const std::vector<double> &z, const std::vector<double> &u,
                const std::vector<double> &v, const std::vector<double> &w,
                double scale = 1.0, std::string_view line_spec = "");

        /// Fence - Core function
        surface_handle fence(const std::vector<std::vector<double>> &X,
                             const std::vector<std::vector<double>> &Y,
                             const std::vector<std::vector<double>> &Z,
                             const std::vector<double> &c = {});

        // z = f(x,y)
        using fmesh_function_type = std::function<double(double, double)>;

        /// Lambda function mesh - Core function
        surface_handle fmesh(fcontour_function_type fn,
                             const std::array<double, 2> &x_range,
                             const std::array<double, 2> &y_range,
                             double mesh_density = 40);

        /// Lambda function mesh - parametric function
        surface_handle
        fmesh(fcontour_function_type funx, fcontour_function_type funy,
              fcontour_function_type funz, const std::array<double, 2> &u_range,
              const std::array<double, 2> &v_range, double mesh_density = 40);

        /// Lambda function mesh
        /// Grid / Both ranges in the same array size 4
        surface_handle fmesh(fcontour_function_type fn,
                             const std::array<double, 4> &xy_range,
                             double mesh_density = 40);

        /// Lambda function mesh - Parametric
        /// Parametric / Both ranges in the same array size 4
        surface_handle fmesh(fcontour_function_type funx,
                             fcontour_function_type funy,
                             fcontour_function_type funz,
                             const std::array<double, 4> &uv_range,
                             double mesh_density = 40);

        /// Function mesh
        /// Grid / Both ranges in the same array size 2
        surface_handle fmesh(fcontour_function_type fn,
                             const std::array<double, 2> &xy_range = {-5, +5},
                             double mesh_density = 40);

        /// Function mesh
        /// Parametric / Both ranges in the same array size 2
        surface_handle fmesh(fcontour_function_type funx,
                             fcontour_function_type funy,
                             fcontour_function_type funz,
                             const std::array<double, 2> &uv_range = {-5, +5},
                             double mesh_density = 40);

        // z = f(x,y)
        using fsurf_function_type = std::function<double(double, double)>;

        /// Function surf - Core function
        surface_handle fsurf(fcontour_function_type fn,
                             const std::array<double, 2> &x_range,
                             const std::array<double, 2> &y_range,
                             std::string_view line_spec = "",
                             double mesh_density = 40);

        /// Function surf - Parametric
        surface_handle
        fsurf(fcontour_function_type funx, fcontour_function_type funy,
              fcontour_function_type funz, const std::array<double, 2> &u_range,
              const std::array<double, 2> &v_range,
              std::string_view line_spec = "", double mesh_density = 40);

        /// Function surf
        /// Grid / Both ranges in the same array size 4
        surface_handle fsurf(fcontour_function_type fn,
                             const std::array<double, 4> &xy_range,
                             std::string_view line_spec = "",
                             double mesh_density = 40);

        /// Parametric / Both ranges in the same array size 4
        surface_handle fsurf(fcontour_function_type funx,
                             fcontour_function_type funy,
                             fcontour_function_type funz,
                             const std::array<double, 4> &uv_range,
                             std::string_view line_spec = "",
                             double mesh_density = 40);

        /// Function surf
        /// Grid / Both ranges in the same array size 2
        surface_handle fsurf(fcontour_function_type fn,
                             const std::array<double, 2> &xy_range = {-5, +5},
                             std::string_view line_spec = "",
                             double mesh_density = 40);

        /// Function surf
        /// Parametric / Both ranges in the same array size 2
        surface_handle fsurf(fcontour_function_type funx,
                             fcontour_function_type funy,
                             fcontour_function_type funz,
                             const std::array<double, 2> &uv_range = {-5, +5},
                             std::string_view line_spec = "",
                             double mesh_density = 40);

        /// Function surf
        /// Grid / Both ranges in the same array size 2
        surface_handle fsurf(fcontour_function_type fn,
                             std::initializer_list<double> &xy_range,
                             std::string_view line_spec = "",
                             double mesh_density = 40) {
            if (xy_range.size() == 2) {
                return fsurf(fn, to_array<2>(xy_range), line_spec,
                             mesh_density);
            } else if (xy_range.size() == 4) {
                return fsurf(fn, to_array<4>(xy_range), line_spec,
                             mesh_density);
            }
            throw std::invalid_argument(
                    "fsurf: xy_range needs to have size 2 or 4");
        }

        /// Function surf
        /// Parametric / Both ranges in the same array size 2
        inline surface_handle fsurf(fcontour_function_type funx,
                                    fcontour_function_type funy,
                                    fcontour_function_type funz,
                                    std::initializer_list<double> &uv_range,
                                    std::string_view line_spec = "",
                                    double mesh_density = 40) {
            if (uv_range.size() == 2) {
                return fsurf(funx, funy, funz, to_array<2>(uv_range), line_spec,
                             mesh_density);
            } else if (uv_range.size() == 4) {
                return fsurf(funx, funy, funz, to_array<4>(uv_range), line_spec,
                             mesh_density);
            }
            throw std::invalid_argument(
                    "fsurf: uv_range needs to have size 2 or 4");
        }

        /// Mesh - Core function
        surface_handle mesh(const std::vector<std::vector<double>> &X,
                            const std::vector<std::vector<double>> &Y,
                            const std::vector<std::vector<double>> &Z,
                            const std::vector<std::vector<double>> &C = {});

        /// Mesh with contour
        surface_handle meshc(const std::vector<std::vector<double>> &X,
                             const std::vector<std::vector<double>> &Y,
                             const std::vector<std::vector<double>> &Z,
                             const std::vector<std::vector<double>> &C = {});

        /// Mesh with curtain
        /// Core function
        surface_handle meshz(const std::vector<std::vector<double>> &X,
                             const std::vector<std::vector<double>> &Y,
                             const std::vector<std::vector<double>> &Z,
                             const std::vector<std::vector<double>> &C = {});

        /// Ribbon
        surface_handle ribbon(const std::vector<std::vector<double>> &X,
                              const std::vector<std::vector<double>> &Y,
                              const std::vector<std::vector<double>> &Z,
                              const std::vector<std::vector<double>> &C = {},
                              double width = 0.75);

        /// Surf - Core function
        surface_handle surf(const std::vector<std::vector<double>> &X,
                            const std::vector<std::vector<double>> &Y,
                            const std::vector<std::vector<double>> &Z,
                            const std::vector<std::vector<double>> &C = {},
                            std::string_view line_spec = "");

        /// Surf with contour - Core function
        surface_handle surfc(const std::vector<std::vector<double>> &X,
                             const std::vector<std::vector<double>> &Y,
                             const std::vector<std::vector<double>> &Z,
                             const std::vector<std::vector<double>> &C = {});

        /// Waterfall - Core function
        surface_handle
        waterfall(const std::vector<std::vector<double>> &X,
                  const std::vector<std::vector<double>> &Y,
                  const std::vector<std::vector<double>> &Z,
                  const std::vector<std::vector<double>> &C = {});

        /// Graph - Undirected - Core function
        network_handle
        graph(const std::vector<std::pair<size_t, size_t>> &edges,
              const std::vector<double> &weights = {}, size_t n_vertices = 0,
              std::string_view line_spec = "-o");

        network_handle
        graph(const std::vector<std::pair<size_t, size_t>> &edges,
              std::string_view line_spec);

        /// B&W image show - Core function
        matrix_handle
        imshow(const std::vector<std::vector<unsigned char>> &gray_scale_img);

        /// Core RGB / RGBA imshow function
        matrix_handle
        imshow(const std::vector<std::vector<unsigned char>> &r_channel,
               const std::vector<std::vector<unsigned char>> &g_channel,
               const std::vector<std::vector<unsigned char>> &b_channel,
               const std::vector<std::vector<unsigned char>> &a_channel = {});

        /// Image show from 2d vectors with image channels
        matrix_handle
        imshow(const std::vector<std::vector<std::vector<unsigned char>>> &img);

        /// Image show from filename
        matrix_handle imshow(const std::string &filename);

        /// Display array as image
        matrix_handle image(const std::vector<std::vector<double>> &C,
                            bool scaled_colorbar = false);

        /// Display 3 arrays as image
        matrix_handle image(const std::vector<std::vector<double>> &r_channel,
                            const std::vector<std::vector<double>> &g_channel,
                            const std::vector<std::vector<double>> &b_channel,
                            bool scaled_colorbar = false);

        /// Show 2d-array as image
        matrix_handle image(double x_min, double x_max, double y_min,
                            double y_max,
                            const std::vector<std::vector<double>> &C,
                            bool scaled_colorbar = false);

        /// Show image from tuple of rgb 2d vectors
        matrix_handle
        image(const std::tuple<std::vector<std::vector<double>>,
                               std::vector<std::vector<double>>,
                               std::vector<std::vector<double>>> &img);

        /// Annotate plot with text
        /// \param x Positions
        /// \param y Positions
        /// \param texts Texts
        /// \return
        labels_handle text(const std::vector<double> &x,
                           const std::vector<double> &y,
                           const std::vector<std::string> &texts);

        /// Annotate plot with single text
        labels_handle text(double x, double y, std::string_view str);

        /// Annotate plot with same text at many positions
        labels_handle text(const std::vector<double> &x,
                           const std::vector<double> &y, std::string_view str);

        /// Annotate plot with arrow
        vectors_handle arrow(double x1, double y1, double x2, double y2);

        /// Annotate plot with single line between two points
        line_handle line(double x1, double y1, double x2, double y2);

        /// Annotate plot with text and arrow
        std::pair<labels_handle, vectors_handle>
        textarrow(double x1, double y1, double x2, double y2,
                  std::string_view str);

        /// Annotate plot with rectangle
        line_handle rectangle(double x, double y, double w, double h,
                              double curvature = 0.);

        /// Annotate plot with text in a box
        std::pair<labels_handle, line_handle>
        textbox(double x, double y, double w, double h, std::string_view str);

        /// Annotate plot with a filled polygon
        line_handle fill(const std::vector<double> &x,
                         const std::vector<double> &y,
                         std::string_view line_spec = "");

        line_handle ellipse(double x, double y, double w, double h);

      public /* create plots on the axes from ranges we can convert to
                std::vector<double> */
          :
        template <class T1, class T2>
        line_handle plot(const IterableValues<T1> &x,
                         const IterableValues<T2> &y,
                         std::string_view line_spec = "") {
            return plot(to_vector_1d(x), to_vector_1d(y), line_spec);
        }

        template <class T1>
        line_handle plot(const IterableValues<T1> &y,
                         std::string_view line_spec = "") {
            return plot(to_vector_1d(y), line_spec);
        }

        template <class T1, class T2, class... Args>
        auto plot(const IterableValues<T1> &x, const IterableValues<T2> &y,
                  std::string_view line_spec, Args &&...args) {
            return plot(to_vector_1d(x), to_vector_1d(y), line_spec,
                        std::forward<Args>(args)...);
        }

        template <class T1, class T2, class T3, class... Args>
        auto plot(const IterableValues<T1> &x, const IterableValues<T2> &y,
                  const IterableValues<T3> &x2, Args &&...args) {
            return plot(to_vector_1d(x), to_vector_1d(y), to_vector_1d(x2),
                        std::forward<Args>(args)...);
        }

        template <class T1, class... Args>
        auto plot(const IterableValues<T1> &y, std::string_view line_spec,
                  Args &&...args) {
            return plot(to_vector_1d(y), line_spec,
                        std::forward<Args>(args)...);
        }

        template <class T1, class T2>
        std::vector<line_handle> plot(const IterableValues<T1> &x,
                                      const IterableIterables<T2> &Y,
                                      std::string_view line_spec = "") {
            return plot(to_vector_1d(x), to_vector_2d(Y), line_spec);
        }

        template <class T1>
        std::vector<line_handle> plot(const IterableIterables<T1> &Y,
                                      std::string_view line_spec = "") {
            return plot(to_vector_2d(Y), line_spec);
        }

        template <class T1>
        std::vector<line_handle>
        rgbplot(const IterableIterables<T1> &colormap) {
            return rgbplot(to_vector_2d(colormap));
        }

        template <class T1, class T2, class T3>
        line_handle
        plot3(const IterableValues<T1> &x, const IterableValues<T2> &y,
              const IterableValues<T3> &z, std::string_view line_spec = "") {
            return plot3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                         line_spec);
        }

        template <class T1, class T2, class T3>
        std::enable_if_t<is_iterable_value_v<T3>, std::vector<line_handle>>
        plot3(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
              const IterableValues<T3> &z, std::string_view line_spec = "") {
            return plot3(to_vector_2d(X), to_vector_2d(Y), to_vector_1d(z),
                         line_spec);
        }

        template <class T1, class T2, class T3>
        std::enable_if_t<is_iterable_iterable_v<T3>, std::vector<line_handle>>
        plot3(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
              const IterableIterables<T3> &Z, std::string_view line_spec = "") {
            return plot3(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                         line_spec);
        }

        template <class T1, class T2, class T3, class... Args>
        auto plot3(const IterableValues<T1> &x, const IterableValues<T2> &y,
                   const IterableValues<T3> &z, std::string_view line_spec,
                   Args &&...args) {
            return plot3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                         line_spec, std::forward<Args>(args)...);
        }

        template <class T1, class T2, class T3, class T4, class... Args>
        auto plot3(const IterableValues<T1> &x, const IterableValues<T2> &y,
                   const IterableValues<T3> &z, const IterableValues<T4> &x2,
                   Args &&...args) {
            return plot3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                         to_vector_1d(x2), std::forward<Args>(args)...);
        }

        template <class T1, class... Args>
        auto plot3(const IterableValues<T1> &z, std::string_view line_spec,
                   Args &&...args) {
            return plot3(to_vector_1d(z), line_spec,
                         std::forward<Args>(args)...);
        }

        template <class T1, class T2>
        stair_handle stairs(const IterableValues<T1> &x,
                            const IterableValues<T2> &y,
                            std::string_view line_spec = "") {
            return stairs(to_vector_1d(x), to_vector_1d(y), line_spec);
        }

        template <class T1>
        stair_handle stairs(const IterableValues<T1> &y,
                            std::string_view line_spec = "") {
            return stairs(to_vector_1d(y), line_spec);
        }

        template <class T1, class T2>
        std::enable_if_t<is_iterable_value_v<T1>, std::vector<stair_handle>>
        stairs(const IterableValues<T1> &x, const IterableIterables<T2> &Y,
               std::string_view line_spec = "") {
            return stairs(to_vector_1d(x), to_vector_2d(Y), line_spec);
        }

        template <class T1, class T2>
        std::enable_if_t<is_iterable_iterable_v<T1>, std::vector<stair_handle>>
        stairs(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
               std::string_view line_spec = "") {
            return stairs(to_vector_2d(X), to_vector_2d(Y), line_spec);
        }

        template <class T1>
        std::vector<stair_handle> stairs(const IterableIterables<T1> &Y,
                                         std::string_view line_spec = "") {
            return stairs(to_vector_2d(Y), line_spec);
        }

        template <class T1, class T2, class... Args>
        auto stairs(const IterableValues<T1> &x, const IterableValues<T2> &y,
                    std::string_view line_spec, Args &&...args) {
            return stairs(to_vector_1d(x), to_vector_1d(y), line_spec,
                          std::forward<Args>(args)...);
        }

        template <class T1, class T2, class T3, class... Args>
        auto stairs(const IterableValues<T1> &x, const IterableValues<T2> &y,
                    const IterableValues<T3> &x2, Args &&...args) {
            return stairs(to_vector_1d(x), to_vector_1d(y), to_vector_1d(x2),
                          std::forward<Args>(args)...);
        }

        template <class T1, class... Args>
        auto stairs(const IterableValues<T1> &y, std::string_view line_spec,
                    Args &&...args) {
            return stairs(to_vector_1d(y), line_spec,
                          std::forward<Args>(args)...);
        }

        template <class T1, class T2, class T3>
        error_bar_handle
        errorbar(const IterableValues<T1> &x, const IterableValues<T2> &y,
                 const IterableValues<T3> &error,
                 error_bar::type type = error_bar::type::vertical,
                 std::string_view line_spec = "") {
            return errorbar(to_vector_1d(x), to_vector_1d(y),
                            to_vector_1d(error), type, line_spec);
        }

        template <class T1, class T2, class T3, class T4, class T5, class T6>
        error_bar_handle errorbar(const IterableValues<T1> &x,
                                  const IterableValues<T2> &y,
                                  const IterableValues<T3> &y_neg_delta,
                                  const IterableValues<T4> &y_pos_delta,
                                  const IterableValues<T5> &x_neg_delta,
                                  const IterableValues<T6> &x_pos_delta,
                                  std::string_view line_spec = "") {
            return errorbar(
                to_vector_1d(x), to_vector_1d(y), to_vector_1d(y_neg_delta),
                to_vector_1d(y_pos_delta), to_vector_1d(x_neg_delta),
                to_vector_1d(x_pos_delta), line_spec);
        }

        template <class T1, class T2, class T3>
        error_bar_handle errorbar(const IterableValues<T1> &x,
                                  const IterableValues<T2> &y,
                                  const IterableValues<T3> &y_error,
                                  std::string_view line_spec) {
            return errorbar(to_vector_1d(x), to_vector_1d(y),
                            to_vector_1d(y_error), line_spec);
        }

        template <class T1, class T2>
        std::vector<filled_area_handle>
        area(const IterableValues<T1> &x, const IterableIterables<T2> &Y,
             double base_value = 0., bool stacked = true,
             std::string_view line_spec = "k-") {
            return area(to_vector_1d(x), to_vector_2d(Y), base_value, stacked,
                        line_spec);
        }

        template <class T1, class T2>
        filled_area_handle area(const IterableValues<T1> &x,
                                const IterableValues<T2> &y,
                                double base_value = 0., bool stacked = true,
                                std::string_view line_spec = "k-") {
            return area(to_vector_1d(x), to_vector_1d(y), base_value, stacked,
                        line_spec);
        }

        template <class T1>
        std::vector<filled_area_handle>
        area(const IterableIterables<T1> &Y, double base_value = 0.,
             bool stacked = true, std::string_view line_spec = "k-") {
            return area(to_vector_2d(Y), base_value, stacked, line_spec);
        }

        template <class T1>
        std::vector<filled_area_handle>
        area(const IterableIterables<T1> &Y, bool stacked,
             std::string_view line_spec = "k-") {
            return area(to_vector_2d(Y), stacked, line_spec);
        }

        template <class T1>
        filled_area_handle area(const IterableValues<T1> &y,
                                double base_value = 0., bool stacked = true,
                                std::string_view line_spec = "k-") {
            return area(to_vector_1d(y), base_value, stacked, line_spec);
        }

        template <class T1>
        histogram_handle hist(const IterableValues<T1> &data,
                              histogram::binning_algorithm algorithm =
                                  histogram::binning_algorithm::automatic,
                              enum histogram::normalization normalization_alg =
                                  histogram::normalization::count) {
            return hist(to_vector_1d(data), algorithm, normalization_alg);
        }

        template <class T1>
        histogram_handle hist(const IterableValues<T1> &data, size_t n_bins) {
            return hist(to_vector_1d(data), n_bins);
        }

        template <class T1, class T2>
        histogram_handle hist(const IterableValues<T1> &data,
                              const IterableValues<T2> &edges) {
            return hist(to_vector_1d(data), to_vector_1d(edges));
        }

        template <class T1>
        histogram_handle hist(const IterableValues<T1> &data,
                              enum histogram::normalization normalization_alg) {
            return hist(to_vector_1d(data), normalization_alg);
        }

        template <class T1, class T2, class T3, class T4>
        axes_object_handle binscatter(
            const IterableValues<T1> &x, const IterableValues<T2> &y,
            const IterableValues<T3> &edges_x,
            const IterableValues<T4> &edges_y,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count) {
            return binscatter(to_vector_1d(x), to_vector_1d(y),
                              to_vector_1d(edges_x), to_vector_1d(edges_y),
                              scatter_style, normalization_alg);
        }

        template <class T1, class T2>
        axes_object_handle binscatter(
            const IterableValues<T1> &x, const IterableValues<T2> &y,
            histogram::binning_algorithm algorithm =
                histogram::binning_algorithm::automatic,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count) {
            return binscatter(to_vector_1d(x), to_vector_1d(y), algorithm,
                              scatter_style, normalization_alg);
        }

        template <class T1, class T2>
        axes_object_handle
        binscatter(const IterableValues<T1> &x, const IterableValues<T2> &y,
                   enum bin_scatter_style scatter_style,
                   histogram::binning_algorithm algorithm =
                       histogram::binning_algorithm::automatic,
                   enum histogram::normalization normalization_alg =
                       histogram::normalization::count) {
            return binscatter(to_vector_1d(x), to_vector_1d(y), scatter_style,
                              algorithm, normalization_alg);
        }

        template <class T1, class T2>
        axes_object_handle binscatter(
            const IterableValues<T1> &x, const IterableValues<T2> &y,
            size_t nbins_x, size_t nbins_y,
            enum bin_scatter_style scatter_style = bin_scatter_style::automatic,
            enum histogram::normalization normalization_alg =
                histogram::normalization::count) {
            return binscatter(to_vector_1d(x), to_vector_1d(y), nbins_x,
                              nbins_y, scatter_style, normalization_alg);
        }

        template <class T1>
        std::enable_if_t<is_iterable_value_v<T1>, box_chart_handle>
        boxplot(const IterableValues<T1> &data) {
            return boxplot(to_vector_1d(data));
        }

        template <class T1>
        std::enable_if_t<is_iterable_iterable_v<T1>, box_chart_handle>
        boxplot(const IterableIterables<T1> &data) {
            return boxplot(to_vector_2d(data));
        }

        template <class T1, class T2>
        box_chart_handle boxplot(const IterableValues<T1> &data,
                                 const IterableValues<T2> &groups) {
            return boxplot(to_vector_1d(data), to_vector_1d(groups));
        }

        template <class T1>
        box_chart_handle boxplot(const IterableValues<T1> &y_data,
                                 const std::vector<std::string> &groups) {
            return boxplot(to_vector_1d(y_data), groups);
        }

        template <class T1, class T2>
        std::enable_if_t<is_iterable_value_v<T2>, bars_handle>
        bar(const IterableValues<T1> &x, const IterableValues<T2> &y) {
            return bar(to_vector_1d(x), to_vector_1d(y));
        }

        template <class T1>
        std::enable_if_t<is_iterable_value_v<T1>, bars_handle>
        bar(const IterableValues<T1> &y) {
            return bar(to_vector_1d(y));
        }

        template <class T1>
        std::enable_if_t<is_iterable_iterable_v<T1>, bars_handle>
        bar(const IterableIterables<T1> &Y) {
            return bar(to_vector_2d(Y));
        }

        template <class T1, class T2>
        std::enable_if_t<is_iterable_iterable_v<T2>, bars_handle>
        bar(const IterableValues<T1> &x, const IterableIterables<T2> &Y) {
            return bar(to_vector_1d(x), to_vector_2d(Y));
        }

        template <class T1>
        bars_handle bar(const IterableValues<T1> &y, double width) {
            return bar(to_vector_1d(y), width);
        }

        template <class T1>
        std::vector<bars_handle> barstacked(const IterableIterables<T1> &Y) {
            return barstacked(to_vector_2d(Y));
        }

        template <class T1, class T2>
        std::vector<bars_handle> barstacked(const IterableValues<T1> &x,
                                            const IterableIterables<T2> &Y) {
            return barstacked(to_vector_1d(x), to_vector_2d(Y));
        }

        template <class T1>
        matrix_handle heatmap(const IterableIterables<T1> &m) {
            return heatmap(to_vector_2d(m));
        }

        template <class T1>
        matrix_handle pcolor(const IterableIterables<T1> &m) {
            return pcolor(to_vector_2d(m));
        }

        template <class T1, class T2>
        parallel_lines_handle parallelplot(const IterableIterables<T1> &X,
                                           const IterableValues<T2> &colors,
                                           std::string_view line_spec = "") {
            return parallelplot(to_vector_2d(X), to_vector_1d(colors),
                                line_spec);
        }

        template <class T1>
        parallel_lines_handle parallelplot(const IterableIterables<T1> &X,
                                           std::string_view line_spec = "") {
            return parallelplot(to_vector_2d(X), line_spec);
        }

        template <class T1, class T2>
        circles_handle pie(const IterableValues<T1> &x,
                           const IterableValues<T2> &explode = {},
                           const std::vector<std::string> &labels = {}) {
            return pie(to_vector_1d(x), to_vector_1d(explode), labels);
        }

        template <class T1, class T2>
        circles_handle pie(const IterableValues<T1> &x,
                           const std::vector<std::string> &labels) {
            return pie(to_vector_1d(x), labels);
        }

        template <class T1, class T2, class T3, class T4>
        line_handle scatter(const IterableValues<T1> &x,
                            const IterableValues<T2> &y,
                            const IterableValues<T3> &sizes = {},
                            const IterableValues<T4> &colors = {}) {
            return scatter(to_vector_1d(x), to_vector_1d(y),
                           to_vector_1d(sizes), to_vector_1d(colors));
        }

        template <class T1, class T2, class T3>
        line_handle scatter(const IterableValues<T1> &x,
                            const IterableValues<T2> &y, double sizes,
                            const IterableValues<T3> &colors = {}) {
            return scatter(to_vector_1d(x), to_vector_1d(y), sizes,
                           to_vector_1d(colors));
        }

        template <class T1, class T2, class T3, class T4, class T5>
        line_handle scatter3(const IterableValues<T1> &x,
                             const IterableValues<T2> &y,
                             const IterableValues<T3> &z,
                             const IterableValues<T4> &sizes = {},
                             const IterableValues<T5> &colors = {},
                             std::string_view line_spec = "o") {
            return scatter3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                            to_vector_1d(sizes), to_vector_1d(colors),
                            line_spec);
        }

        template <class T1, class T2, class T3>
        line_handle
        scatter3(const IterableValues<T1> &x, const IterableValues<T2> &y,
                 const IterableValues<T3> &z, std::string_view line_spec) {
            return scatter3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                            line_spec);
        }

        template <class T1, class T2>
        labels_handle wordcloud(const std::vector<std::string> &words,
                                const IterableValues<T1> &sizes,
                                const IterableValues<T2> &custom_colors = {}) {
            return wordcloud(words, to_vector_1d(sizes),
                             to_vector_1d(custom_colors));
        }

        template <class T1, class T2>
        labels_handle
        wordcloud(std::string_view text,
                  const std::vector<std::string> &black_list,
                  std::string_view delimiters = " ',\n\r\t\".!?:;",
                  size_t max_cloud_size = 100,
                  const IterableValues<T1> &custom_colors = {}) {
            return wordcloud(text, black_list, delimiters, max_cloud_size,
                             to_vector_1d(custom_colors));
        }

        template <class T1>
        std::pair<bars_handle, line_handle>
        pareto(const IterableValues<T1> &y,
               const std::vector<std::string> &names = {},
               double p_threshold = 0.95, size_t item_threshold = 10) {
            return pareto(to_vector_1d(y), names, p_threshold, item_threshold);
        }

        template <class T1, class T2>
        line_handle stem(const IterableValues<T1> &x,
                         const IterableValues<T2> &y,
                         std::string_view line_spec = "-o") {
            return stem(to_vector_1d(x), to_vector_1d(y), line_spec);
        }

        template <class T1>
        line_handle stem(const IterableValues<T1> &y,
                         std::string_view line_spec = "-o") {
            return stem(to_vector_1d(y), line_spec);
        }

        template <class T1, class T2>
        std::vector<line_handle> stem(const IterableValues<T1> &x,
                                      const IterableIterables<T2> &Y,
                                      std::string_view line_spec = "-o") {
            return stem(to_vector_1d(x), to_vector_2d(Y), line_spec);
        }

        template <class T1>
        std::vector<line_handle> stem(const IterableIterables<T1> &Y,
                                      std::string_view line_spec = "-o") {
            return stem(to_vector_2d(Y), line_spec);
        }

        template <class T1, class T2, class T3>
        line_handle
        stem3(const IterableValues<T1> &x, const IterableValues<T2> &y,
              const IterableValues<T3> &z, std::string_view line_spec = "-o") {
            return stem3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                         line_spec);
        }

        template <class T1, class T2, class T3>
        std::vector<line_handle>
        stem3(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
              const IterableValues<T3> &z, std::string_view line_spec = "-o") {
            return stem3(to_vector_2d(X), to_vector_2d(Y), to_vector_1d(z),
                         line_spec);
        }

        template <class T1>
        line_handle stem3(const IterableValues<T1> &z,
                          std::string_view line_spec = "-o") {
            return stem3(to_vector_1d(z), line_spec);
        }

        template <class T1, class T2>
        line_handle stem3(const IterableIterables<T1> &Z,
                          std::string_view line_spec = "-o") {
            return stem3(to_vector_2d(Z), line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        circles_handle geobubble(const IterableValues<T1> &latitude,
                                 const IterableValues<T2> &longitude,
                                 const IterableValues<T3> &sizes = {},
                                 const IterableValues<T4> &colors = {}) {
            return geobubble(to_vector_1d(latitude), to_vector_1d(longitude),
                             to_vector_1d(sizes), to_vector_1d(colors));
        }

        template <class T1, class T2, class T3>
        line_handle geodensityplot(const IterableValues<T1> &latitude,
                                   const IterableValues<T2> &longitude,
                                   const IterableValues<T3> &weights = {}) {
            return geodensityplot(to_vector_1d(latitude),
                                  to_vector_1d(longitude),
                                  to_vector_1d(weights));
        }

        template <class T1, class T2>
        line_handle geoplot(const IterableValues<T1> &latitude,
                            const IterableValues<T2> &longitude,
                            std::string_view line_spec = "") {
            return geoplot(to_vector_1d(latitude), to_vector_1d(longitude),
                           line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        line_handle geoscatter(const IterableValues<T1> &latitude,
                               const IterableValues<T2> &longitude,
                               const IterableValues<T3> &sizes = {},
                               const IterableValues<T4> &colors = {}) {
            return geoscatter(to_vector_1d(latitude), to_vector_1d(longitude),
                              to_vector_1d(sizes), to_vector_1d(colors));
        }

        template <class T1, class T2>
        vectors_handle compass(const IterableValues<T1> &x,
                               const IterableValues<T2> &y,
                               std::string_view line_spec = "") {
            return compass(to_vector_1d(x), to_vector_1d(y), line_spec);
        }

        template <class T1>
        histogram_handle polarhistogram(const IterableValues<T1> &theta,
                                        size_t nbins) {
            return polarhistogram(to_vector_1d(theta), nbins);
        }

        template <class T1, class T2>
        line_handle polarplot(const IterableValues<T1> &theta,
                              const IterableValues<T2> &rho,
                              std::string_view line_spec = "") {
            return polarplot(to_vector_1d(theta), to_vector_1d(rho), line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        line_handle
        polarscatter(const IterableValues<T1> &theta,
                     const IterableValues<T2> &rho,
                     const IterableValues<T3> &sizes = std::vector<double>{},
                     const IterableValues<T4> &colors = std::vector<double>{},
                     std::string_view line_spec = "o") {
            return polarscatter(to_vector_1d(theta), to_vector_1d(rho),
                                to_vector_1d(sizes), to_vector_1d(colors),
                                line_spec);
        }

        template <class T1, class T2>
        line_handle polarscatter(const IterableValues<T1> &theta,
                                 const IterableValues<T2> &rho, double size,
                                 std::string_view line_spec = "o") {
            return polarscatter(to_vector_1d(theta), to_vector_1d(rho), size,
                                line_spec);
        }

        template <class T1, class T2>
        line_handle polarscatter(const IterableValues<T1> &theta,
                                 const IterableValues<T2> &rho,
                                 std::string_view line_spec) {
            return polarscatter(to_vector_1d(theta), to_vector_1d(rho), size,
                                line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        contours_handle
        contour(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
                const IterableIterables<T3> &Z,
                const IterableValues<T4> &levels,
                std::string_view line_spec = "", size_t n_levels = 0) {
            return contour(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                           to_vector_1d(levels), line_spec, n_levels);
        }

        template <class T1, class T2, class T3>
        contours_handle
        contour(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
                const IterableIterables<T3> &Z, size_t n_levels = 0,
                std::string_view line_spec = "") {
            return contour(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                           n_levels, line_spec);
        }

        template <class T1, class T2, class T3>
        contours_handle
        contour(const IterableIterables<T1> &X, const IterableIterables<T1> &Y,
                const IterableIterables<T1> &Z, std::string_view line_spec) {
            return contour(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                           line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        contours_handle
        contourf(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
                 const IterableIterables<T3> &Z,
                 const IterableValues<T4> &levels,
                 std::string_view line_spec = "", size_t n_levels = 0) {
            return contourf(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                            to_vector_1d(levels), line_spec, n_levels);
        }

        template <class T1, class T2, class T3>
        contours_handle
        contourf(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
                 const IterableIterables<T3> &Z, size_t n_levels = 0,
                 std::string_view line_spec = "") {
            return contourf(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                            n_levels, line_spec);
        }

        template <class T1, class T2, class T3>
        contours_handle
        contourf(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
                 const IterableIterables<T3> &Z, std::string_view line_spec) {
            return contourf(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                            line_spec);
        }

        template <class T1>
        contours_handle fcontour(fcontour_function_type fn,
                                 const std::array<double, 4> &xy_range,
                                 IterableValues<T1> levels = {},
                                 std::string_view line_spec = "",
                                 size_t n_levels = 0) {
            return fcontour(fn, xy_range, to_vector_1d(levels), line_spec,
                            n_levels);
        }

        template <class T1, class T2>
        vectors_handle feather(const IterableValues<T1> &u,
                               const IterableValues<T2> &v,
                               std::string_view line_spec = "") {
            return feather(to_vector_1d(u), to_vector_1d(v), line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        std::enable_if_t<is_iterable_value_v<T1> && is_iterable_value_v<T2> &&
                             is_iterable_value_v<T3> && is_iterable_value_v<T4>,
                         vectors_handle>
        quiver(const IterableValues<T1> &x, const IterableValues<T2> &y,
               const IterableValues<T3> &u, const IterableValues<T4> &v,
               double scale = 1.0, std::string_view line_spec = "") {
            return quiver(to_vector_1d(x), to_vector_1d(y), to_vector_1d(u),
                          to_vector_1d(v), scale, line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        std::enable_if_t<
            is_iterable_iterable_v<T1> && is_iterable_iterable_v<T2> &&
                is_iterable_iterable_v<T3> && is_iterable_iterable_v<T4>,
            vectors_handle>
        quiver(const IterableIterables<T1> &x, const IterableIterables<T2> &y,
               const IterableIterables<T3> &u, const IterableIterables<T4> &v,
               double scale = 1.0, std::string_view line_spec = "") {
            return quiver(to_vector_2d(x), to_vector_2d(y), to_vector_2d(u),
                          to_vector_2d(v), scale, line_spec);
        }

        template <class T1, class T2, class T3, class T4, class T5, class T6>
        std::enable_if_t<is_iterable_value_v<T1> && is_iterable_value_v<T2> &&
                             is_iterable_value_v<T3> &&
                             is_iterable_value_v<T4> &&
                             is_iterable_value_v<T5> && is_iterable_value_v<T6>,
                         vectors_handle>
        quiver3(const IterableValues<T1> &x, const IterableValues<T2> &y,
                const IterableValues<T3> &z, const IterableValues<T4> &u,
                const IterableValues<T5> &v, const IterableValues<T6> &w,
                double scale = 1.0, std::string_view line_spec = "") {
            return quiver3(to_vector_1d(x), to_vector_1d(y), to_vector_1d(z),
                           to_vector_1d(u), to_vector_1d(v), to_vector_1d(w),
                           scale, line_spec);
        }

        template <class T1, class T2, class T3, class T4, class T5, class T6>
        std::enable_if_t<
            is_iterable_iterable_v<T1> && is_iterable_iterable_v<T2> &&
                is_iterable_iterable_v<T3> && is_iterable_iterable_v<T4> &&
                is_iterable_iterable_v<T5> && is_iterable_iterable_v<T6>,
            vectors_handle>
        quiver3(const IterableIterables<T1> &x, const IterableIterables<T2> &y,
                const IterableIterables<T3> &z, const IterableIterables<T4> &u,
                const IterableIterables<T5> &v, const IterableIterables<T6> &w,
                double scale = 1.0, std::string_view line_spec = "") {
            return quiver3(to_vector_2d(x), to_vector_2d(y), to_vector_2d(z),
                           to_vector_2d(u), to_vector_2d(v), to_vector_2d(w),
                           scale, line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        vectors_handle
        quiver3(const IterableIterables<T1> &z, const IterableIterables<T2> &u,
                const IterableIterables<T3> &v, const IterableIterables<T4> &w,
                double scale = 1.0, std::string_view line_spec = "") {
            return quiver3(to_vector_2d(z), to_vector_2d(u), to_vector_2d(v),
                           to_vector_2d(w), scale, line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle fence(const IterableIterables<T1> &X,
                             const IterableIterables<T2> &Y,
                             const IterableIterables<T3> &Z,
                             const IterableValues<T4> &c = {}) {
            return fence(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                         to_vector_1d(c));
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle mesh(const IterableIterables<T1> &X,
                            const IterableIterables<T2> &Y,
                            const IterableIterables<T3> &Z,
                            const IterableIterables<T4> &C = {}) {
            return mesh(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                        to_vector_2d(C));
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle meshc(const IterableIterables<T1> &X,
                             const IterableIterables<T2> &Y,
                             const IterableIterables<T3> &Z,
                             const IterableIterables<T4> &C = {}) {
            return meshc(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                         to_vector_2d(C));
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle meshz(const IterableIterables<T1> &X,
                             const IterableIterables<T2> &Y,
                             const IterableIterables<T3> &Z,
                             const IterableIterables<T4> &C = {}) {
            return meshz(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                         to_vector_2d(C));
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle
        ribbon(const IterableIterables<T1> &X, const IterableIterables<T2> &Y,
               const IterableIterables<T3> &Z,
               const IterableIterables<T4> &C = {}, double width = 0.75) {
            return ribbon(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                          to_vector_2d(C), width);
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle surf(const IterableIterables<T1> &X,
                            const IterableIterables<T2> &Y,
                            const IterableIterables<T3> &Z,
                            const IterableIterables<T4> &C = {},
                            std::string_view line_spec = "") {
            return IterableIterables<T1>(to_vector_2d(X), to_vector_2d(Y),
                                         to_vector_2d(Z), to_vector_2d(C),
                                         line_spec);
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle surfc(const IterableIterables<T1> &X,
                             const IterableIterables<T2> &Y,
                             const IterableIterables<T3> &Z,
                             const IterableIterables<T4> &C = {}) {
            return surfc(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                         to_vector_2d(C));
        }

        template <class T1, class T2, class T3, class T4>
        surface_handle waterfall(const IterableIterables<T1> &X,
                                 const IterableIterables<T2> &Y,
                                 const IterableIterables<T3> &Z,
                                 const IterableIterables<T4> &C = {}) {
            return waterfall(to_vector_2d(X), to_vector_2d(Y), to_vector_2d(Z),
                             to_vector_2d(C));
        }

        template <class T1, class T2>
        network_handle graph(const IterableValues<T1> &edges,
                             const IterableValues<T2> &weights = {},
                             size_t n_vertices = 0,
                             std::string_view line_spec = "-o") {
            return graph(to_vector_1d<T1, std::pair<size_t, size_t>>(edges),
                         to_vector_1d(weights), n_vertices, line_spec);
        }

        template <class T1>
        network_handle graph(const IterableValues<T1> &edges,
                             std::string_view line_spec) {
            return graph(to_vector_1d<T1, std::pair<size_t, size_t>>(edges),
                         line_spec);
        }

        template <class T1>
        matrix_handle imshow(const IterableIterables<T1> &gray_scale_img) {
            return imshow(to_vector_2d<T1, unsigned char>(gray_scale_img));
        }

        template <class T1, class T2, class T3, class T4>
        matrix_handle imshow(const IterableIterables<T1> &r_channel,
                             const IterableIterables<T2> &g_channel,
                             const IterableIterables<T3> &b_channel,
                             const IterableIterables<T4> &a_channel =
                                 std::vector<std::vector<unsigned char>>{}) {
            return imshow(to_vector_2d<T1, unsigned char>(r_channel),
                          to_vector_2d<T1, unsigned char>(g_channel),
                          to_vector_2d<T1, unsigned char>(b_channel),
                          to_vector_2d<T1, unsigned char>(a_channel));
        }

        template <class T1>
        matrix_handle imshow(const std::vector<IterableIterables<T1>> &img) {
            std::vector<std::vector<std::vector<unsigned char>>> v_img(
                img.size());
            for (size_t i = 0; i < img.size(); ++i) {
                v_img[i] = to_vector_2d<T1, unsigned char>(img[i]);
            }
            return imshow(v_img);
        }

        template <class T1>
        matrix_handle image(const IterableIterables<T1> &C,
                            bool scaled_colorbar = false) {
            return image(to_vector_2d(C), scaled_colorbar);
        }

        template <class T1, class T2, class T3>
        matrix_handle image(const IterableIterables<T1> &r_channel,
                            const IterableIterables<T2> &g_channel,
                            const IterableIterables<T3> &b_channel,
                            bool scaled_colorbar = false) {
            return image(to_vector_2d(r_channel), to_vector_2d(g_channel),
                         to_vector_2d(b_channel), scaled_colorbar);
        }

        template <class T1>
        matrix_handle image(double x_min, double x_max, double y_min,
                            double y_max, const IterableIterables<T1> &C,
                            bool scaled_colorbar = false) {
            return image(x_min, x_max, y_min, y_max, to_vector_2d(C),
                         scaled_colorbar);
        }

        template <class T1, class T2, class T3>
        matrix_handle
        image(const std::tuple<IterableIterables<T1>, IterableIterables<T2>,
                               IterableIterables<T3>> &img) {
            std::tuple<std::vector<std::vector<double>>,
                       std::vector<std::vector<double>>,
                       std::vector<std::vector<double>>>
                t = std::make_tuple(to_vector_2d(std::get<0>(img)),
                                    to_vector_2d(std::get<1>(img)),
                                    to_vector_2d(std::get<2>(img)));
            return image(t);
        }

        template <class T1, class T2>
        labels_handle text(const IterableValues<T1> &x,
                           const IterableValues<T2> &y,
                           const std::vector<std::string> &texts) {
            return text(to_vector_1d(x), to_vector_1d(y), texts);
        }

        template <class T1, class T2>
        labels_handle text(const IterableValues<T1> &x,
                           const IterableValues<T2> &y, std::string_view str) {
            return text(to_vector_1d(x), to_vector_1d(y), str);
        }

        template <class T1, class T2>
        line_handle fill(const IterableValues<T1> &x,
                         const IterableValues<T2> &y,
                         std::string_view line_spec = "") {
            return fill(to_vector_1d(x), to_vector_1d(y), line_spec);
        }

      public /* template variants of plot functions above  */:
        /// Loglog is a line plot variant with log on both x and y axes
        template <class... Args> auto loglog(Args &&...args) {
            auto h = this->plot(std::forward<Args>(args)...);
            this->x_axis().scale(axis_type::axis_scale::log);
            this->y_axis().scale(axis_type::axis_scale::log);
            return h;
        }

        /// Semilogx is a plot variant with log scale on the x axis
        template <class... Args> auto semilogx(Args &&...args) {
            auto h = this->plot(std::forward<Args>(args)...);
            this->x_axis().scale(axis_type::axis_scale::log);
            return h;
        }

        /// Semilogx is a plot variant with log scale on the x axis
        template <class... Args> auto semilogy(Args &&...args) {
            auto h = this->plot(std::forward<Args>(args)...);
            this->y_axis().scale(axis_type::axis_scale::log);
            return h;
        }

        /// If the user tries to use boxchart instead of boxplot, it also works
        template <class T1, class... Args> auto boxchart(Args &&...args) {
            return boxplot(std::forward<Args>(args)...);
        }

        /// The function hist2 is equivalent to binscatter (usually with a
        /// heatmap)
        template <class... Args> auto hist2(Args &&...args) {
            return binscatter(std::forward<Args>(args)...);
        }

        /// Directed graph
        template <class... Args> auto digraph(Args &&...args) {
            auto l = graph(std::forward<Args>(args)...);
            l->directed(true);
            return l;
        }

        /// Show array as image and scale the colorbar
        template <class... Args> auto imagesc(Args &&...args) {
            return image(std::forward<Args>(args)..., true);
        }

        /// Show array as image and scale the colorbar
        template <class... Args> auto polygon(Args &&...args) {
            return fill(std::forward<Args>(args)...);
        }

      public /* templates converting to vector<double> and
                vector<vector<double>> */
          :

      private /* run gnuplot commands */:
        void run_colormap_command();
        void run_position_margin_command();
        std::tuple<double, double, double, double, double, double>
        calculate_margins();
        void run_title_command();
        void run_box_command();
        void run_grid_command();
        void run_axes_command();
        void run_labels_command();
        void run_legend_command();
        void run_background_command();
        void run_plot_objects_command();
        void run_unset_objects_command();
        void run_empty_plot_command();

      private /* run draw commands */:
        void run_background_draw_commands();
        void run_title_draw_commands();
        void run_box_draw_commands();
        void run_grid_draw_commands();
        void run_axes_draw_commands();
        void run_labels_draw_commands();
        void run_legend_draw_commands();
        void run_plot_objects_draw_commands();

      public /* commands children objects can call to draw on the axes */:
        /*
         * These functions are intermediaries between the axes
         * and the backend because the axes need to clamp points
         * outside the axes and normalize the points to the axes
         * position on screen.
         */
        void draw_path(const std::vector<double> &x,
                       const std::vector<double> &y,
                       const std::array<float, 4> &color);

      private /* members */:
        // axes
        class axis_type x_axis_ {
            this, true
        };

        class axis_type x2_axis_ {
            this, false
        };

        class axis_type y_axis_ {
            this, true
        };

        class axis_type y2_axis_ {
            this, false
        };

        class axis_type z_axis_ {
            this, true
        };

        class axis_type r_axis_ {
            this, false
        };

        class axis_type t_axis_ {
            this, false
        };

        class axis_type cb_axis_ {
            this, false
        };

        bool cb_vertical_{true};
        bool cb_inside_{false};
        std::array<float, 4> cb_position_{0., 0., 0., 0.};

        // {x, y, width, height}
        std::array<float, 4> position_{default_axes_position};
        std::array<float, 4> margin_{0.0, 0.0, 0.0, 0.0};

        // 3d view
        float azimuth_ = -37.5;
        float elevation_ = 30.;

        // set xrange, yrange and zrange according to this ratio
        float axes_aspect_ratio_{1};
        bool axes_aspect_ratio_auto_{true};

        // text
        std::optional<float> font_size_{10.f};
        std::optional<std::string> font_{"Helvetica"};
        std::string font_weight_{"normal"};

        // title text
        std::string title_;
        bool title_visible_{false};
        bool title_enhanced_{true};
        float title_font_size_multiplier_{1.1f};
        std::string title_font_weight_{"bold"};
        color_array title_color_{0, 0, 0, 0};

        // colors for children object
        std::array<float, 4> color_{0, 1.0, 1.0, 1.0};
        std::array<float, 4> color_outside_{0, 0.97f, 0.97f, 0.97f};
        std::vector<std::array<float, 4>> colororder_{
            default_color(0), default_color(1), default_color(2),
            default_color(3), default_color(4), default_color(5),
            default_color(6)};
        size_t colororder_index_{0};
        std::vector<std::vector<double>> colormap_ = {palette::default_map()};
        size_t max_colors_{0}; // limit number of colors in the colormap

        // complete box around the axes
        bool box_{true};
        bool box_full_{false};
        std::array<float, 4> box_color_{.0, .0, .0, .0};

        // grid inside the axes
        line_spec grid_line_style_{"-"};
        line_spec minor_grid_line_style_{"--"};
        bool grid_front_{false};

        bool x_grid_{false};
        bool x_user_grid_{false};
        bool x_minor_grid_{false};

        bool y_grid_{false};
        bool y_user_grid_{false};
        bool y_minor_grid_{false};

        bool z_grid_{false};
        bool z_user_grid_{false};
        bool z_minor_grid_{false};

        bool r_grid_{false};
        bool r_user_grid_{false};
        bool r_minor_grid_{false};
        bool t_minor_grid_{false};

        // background
        bool visible_{true};
        bool handle_visibility_{true};
        float line_width_{0.5};

        // line styles for children objects that don't have
        // a color defined by the user
        std::string line_style_order_ = "-";
        size_t line_style_order_index_{0};

        // children objects
        std::vector<axes_object_handle> children_;
        bool next_plot_replace_ = true;
        legend_handle legend_;

        // parent figure
        // we don't need a shared_ptr here because there is no
        // relationship of ownership
        class figure_type *parent_;
    };

} // namespace matplot

#endif // MATPLOTPLUSPLUS_AXES_TYPE_H
