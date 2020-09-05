//
// Created by Alan Freitas on 10/08/20.
//

#ifndef MATPLOTPLUSPLUS_AXES_FUNCTIONS_H
#define MATPLOTPLUSPLUS_AXES_FUNCTIONS_H

#include <matplot/core/axes_type.h>

namespace matplot {
    using axes_handle = std::shared_ptr<class axes_type>;

    /// \brief Create new axes
    axes_handle axes();

    /// \brief Create new axes
    axes_handle axes(figure_handle parent, bool replace_if_overlap = false);

    /// \brief Set the current axes
    axes_handle axes(axes_handle h);

    /// \brief Put axes in parent
    axes_handle axes(axes_handle h, figure_handle parent,
                     bool replace_if_overlap = false);

    /// \brief Create new axes in a specific position with replace = false
    axes_handle axes(std::array<float, 4> position);

    void hold(bool);
    void hold(axes_handle, bool);
    void hold(const std::vector<axes_handle> &axs, bool);

    void box(bool);
    void box(axes_handle, bool);

    /// Set view with azimuth and elevation
    void view(float azimuth, float elevation);
    void view(axes_handle ax, float azimuth, float elevation);

    /// Set view with azimuth and elevation
    void rotate(float azimuth = 10., float elevation = 0.);
    void rotate(axes_handle ax, float azimuth = 10., float elevation = 0.);

    /// Coordinates of a vector that starts at the center of the
    /// plot box and points toward the camera (line-of-sight vector)
    std::pair<float, float> view(float x, float y, float z);
    std::pair<float, float> view(axes_handle ax, float x, float y, float z);

    /// Default line of sight for dim-D plots
    /// If dim=2 "set view map"
    /// If dim=3 set view to default
    void view(size_t dim);
    void view(axes_handle ax, size_t dim);

    /// Get view values
    std::pair<float, float> view();
    std::pair<float, float> view(axes_handle ax);

    /// \brief Create new axes in a subplot
    /// If the axes exist, make it the current axes.
    /// If the axes overlap with existing axes, these are deleted,
    /// unless the position is identical.
    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        bool replace_if_same_position = false);
    axes_handle subplot(figure_handle, size_t rows, size_t cols, size_t plot_id,
                        bool replace_if_same_position = false);

    /// \brief Make axes a subplot of its figure
    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        axes_handle axes);

    /// \brief Create new axes in a subplot covering all positions in P
    axes_handle subplot(size_t rows, size_t cols,
                        std::initializer_list<size_t> P,
                        bool replace_if_same_position = false);

    /// \brief Create new axes in a specific position
    axes_handle subplot(figure_handle f, std::array<float, 4> position,
                        bool replace_if_same_position = false);
    axes_handle subplot(std::array<float, 4> position,
                        bool replace_if_same_position = false);

    /// \brief Make axes the current axes
    /// Another way to axes(h)
    axes_handle subplot(axes_handle h);

    /// \brief Make axes in a tiled layout
    /// This is only very basic support for tiled
    /// layouts. It does not include a tiledlayout
    /// class we can use to adjust padding, spacing,
    /// title, etc.
    void tiledlayout();
    void tiledlayout(size_t rows, size_t cols);
    axes_handle nexttile();
    axes_handle nexttile(size_t index);

    void title(std::string_view str);
    void title(std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void title(std::string_view str, COLOR_TYPE c) {
        title(str, to_array(c));
    }

    void title(axes_handle ax, std::string_view str);
    void title(axes_handle ax, std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void title(axes_handle ax, std::string_view str, COLOR_TYPE c) {
        title(ax, str, to_array(c));
    }

    void title(legend_handle lgd, std::string_view str);

    void sgtitle(std::string_view str);
    void sgtitle(std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void sgtitle(std::string_view str, COLOR_TYPE c) {
        sgtitle(str, to_array(c));
    }

    void sgtitle(axes_handle ax, std::string_view str);
    void sgtitle(axes_handle ax, std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void sgtitle(axes_handle ax, std::string_view str, COLOR_TYPE c) {
        sgtitle(ax, str, to_array(c));
    }

    void xlabel(std::string_view str);
    void xlabel(axes_handle ax, std::string_view str);
    void ylabel(std::string_view str);
    void ylabel(axes_handle ax, std::string_view str);
    void y2label(std::string_view str);
    void y2label(axes_handle ax, std::string_view str);
    void zlabel(std::string_view str);
    void zlabel(axes_handle ax, std::string_view str);

    void xtickformat(std::string_view str);
    void xtickformat(axes_handle ax, std::string_view str);
    void ytickformat(std::string_view str);
    void ytickformat(axes_handle ax, std::string_view str);
    void ztickformat(std::string_view str);
    void ztickformat(axes_handle ax, std::string_view str);

    std::string xtickformat();
    std::string xtickformat(axes_handle ax);
    std::string ytickformat();
    std::string ytickformat(axes_handle ax);
    std::string ztickformat();
    std::string ztickformat(axes_handle ax);

    void xticks(const std::vector<double> &ticks);
    void xticks(axes_handle ax, const std::vector<double> &ticks);
    void yticks(const std::vector<double> &ticks);
    void yticks(axes_handle ax, const std::vector<double> &ticks);
    void y2ticks(const std::vector<double> &ticks);
    void y2ticks(axes_handle ax, const std::vector<double> &ticks);
    void zticks(const std::vector<double> &ticks);
    void zticks(axes_handle ax, const std::vector<double> &ticks);

    void xticks(std::initializer_list<double> ticks);
    void xticks(axes_handle ax, std::initializer_list<double> ticks);
    void yticks(std::initializer_list<double> ticks);
    void yticks(axes_handle ax, std::initializer_list<double> ticks);
    void y2ticks(std::initializer_list<double> ticks);
    void y2ticks(axes_handle ax, std::initializer_list<double> ticks);
    void zticks(std::initializer_list<double> ticks);
    void zticks(axes_handle ax, std::initializer_list<double> ticks);

    void xticks(keyword_automatic_type);
    void xticks(axes_handle ax, keyword_automatic_type);
    void yticks(keyword_automatic_type);
    void yticks(axes_handle ax, keyword_automatic_type);
    void y2ticks(keyword_automatic_type);
    void y2ticks(axes_handle ax, keyword_automatic_type);
    void zticks(keyword_automatic_type);
    void zticks(axes_handle ax, keyword_automatic_type);

    void xticklabels(const std::vector<std::string> &ticks);
    void xticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    void yticklabels(const std::vector<std::string> &ticks);
    void yticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    void y2ticklabels(const std::vector<std::string> &ticks);
    void y2ticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    void zticklabels(const std::vector<std::string> &ticks);
    void zticklabels(axes_handle ax, const std::vector<std::string> &ticks);

    void xticklabels(std::initializer_list<std::string> ticks);
    void xticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    void yticklabels(std::initializer_list<std::string> ticks);
    void yticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    void y2ticklabels(std::initializer_list<std::string> ticks);
    void y2ticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    void zticklabels(std::initializer_list<std::string> ticks);
    void zticklabels(axes_handle ax, std::initializer_list<std::string> ticks);

    void xticklabels(keyword_automatic_type);
    void xticklabels(axes_handle ax, keyword_automatic_type);
    void yticklabels(keyword_automatic_type);
    void yticklabels(axes_handle ax, keyword_automatic_type);
    void y2ticklabels(keyword_automatic_type);
    void y2ticklabels(axes_handle ax, keyword_automatic_type);
    void zticklabels(keyword_automatic_type);
    void zticklabels(axes_handle ax, keyword_automatic_type);

    void xtickangle(double degrees);
    void xtickangle(axes_handle ax, double degrees);
    void ytickangle(double degrees);
    void ytickangle(axes_handle ax, double degrees);
    void y2tickangle(double degrees);
    void y2tickangle(axes_handle ax, double degrees);
    void ztickangle(double degrees);
    void ztickangle(axes_handle ax, double degrees);

    double xtickangle();
    double xtickangle(axes_handle ax);
    double ytickangle();
    double ytickangle(axes_handle ax);
    double y2tickangle();
    double y2tickangle(axes_handle ax);
    double ztickangle();
    double ztickangle(axes_handle ax);

    void grid(axes_handle ax, bool v);
    void grid(bool v);

    /// \brief Get the current axes
    axes_handle gca();

    void cla();
    void cla(axes_handle);

    /// Put legends on the figure
    legend_handle legend(axes_handle ax, const std::vector<std::string> &names);
    legend_handle legend(const std::vector<std::string> &names);

    legend_handle legend(axes_handle ax, bool visible = true);
    legend_handle legend(bool visible = true);

    legend_handle legend(std::vector<axes_object_handle> objs,
                         const std::vector<std::string> &names);

    // Hackfix for a compiler bug in MSVC
    namespace {
        template <typename... Args>
        legend_handle legend(axes_handle ax, std::string_view name,
                             Args const &... next_name) {
            std::vector<std::string> legends = {std::string(name), std::string(next_name)...};
            return ::matplot::legend(ax, legends);
        }

        template <typename... Args>
        legend_handle legend(std::string_view name,
                             Args const &... next_name) {
            return legend(gca(), name, next_name...);
        }
    } // namespace

    void colormap(axes_handle ax, const std::vector<std::vector<double>> &map);
    void colormap(const std::vector<std::vector<double>> &map);

    std::vector<std::vector<double>> colormap(axes_handle ax);
    std::vector<std::vector<double>> colormap();

    template <class T>
    void colororder(axes_handle ax, const std::vector<T> &order) {
        ax->colororder(order);
    }

    template <class T> void colororder(const std::vector<T> &order) {
        colororder(gca(), order);
    }

    template <class T> void colororder(std::initializer_list<T> order) {
        colororder(gca(), std::vector<T>{order});
    }

    /// Set x limits
    void xrange(const std::array<double, 2> &range);
    void xrange(axes_handle ah, const std::array<double, 2> &range);

    /// Set y limits
    void yrange(const std::array<double, 2> &range);
    void yrange(axes_handle ah, const std::array<double, 2> &range);

    /// Enable colorbar axis
    axis_type &colorbar();
    axis_type &colorbar(axes_handle ah);
    class axis_type &colorbar(bool v);
    class axis_type &colorbar(axes_handle ah, bool v);

    /// Set limits of x and y axes
    void axis(const std::array<double, 4> &limits_x_y);
    void axis(axes_handle axes_handle, const std::array<double, 4> &limits_x_y);
    void axis(std::initializer_list<axes_handle> axes_handles,
              const std::array<double, 4> &limits_x_y);

    /// Make axis visible invisible
    void axis(bool);

    /// Set limits to automatic
    void axis(keyword_automatic_type);

    /// Set limits to manual
    void axis(keyword_manual_type);

    /// Reverse y-axis
    void axis(keyword_ij_type);

    /// Make xlim equal size (ratio -1)
    void axis(keyword_equal_type);
    void axis(axes_handle, keyword_equal_type);

    /// Make xlim equal size (ratio -1)
    void axis(keyword_tight_type);
    void axis(axes_handle, keyword_tight_type);

    /// Make xlim equal size (ratio 1)
    void axis(keyword_square_type);
    void axis(axes_handle, keyword_square_type);
} // namespace matplot

#endif // MATPLOTPLUSPLUS_AXES_FUNCTIONS_H
