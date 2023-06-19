//
// Created by Alan Freitas on 10/08/20.
//

#ifndef MATPLOTPLUSPLUS_AXES_FUNCTIONS_H
#define MATPLOTPLUSPLUS_AXES_FUNCTIONS_H

#include <matplot/detail/config.h>
#include <matplot/core/axes_type.h>

namespace matplot {
    using axes_handle = std::shared_ptr<class axes_type>;

    /// \brief Create new axes
    MATPLOT_EXPORTS
    axes_handle axes();

    /// \brief Create new axes
    MATPLOT_EXPORTS
    axes_handle axes(figure_handle parent, bool replace_if_overlap = false);

    /// \brief Set the current axes
    MATPLOT_EXPORTS
    axes_handle axes(axes_handle h);

    /// \brief Put axes in parent
    MATPLOT_EXPORTS
    axes_handle axes(axes_handle h, figure_handle parent,
                     bool replace_if_overlap = false);

    /// \brief Create new axes in a specific position with replace = false
    MATPLOT_EXPORTS
    axes_handle axes(std::array<float, 4> position);

    MATPLOT_EXPORTS
    void hold(bool);
    MATPLOT_EXPORTS
    void hold(axes_handle, bool);
    MATPLOT_EXPORTS
    void hold(const std::vector<axes_handle> &axs, bool);

    MATPLOT_EXPORTS
    void box(bool);
    MATPLOT_EXPORTS
    void box(axes_handle, bool);

    /// Set view with azimuth and elevation
    MATPLOT_EXPORTS
    void view(float azimuth, float elevation);
    MATPLOT_EXPORTS
    void view(axes_handle ax, float azimuth, float elevation);

    /// Set view with azimuth and elevation
    MATPLOT_EXPORTS
    void rotate(float azimuth = 10., float elevation = 0.);
    MATPLOT_EXPORTS
    void rotate(axes_handle ax, float azimuth = 10., float elevation = 0.);

    /// Coordinates of a vector that starts at the center of the
    /// plot box and points toward the camera (line-of-sight vector)
    MATPLOT_EXPORTS
    std::pair<float, float> view(float x, float y, float z);
    MATPLOT_EXPORTS
    std::pair<float, float> view(axes_handle ax, float x, float y, float z);

    /// Default line of sight for dim-D plots
    /// If dim=2 "set view map"
    /// If dim=3 set view to default
    MATPLOT_EXPORTS
    void view(size_t dim);
    MATPLOT_EXPORTS
    void view(axes_handle ax, size_t dim);

    /// Get view values
    MATPLOT_EXPORTS
    std::pair<float, float> view();
    MATPLOT_EXPORTS
    std::pair<float, float> view(axes_handle ax);

    /// \brief Create new axes in a subplot
    /// If the axes exist, make it the current axes.
    /// If the axes overlap with existing axes, these are deleted,
    /// unless the position is identical.
    MATPLOT_EXPORTS
    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        bool replace_if_same_position = false);
    MATPLOT_EXPORTS
    axes_handle subplot(figure_handle, size_t rows, size_t cols, size_t plot_id,
                        bool replace_if_same_position = false);

    /// \brief Make axes a subplot of its figure
    MATPLOT_EXPORTS
    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        axes_handle axes);

    /// \brief Create new axes in a subplot covering all positions in P
    MATPLOT_EXPORTS
    axes_handle subplot(size_t rows, size_t cols,
                        std::initializer_list<size_t> P,
                        bool replace_if_same_position = false);

    /// \brief Create new axes in a specific position
    MATPLOT_EXPORTS
    axes_handle subplot(figure_handle f, std::array<float, 4> position,
                        bool replace_if_same_position = false);
    MATPLOT_EXPORTS
    axes_handle subplot(std::array<float, 4> position,
                        bool replace_if_same_position = false);

    /// \brief Make axes the current axes
    /// Another way to axes(h)
    MATPLOT_EXPORTS
    axes_handle subplot(axes_handle h);

    /// \brief Make axes in a tiled layout
    /// This is only very basic support for tiled
    /// layouts. It does not include a tiledlayout
    /// class we can use to adjust padding, spacing,
    /// title, etc.
    MATPLOT_EXPORTS
    void tiledlayout();
    MATPLOT_EXPORTS
    void tiledlayout(size_t rows, size_t cols);
    MATPLOT_EXPORTS
    axes_handle nexttile();
    MATPLOT_EXPORTS
    axes_handle nexttile(size_t index);

    MATPLOT_EXPORTS
    void title(std::string_view str);
    MATPLOT_EXPORTS
    void title(std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void title(std::string_view str, COLOR_TYPE c) {
        title(str, to_array(c));
    }

    MATPLOT_EXPORTS
    void title(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void title(axes_handle ax, std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void title(axes_handle ax, std::string_view str, COLOR_TYPE c) {
        title(ax, str, to_array(c));
    }

    MATPLOT_EXPORTS
    void title(legend_handle lgd, std::string_view str);

    MATPLOT_EXPORTS
    void sgtitle(std::string_view str);
    MATPLOT_EXPORTS
    void sgtitle(std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void sgtitle(std::string_view str, COLOR_TYPE c) {
        sgtitle(str, to_array(c));
    }

    MATPLOT_EXPORTS
    void sgtitle(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void sgtitle(axes_handle ax, std::string_view str, const color_array &c);

    template <class COLOR_TYPE>
    void sgtitle(axes_handle ax, std::string_view str, COLOR_TYPE c) {
        sgtitle(ax, str, to_array(c));
    }

    MATPLOT_EXPORTS
    void xlabel(std::string_view str);
    MATPLOT_EXPORTS
    void xlabel(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void ylabel(std::string_view str);
    MATPLOT_EXPORTS
    void ylabel(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void y2label(std::string_view str);
    MATPLOT_EXPORTS
    void y2label(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void zlabel(std::string_view str);
    MATPLOT_EXPORTS
    void zlabel(axes_handle ax, std::string_view str);

    MATPLOT_EXPORTS
    void xtickformat(std::string_view str);
    MATPLOT_EXPORTS
    void xtickformat(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void ytickformat(std::string_view str);
    MATPLOT_EXPORTS
    void ytickformat(axes_handle ax, std::string_view str);
    MATPLOT_EXPORTS
    void ztickformat(std::string_view str);
    MATPLOT_EXPORTS
    void ztickformat(axes_handle ax, std::string_view str);

    MATPLOT_EXPORTS
    std::string xtickformat();
    MATPLOT_EXPORTS
    std::string xtickformat(axes_handle ax);
    MATPLOT_EXPORTS
    std::string ytickformat();
    MATPLOT_EXPORTS
    std::string ytickformat(axes_handle ax);
    MATPLOT_EXPORTS
    std::string ztickformat();
    MATPLOT_EXPORTS
    std::string ztickformat(axes_handle ax);

    MATPLOT_EXPORTS
    void xticks(const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void xticks(axes_handle ax, const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void yticks(const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void yticks(axes_handle ax, const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void y2ticks(const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void y2ticks(axes_handle ax, const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void zticks(const std::vector<double> &ticks);
    MATPLOT_EXPORTS
    void zticks(axes_handle ax, const std::vector<double> &ticks);

    MATPLOT_EXPORTS
    void xticks(std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void xticks(axes_handle ax, std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void yticks(std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void yticks(axes_handle ax, std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void y2ticks(std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void y2ticks(axes_handle ax, std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void zticks(std::initializer_list<double> ticks);
    MATPLOT_EXPORTS
    void zticks(axes_handle ax, std::initializer_list<double> ticks);

    MATPLOT_EXPORTS
    void xticks(keyword_automatic_type);
    MATPLOT_EXPORTS
    void xticks(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void yticks(keyword_automatic_type);
    MATPLOT_EXPORTS
    void yticks(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void y2ticks(keyword_automatic_type);
    MATPLOT_EXPORTS
    void y2ticks(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void zticks(keyword_automatic_type);
    MATPLOT_EXPORTS
    void zticks(axes_handle ax, keyword_automatic_type);

    MATPLOT_EXPORTS
    void xticklabels(const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void xticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void yticklabels(const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void yticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void y2ticklabels(const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void y2ticklabels(axes_handle ax, const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void zticklabels(const std::vector<std::string> &ticks);
    MATPLOT_EXPORTS
    void zticklabels(axes_handle ax, const std::vector<std::string> &ticks);

    MATPLOT_EXPORTS
    void xticklabels(std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void xticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void yticklabels(std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void yticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void y2ticklabels(std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void y2ticklabels(axes_handle ax, std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void zticklabels(std::initializer_list<std::string> ticks);
    MATPLOT_EXPORTS
    void zticklabels(axes_handle ax, std::initializer_list<std::string> ticks);

    MATPLOT_EXPORTS
    void xticklabels(keyword_automatic_type);
    MATPLOT_EXPORTS
    void xticklabels(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void yticklabels(keyword_automatic_type);
    MATPLOT_EXPORTS
    void yticklabels(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void y2ticklabels(keyword_automatic_type);
    MATPLOT_EXPORTS
    void y2ticklabels(axes_handle ax, keyword_automatic_type);
    MATPLOT_EXPORTS
    void zticklabels(keyword_automatic_type);
    MATPLOT_EXPORTS
    void zticklabels(axes_handle ax, keyword_automatic_type);

    MATPLOT_EXPORTS
    void xtickangle(double degrees);
    MATPLOT_EXPORTS
    void xtickangle(axes_handle ax, double degrees);
    MATPLOT_EXPORTS
    void ytickangle(double degrees);
    MATPLOT_EXPORTS
    void ytickangle(axes_handle ax, double degrees);
    MATPLOT_EXPORTS
    void y2tickangle(double degrees);
    MATPLOT_EXPORTS
    void y2tickangle(axes_handle ax, double degrees);
    MATPLOT_EXPORTS
    void ztickangle(double degrees);
    MATPLOT_EXPORTS
    void ztickangle(axes_handle ax, double degrees);

    MATPLOT_EXPORTS
    double xtickangle();
    MATPLOT_EXPORTS
    double xtickangle(axes_handle ax);
    MATPLOT_EXPORTS
    double ytickangle();
    MATPLOT_EXPORTS
    double ytickangle(axes_handle ax);
    MATPLOT_EXPORTS
    double y2tickangle();
    MATPLOT_EXPORTS
    double y2tickangle(axes_handle ax);
    MATPLOT_EXPORTS
    double ztickangle();
    MATPLOT_EXPORTS
    double ztickangle(axes_handle ax);

    MATPLOT_EXPORTS
    void grid(axes_handle ax, bool v);
    MATPLOT_EXPORTS
    void grid(bool v);

    /// \brief Get the current axes
    MATPLOT_EXPORTS
    axes_handle gca();

    MATPLOT_EXPORTS
    void cla();
    MATPLOT_EXPORTS
    void cla(axes_handle);

    /// Put legends on the figure
    MATPLOT_EXPORTS
    legend_handle legend(axes_handle ax, const std::vector<std::string> &names);
    MATPLOT_EXPORTS
    legend_handle legend(const std::vector<std::string> &names);

    MATPLOT_EXPORTS
    legend_handle legend(axes_handle ax, bool visible = true);
    MATPLOT_EXPORTS
    legend_handle legend(bool visible = true);

    MATPLOT_EXPORTS
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

    MATPLOT_EXPORTS
    void colormap(axes_handle ax, const std::vector<std::vector<double>> &map);
    MATPLOT_EXPORTS
    void colormap(const std::vector<std::vector<double>> &map);

    MATPLOT_EXPORTS
    std::vector<std::vector<double>> colormap(axes_handle ax);
    MATPLOT_EXPORTS
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
    MATPLOT_EXPORTS
    void xrange(const std::array<double, 2> &range);
    MATPLOT_EXPORTS
    void xrange(axes_handle ah, const std::array<double, 2> &range);

    /// Set y limits
    MATPLOT_EXPORTS
    void yrange(const std::array<double, 2> &range);
    MATPLOT_EXPORTS
    void yrange(axes_handle ah, const std::array<double, 2> &range);

    /// Enable colorbar axis
    MATPLOT_EXPORTS
    axis_type &colorbar();
    MATPLOT_EXPORTS
    axis_type &colorbar(axes_handle ah);
    MATPLOT_EXPORTS
    class axis_type &colorbar(bool v);
    MATPLOT_EXPORTS
    class axis_type &colorbar(axes_handle ah, bool v);

    /// Set limits of x and y axes
    MATPLOT_EXPORTS
    void axis(const std::array<double, 4> &limits_x_y);
    MATPLOT_EXPORTS
    void axis(axes_handle axes_handle, const std::array<double, 4> &limits_x_y);
    MATPLOT_EXPORTS
    void axis(std::initializer_list<axes_handle> axes_handles,
              const std::array<double, 4> &limits_x_y);

    /// Make axis visible invisible
    MATPLOT_EXPORTS
    void axis(bool);

    /// Set limits to automatic
    MATPLOT_EXPORTS
    void axis(keyword_automatic_type);

    /// Set limits to manual
    MATPLOT_EXPORTS
    void axis(keyword_manual_type);

    /// Reverse y-axis
    MATPLOT_EXPORTS
    void axis(keyword_ij_type);

    /// Make xlim equal size (ratio -1)
    MATPLOT_EXPORTS
    void axis(keyword_equal_type);
    MATPLOT_EXPORTS
    void axis(axes_handle, keyword_equal_type);

    /// Make xlim equal size (ratio -1)
    MATPLOT_EXPORTS
    void axis(keyword_tight_type);
    MATPLOT_EXPORTS
    void axis(axes_handle, keyword_tight_type);

    /// Make xlim equal size (ratio 1)
    MATPLOT_EXPORTS
    void axis(keyword_square_type);
    MATPLOT_EXPORTS
    void axis(axes_handle, keyword_square_type);
} // namespace matplot

#endif // MATPLOTPLUSPLUS_AXES_FUNCTIONS_H
