//
// Created by Alan Freitas on 10/08/20.
//

#include <algorithm>
#include <matplot/core/figure_registry.h>
#include <matplot/freestanding/axes_functions.h>

namespace matplot {
    /// \brief Create a new axes in the current figure
    axes_handle axes() {
        figure_handle h = gcf();
        return axes(h, false);
    }

    /// \brief Create new axes in a figure
    axes_handle axes(figure_handle parent, bool replace_if_overlap) {
        axes_handle h = std::make_shared<class axes_type>(parent);
        return axes(h, parent, replace_if_overlap);
    }

    /// \brief Emplace axes in the figure
    axes_handle axes(axes_handle h, figure_handle parent,
                     bool replace_if_overlap) {
        return parent->add_axes(h, replace_if_overlap, replace_if_overlap);
    }

    /// \brief Set the current axes as current axes in its figure
    axes_handle axes(axes_handle h) {
        // set axes' parent figure as current figure
        auto cf = h->parent();
        if (cf != nullptr) {
            cf->current_axes(h);
        }
        return h;
    }

    /// \brief Create axes in the position in the current figure
    axes_handle axes(std::array<float, 4> position) {
        auto parent = gcf();
        return parent->add_axes(position);
    }

    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        bool replace_if_same_position) {
        return subplot(gcf(), rows, cols, plot_id, replace_if_same_position);
    }

    axes_handle subplot(figure_handle f, size_t rows, size_t cols,
                        size_t plot_id, bool replace_if_same_position) {
        return f->add_subplot(rows, cols, plot_id, replace_if_same_position);
    }

    axes_handle subplot(size_t rows, size_t cols, size_t plot_id,
                        axes_handle ax) {
        return gcf()->add_subplot(rows, cols, plot_id, ax);
    }

    axes_handle subplot(size_t rows, size_t cols,
                        std::initializer_list<size_t> P,
                        bool replace_if_same_position) {
        return gcf()->add_subplot(rows, cols, P, replace_if_same_position);
    }

    axes_handle subplot(figure_handle parent, std::array<float, 4> position,
                        bool replace_if_same_position) {
        return parent->add_subplot(position, replace_if_same_position);
    }

    axes_handle subplot(std::array<float, 4> position,
                        bool replace_if_same_position) {
        return gcf()->add_subplot(position, replace_if_same_position);
    }

    axes_handle subplot(axes_handle h) { return axes(h); }

    void tiledlayout() { gcf()->tiledlayout(); }

    void tiledlayout(size_t rows, size_t cols) {
        gcf()->tiledlayout(rows, cols);
    }

    void tiledlayout(size_t rows) { gcf()->tiledlayout(rows, 1); }

    axes_handle nexttile() { return gcf()->nexttile(); }

    axes_handle nexttile(size_t index) { return gcf()->nexttile(index); }

    /// \brief Get the current axes
    axes_handle gca() { return gcf()->current_axes(); }

    void cla(axes_handle ax) { ax->clear(); }

    void cla() { cla(gca()); }

    legend_handle legend(axes_handle ax,
                         const std::vector<std::string> &names) {
        ax->legend(names);
        return ax->legend();
    }

    legend_handle legend(const std::vector<std::string> &names) {
        return legend(gca(), names);
    }

    legend_handle legend(axes_handle ax, bool v) {
        if (ax->legend() != nullptr) {
            ax->legend()->visible(v);
        } else {
            legend(ax, std::vector<std::string>{});
        }
        return ax->legend();
    }

    legend_handle legend(bool visible) { return legend(gca(), visible); }

    legend_handle legend(std::vector<axes_object_handle> objs,
                         const std::vector<std::string> &names) {
        size_t n = min(objs.size(), names.size());
        for (size_t i = 0; i < n; ++i) {
            objs[i]->display_name(names[i]);
        }
        return legend();
    }

    void hold(bool v) { gca()->next_plot_replace(!v); }

    void hold(axes_handle ax, bool v) { ax->next_plot_replace(!v); }

    void hold(const std::vector<axes_handle> &axs, bool v) {
        for (auto &ax : axs) {
            hold(ax, v);
        }
    }

    void box(bool v) { gca()->box(v); }

    void box(axes_handle ax, bool v) { ax->box(v); }

    void view(float azimuth, float elevation) {
        view(gca(), azimuth, elevation);
    }

    void view(axes_handle ax, float azimuth, float elevation) {
        ax->view(azimuth, elevation);
    }

    void rotate(float azimuth, float elevation) {
        rotate(gca(), azimuth, elevation);
    }

    void rotate(axes_handle ax, float azimuth, float elevation) {
        ax->rotate(azimuth, elevation);
    }

    std::pair<float, float> view(float x, float y, float z) {
        return view(gca(), x, y, z);
    }

    std::pair<float, float> view(axes_handle ax, float x, float y, float z) {
        float norm = sqrt(x * x + y * y + z * z);
        float unit_x = x / norm;
        float unit_y = y / norm;
        float unit_z = z / norm;
        float az = std::atan2(unit_x, -unit_y) * 180.f / static_cast<float>(pi);
        float el = std::atan2(unit_z, sqrt(std::pow(unit_x, 2.f) + std::pow(unit_y, 2.f))) *
                   180.f / static_cast<float>(pi);
        view(ax, az, el);
        return std::make_pair(az, el);
    }

    void view(size_t dim) { view(gca(), dim); }

    void view(axes_handle ax, size_t dim) {
        if (dim == 2) {
            view(ax, 0, 90);
        } else {
            view(ax, -37.5, 30);
        }
    }

    std::pair<float, float> view() { return view(gca()); }

    std::pair<float, float> view(axes_handle ax) { return ax->view(); }

    void title(std::string_view str) { title(gca(), str); }

    void title(axes_handle ax, std::string_view str) { ax->title(str); }

    void title(std::string_view str, const color_array &c) {
        auto ax = gca();
        title(ax, str);
        ax->title_color(c);
    }

    void title(axes_handle ax, std::string_view str, const color_array &c) {
        title(ax, str);
        ax->title_color(c);
    }

    void title(legend_handle lgd, std::string_view str) { lgd->title(str); }

    void sgtitle(std::string_view str) { sgtitle(gca(), str); }

    void sgtitle(axes_handle ax, std::string_view str) {
        ax->parent()->title(str);
    }

    void sgtitle(std::string_view str, const color_array &c) {
        auto ax = gca();
        sgtitle(ax, str);
        ax->parent()->title_color(c);
    }

    void sgtitle(axes_handle ax, std::string_view str, const color_array &c) {
        sgtitle(ax, str);
        ax->parent()->title_color(c);
    }

    void xlabel(std::string_view str) { xlabel(gca(), str); }

    void xlabel(axes_handle ax, std::string_view str) { ax->xlabel(str); }

    void ylabel(std::string_view str) { ylabel(gca(), str); }

    void ylabel(axes_handle ax, std::string_view str) { ax->ylabel(str); }

    void y2label(std::string_view str) { y2label(gca(), str); }

    void y2label(axes_handle ax, std::string_view str) { ax->y2label(str); }

    void zlabel(std::string_view str) { zlabel(gca(), str); }

    void zlabel(axes_handle ax, std::string_view str) { ax->zlabel(str); }

    void xtickformat(std::string_view str) { xtickformat(gca(), str); }

    void xtickformat(axes_handle ax, std::string_view str) {
        ax->xtickformat(str);
    }

    void ytickformat(std::string_view str) { ytickformat(gca(), str); }

    void ytickformat(axes_handle ax, std::string_view str) {
        ax->ytickformat(str);
    }

    void ztickformat(std::string_view str) { ztickformat(gca(), str); }

    void ztickformat(axes_handle ax, std::string_view str) {
        ax->ztickformat(str);
    }

    std::string xtickformat(axes_handle ax) { return ax->xtickformat(); }

    std::string ytickformat(axes_handle ax) { return ax->ytickformat(); }

    std::string xtickformat() { return xtickformat(gca()); }

    std::string ytickformat() { return ytickformat(gca()); }

    std::string ztickformat() { return ztickformat(gca()); }

    std::string ztickformat(axes_handle ax) { return ax->ztickformat(); }

    void xticks(const std::vector<double> &ticks) { xticks(gca(), ticks); }

    void xticks(axes_handle ax, const std::vector<double> &ticks) {
        ax->xticks(ticks);
    }

    void yticks(const std::vector<double> &ticks) { yticks(gca(), ticks); }

    void yticks(axes_handle ax, const std::vector<double> &ticks) {
        ax->yticks(ticks);
    }

    void y2ticks(const std::vector<double> &ticks) { y2ticks(gca(), ticks); }

    void y2ticks(axes_handle ax, const std::vector<double> &ticks) {
        ax->y2ticks(ticks);
    }

    void zticks(const std::vector<double> &ticks) { zticks(gca(), ticks); }

    void zticks(axes_handle ax, const std::vector<double> &ticks) {
        ax->zticks(ticks);
    }

    void xticks(std::initializer_list<double> ticks) {
        xticks(std::vector<double>(ticks));
    }

    void xticks(axes_handle ax, std::initializer_list<double> ticks) {
        xticks(ax, std::vector<double>(ticks));
    }

    void yticks(std::initializer_list<double> ticks) {
        yticks(std::vector<double>(ticks));
    }

    void yticks(axes_handle ax, std::initializer_list<double> ticks) {
        yticks(ax, std::vector<double>(ticks));
    }

    void y2ticks(std::initializer_list<double> ticks) {
        y2ticks(std::vector<double>(ticks));
    }

    void y2ticks(axes_handle ax, std::initializer_list<double> ticks) {
        y2ticks(ax, std::vector<double>(ticks));
    }

    void zticks(std::initializer_list<double> ticks) {
        zticks(std::vector<double>(ticks));
    }

    void zticks(axes_handle ax, std::initializer_list<double> ticks) {
        zticks(ax, std::vector<double>(ticks));
    }

    void xticks(axes_handle ax, keyword_automatic_type) {
        ax->x_axis().tick_values_automatic(true);
    }

    void yticks(axes_handle ax, keyword_automatic_type) {
        ax->y_axis().tick_values_automatic(true);
    }

    void y2ticks(axes_handle ax, keyword_automatic_type) {
        ax->y2_axis().tick_values_automatic(true);
    }

    void zticks(axes_handle ax, keyword_automatic_type) {
        ax->z_axis().tick_values_automatic(true);
    }

    void xticks(keyword_automatic_type) { xticks(gca(), automatic); }

    void yticks(keyword_automatic_type) { yticks(gca(), automatic); }

    void y2ticks(keyword_automatic_type) { y2ticks(gca(), automatic); }

    void zticks(keyword_automatic_type) { zticks(gca(), automatic); }

    void xticklabels(const std::vector<std::string> &ticks) {
        xticklabels(gca(), ticks);
    }

    void xticklabels(axes_handle ax, const std::vector<std::string> &ticks) {
        ax->xticklabels(ticks);
    }

    void yticklabels(const std::vector<std::string> &ticks) {
        yticklabels(gca(), ticks);
    }

    void yticklabels(axes_handle ax, const std::vector<std::string> &ticks) {
        ax->yticklabels(ticks);
    }

    void y2ticklabels(const std::vector<std::string> &ticks) {
        y2ticklabels(gca(), ticks);
    }

    void y2ticklabels(axes_handle ax, const std::vector<std::string> &ticks) {
        ax->y2ticklabels(ticks);
    }

    void zticklabels(const std::vector<std::string> &ticks) {
        zticklabels(gca(), ticks);
    }

    void zticklabels(axes_handle ax, const std::vector<std::string> &ticks) {
        ax->zticklabels(ticks);
    }

    void xticklabels(std::initializer_list<std::string> ticks) {
        xticklabels(std::vector<std::string>(ticks));
    }

    void xticklabels(axes_handle ax, std::initializer_list<std::string> ticks) {
        xticklabels(ax, std::vector<std::string>(ticks));
    }

    void yticklabels(std::initializer_list<std::string> ticks) {
        yticklabels(std::vector<std::string>(ticks));
    }

    void yticklabels(axes_handle ax, std::initializer_list<std::string> ticks) {
        yticklabels(ax, std::vector<std::string>(ticks));
    }

    void y2ticklabels(std::initializer_list<std::string> ticks) {
        y2ticklabels(std::vector<std::string>(ticks));
    }

    void y2ticklabels(axes_handle ax,
                      std::initializer_list<std::string> ticks) {
        y2ticklabels(ax, std::vector<std::string>(ticks));
    }

    void zticklabels(std::initializer_list<std::string> ticks) {
        zticklabels(std::vector<std::string>(ticks));
    }

    void zticklabels(axes_handle ax, std::initializer_list<std::string> ticks) {
        zticklabels(ax, std::vector<std::string>(ticks));
    }

    void xticklabels(axes_handle ax, keyword_automatic_type) {
        ax->x_axis().ticklabels_mode(false);
    }

    void yticklabels(axes_handle ax, keyword_automatic_type) {
        ax->y_axis().ticklabels_mode(false);
    }

    void y2ticklabels(axes_handle ax, keyword_automatic_type) {
        ax->y2_axis().ticklabels_mode(false);
    }

    void zticklabels(axes_handle ax, keyword_automatic_type) {
        ax->z_axis().ticklabels_mode(false);
    }

    void xticklabels(keyword_automatic_type) { xticklabels(gca(), automatic); }

    void yticklabels(keyword_automatic_type) { yticklabels(gca(), automatic); }

    void y2ticklabels(keyword_automatic_type) {
        y2ticklabels(gca(), automatic);
    }

    void zticklabels(keyword_automatic_type) { zticklabels(gca(), automatic); }

    void xtickangle(axes_handle ax, double degrees) { ax->xtickangle(degrees); }

    void ytickangle(axes_handle ax, double degrees) { ax->ytickangle(degrees); }

    void y2tickangle(axes_handle ax, double degrees) {
        ax->y2tickangle(degrees);
    }

    void ztickangle(axes_handle ax, double degrees) { ax->ztickangle(degrees); }

    void xtickangle(double degrees) { xtickangle(gca(), degrees); }

    void ytickangle(double degrees) { ytickangle(gca(), degrees); }

    void y2tickangle(double degrees) { y2tickangle(gca(), degrees); }

    void ztickangle(double degrees) { ztickangle(gca(), degrees); }

    double xtickangle(axes_handle ax) { return ax->xtickangle(); }

    double ytickangle(axes_handle ax) { return ax->ytickangle(); }

    double y2tickangle(axes_handle ax) { return ax->y2tickangle(); }

    double ztickangle(axes_handle ax) { return ax->ztickangle(); }

    double xtickangle() { return xtickangle(gca()); }
    double ytickangle() { return ytickangle(gca()); }
    double y2tickangle() { return y2tickangle(gca()); }
    double ztickangle() { return ztickangle(gca()); }

    void grid(axes_handle ax, bool v) { ax->grid(v); }

    void grid(bool v) { grid(gca(), v); }

    void colormap(axes_handle ax, const std::vector<std::vector<double>> &map) {
        ax->colormap(map);
    }

    void colormap(const std::vector<std::vector<double>> &map) {
        colormap(gca(), map);
    }

    std::vector<std::vector<double>> colormap(axes_handle ax) {
        return ax->colormap();
    }

    std::vector<std::vector<double>> colormap() { return colormap(gca()); }

    /// \brief Sets limits on current axes
    void axis(const std::array<double, 4> &limits_x_y) {
        axis(gca(), limits_x_y);
    }

    /// \brief Sets limits on given axes
    void axis(axes_handle ah, const std::array<double, 4> &limits_x_y) {
        ah->x_axis().limits({limits_x_y[0], limits_x_y[1]});
        ah->x_axis().limits_mode_auto(false);
        ah->y_axis().limits({limits_x_y[2], limits_x_y[3]});
        ah->y_axis().limits_mode_auto(false);
        ah->touch();
    }

    void axis(std::initializer_list<axes_handle> handles,
              const std::array<double, 4> &limits_x_y) {
        for (const auto &h : handles) {
            axis(h, limits_x_y);
        }
    }

    /// \brief Sets limits on current axes
    void xrange(const std::array<double, 2> &limits_x_y) {
        xrange(gca(), limits_x_y);
    }

    /// \brief Sets limits on given axes
    void xrange(axes_handle ah, const std::array<double, 2> &limits_x) {
        ah->x_axis().limits({limits_x[0], limits_x[1]});
        ah->x_axis().limits_mode_auto(false);
        ah->touch();
    }

    /// \brief Sets limits on current axes
    void yrange(const std::array<double, 2> &limits_x_y) {
        yrange(gca(), limits_x_y);
    }

    /// \brief Sets limits on given axes
    void yrange(axes_handle ah, const std::array<double, 2> &limits_y) {
        ah->y_axis().limits({limits_y[0], limits_y[1]});
        ah->y_axis().limits_mode_auto(false);
        ah->touch();
    }

    class axis_type &colorbar() {
        return colorbar(gca());
    }

    class axis_type &colorbar(axes_handle ah) {
        ah->color_box(true);
        return ah->cb_axis();
    }

    class axis_type &colorbar(bool v) {
        return colorbar(gca(), v);
    }

    class axis_type &colorbar(axes_handle ah, bool v) {
        ah->color_box(v);
        return ah->cb_axis();
    }

    void axis(bool v) {
        axes_handle ax = gca();
        ax->visible(v);
        if (!v) {
            ax->title_visible(true);
        }
    }

    void axis(keyword_automatic_type automatic_keywork) {
        axes_handle ax = gca();
        ax->limits_mode(automatic_keywork);
    }

    void axis(keyword_manual_type manual_keyword) {
        axes_handle ax = gca();
        ax->limits_mode(manual_keyword);
    }

    void axis(keyword_ij_type) {
        axes_handle ax = gca();
        ax->y_axis().reverse(true);
    }

    void axis(keyword_equal_type) { axis(gca(), equal); }

    void axis(axes_handle ax, keyword_equal_type) {
        if (ax->children().empty()) {
            return;
        }
        float w = ax->width() * ax->parent()->width();
        float h = ax->height() * ax->parent()->height();
        auto [xmin, xmax, ymin, ymax] = ax->child_limits();
        double xrange = xmax - xmin;
        double yrange = ymax - ymin;
        // new ranges that could make it proportional
        // new xrange = w / factor that makes current xrange fit in there
        // w / factor == xrange -> factor = w / xrange
        double new_xrange = w * (yrange / h);
        double new_yrange = h * (xrange / w);
        if (new_xrange >= xrange) {
            double size_increase = (new_xrange - xrange) / 2;
            ax->x_axis().limits({xmin - size_increase, xmax + size_increase});
            ax->y_axis().limits({ymin, ymax});
        } else if (new_yrange >= yrange) {
            double size_increase = (new_yrange - yrange) / 2;
            ax->x_axis().limits({xmin, xmax});
            ax->y_axis().limits({ymin - size_increase, ymax + size_increase});
        }
    }

    void axis(keyword_tight_type) { axis(gca(), tight); }

    void axis(axes_handle ax, keyword_tight_type) {
        if (ax->children().empty()) {
            return;
        }
        auto [xmin, xmax, ymin, ymax] = ax->child_limits();
        ax->x_axis().limits({xmin, xmax});
        ax->y_axis().limits({ymin, ymax});
        if (ax->is_3d() && !ax->is_3d_map()) {
            auto zminit = std::min_element(
                ax->children().begin(), ax->children().end(),
                [](axes_object_handle a, axes_object_handle b) {
                    return a->zmin() < b->zmin();
                });
            auto zmaxit = std::max_element(
                ax->children().begin(), ax->children().end(),
                [](axes_object_handle a, axes_object_handle b) {
                    return a->zmax() < b->zmax();
                });
            ax->z_axis().limits({(*zminit)->zmin(), (*zmaxit)->zmax()});
        }
    }

    void axis(keyword_square_type) { axis(gca(), square); }

    void axis(axes_handle ax, keyword_square_type) {
        axis(ax, equal);
        float w = ax->width() * ax->parent()->width();
        float h = ax->height() * ax->parent()->height();
        // make w = h
        if (w > h) {
            double old_width = ax->width();
            double new_width = h / ax->parent()->width();
            ax->width(static_cast<float>(new_width));
            ax->x_origin(ax->x_origin() +
                         (static_cast<float>(old_width - new_width)) / 2.f);
        } else if (h > w) {
            double old_height = ax->height();
            double new_height = h / ax->parent()->height();
            ax->height(static_cast<float>(new_height));
            ax->y_origin(ax->y_origin() +
                         (static_cast<float>(old_height - new_height)) / 2.f);
        }
    }

} // namespace matplot