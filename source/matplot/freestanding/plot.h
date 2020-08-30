//
// Created by Alan Freitas on 2020-07-08.
//

#ifndef MATPLOTPLUSPLUS_PLOT_H
#define MATPLOTPLUSPLUS_PLOT_H

#include <matplot/util/concepts.h>
#include <matplot/util/handle_types.h>

#include <matplot/core/axes.h>

#include <matplot/axes_objects/function_line.h>
#include <matplot/axes_objects/string_function.h>

#include <matplot/freestanding/axes_functions.h>

/**
 * These are the freestanding plot functions
 * These functions will call the corresponding functions in the axes handle
 * If no axes handle is given, it will call the function in the current
 *     axes handle in the current figure.
 * If there is no current axes handle, one is created in the current figure.
 * If there is no current figure, one is created.
 *
 * They always forward the arguments as a parameter pack class ...Args.
 * One problem with this is that the functions cannot infer what to do with
 *     initializer lists. So one needs to pass an explicit vector or array to
 *     the functions. In some cases, we explicitly define these shortcut
 *     functions without templates to make the use of initializer lists
 * possible.
 */
namespace matplot {
    template <class T1, class... Args>
    auto plot(NotAxesHandle<T1> x, Args... args) {
        return gca()->plot(x, args...);
    }

    template <class... Args> auto plot(axes_handle ax, Args... args) {
        return ax->plot(args...);
    }

    inline line_handle plot(const std::vector<double> &x,
                            const std::vector<double> &y,
                            const std::string &line_spec = "") {
        return gca()->plot(x, y, line_spec);
    }

    inline line_handle plot(const std::vector<double> &y,
                            const std::string &line_spec = "") {
        return gca()->plot(y, line_spec);
    }

    template <class... Args>
    auto plot(const std::vector<double> &x, const std::vector<double> &y,
              const std::string &line_spec, Args... args) {
        return gca()->plot(x, y, line_spec, args...);
    }

    template <class... Args>
    auto plot(const std::vector<double> &x, const std::vector<double> &y,
              const std::vector<double> &x2, Args... args) {
        return gca()->plot(x, y, x2, args...);
    }

    template <class... Args>
    auto plot(const std::vector<double> &y, const std::string &line_spec,
              Args... args) {
        return gca()->plot(y, line_spec, args...);
    }

    inline line_handle plot(axes_handle ax, const std::vector<double> &x,
                            const std::vector<double> &y,
                            const std::string &line_spec = "") {
        return ax->plot(x, y, line_spec);
    }

    inline line_handle plot(axes_handle ax, const std::vector<double> &y,
                            const std::string &line_spec = "") {
        return ax->plot(y, line_spec);
    }

    template <class... Args>
    auto plot(axes_handle ax, const std::vector<double> &x,
              const std::vector<double> &y, const std::string &line_spec,
              Args... args) {
        return ax->plot(x, y, line_spec, args...);
    }

    template <class... Args>
    auto plot(axes_handle ax, const std::vector<double> &x,
              const std::vector<double> &y, const std::vector<double> &x2,
              Args... args) {
        return ax->plot(x, y, x2, args...);
    }

    template <class... Args>
    auto plot(axes_handle ax, const std::vector<double> &y,
              const std::string &line_spec, Args... args) {
        return ax->plot(y, line_spec, args...);
    }

    template <class T1, class... Args>
    auto loglog(NotAxesHandle<T1> x, Args... args) {
        return gca()->loglog(x, args...);
    }

    template <class... Args> auto loglog(axes_handle ax, Args... args) {
        return ax->loglog(args...);
    }

    template <class T1, class... Args>
    auto semilogx(NotAxesHandle<T1> x, Args... args) {
        return gca()->semilogx(x, args...);
    }

    template <class... Args> auto semilogx(axes_handle ax, Args... args) {
        return ax->semilogx(args...);
    }

    template <class T1, class... Args>
    auto semilogy(NotAxesHandle<T1> x, Args... args) {
        return gca()->semilogy(x, args...);
    }

    template <class... Args> auto semilogy(axes_handle ax, Args... args) {
        return ax->semilogy(args...);
    }

    template <class T1, class... Args>
    auto rgbplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->rgbplot(x, args...);
    }

    template <class... Args> auto rgbplot(axes_handle ax, Args... args) {
        return ax->rgbplot(args...);
    }

    template <class T1, class... Args>
    auto plot3(NotAxesHandle<T1> x, Args... args) {
        return gca()->plot3(x, args...);
    }

    template <class... Args> auto plot3(axes_handle ax, Args... args) {
        return ax->plot3(args...);
    }

    template <class T1, class... Args>
    auto stairs(NotAxesHandle<T1> x, Args... args) {
        return gca()->stairs(x, args...);
    }

    template <class... Args> auto stairs(axes_handle ax, Args... args) {
        return ax->stairs(args...);
    }

    template <class T1, class... Args>
    auto errorbar(NotAxesHandle<T1> x, Args... args) {
        return gca()->errorbar(x, args...);
    }

    template <class... Args> auto errorbar(axes_handle ax, Args... args) {
        return ax->errorbar(args...);
    }

    template <class T1, class... Args>
    auto area(NotAxesHandle<T1> x, Args... args) {
        return gca()->area(x, args...);
    }

    template <class... Args> auto area(axes_handle ax, Args... args) {
        return ax->area(args...);
    }

    template <class T1, class... Args>
    auto fplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->fplot(x, args...);
    }

    template <class... Args> auto fplot(axes_handle ax, Args... args) {
        return ax->fplot(args...);
    }

    template <class T1, class... Args>
    auto fimplicit(NotAxesHandle<T1> x, Args... args) {
        return gca()->fimplicit(x, args...);
    }

    template <class... Args> auto fimplicit(axes_handle ax, Args... args) {
        return ax->fimplicit(args...);
    }

    template <class T1, class... Args>
    auto fplot3(NotAxesHandle<T1> x, Args... args) {
        return gca()->fplot3(x, args...);
    }

    template <class... Args> auto fplot3(axes_handle ax, Args... args) {
        return ax->fplot3(args...);
    }

    template <class T1, class... Args>
    auto hist(NotAxesHandle<T1> x, Args... args) {
        return gca()->hist(x, args...);
    }

    template <class... Args> auto hist(axes_handle ax, Args... args) {
        return ax->hist(args...);
    }

    inline size_t morebins(histogram_handle h, double bin_increase = 0.1) {
        return h->morebins(bin_increase);
    }

    inline size_t fewerbins(histogram_handle h, double bin_decrease = 0.1) {
        return h->fewerbins(bin_decrease);
    }

    template <class T1, class... Args>
    auto binscatter(NotAxesHandle<T1> x, Args... args) {
        return gca()->binscatter(x, args...);
    }

    template <class... Args> auto binscatter(axes_handle ax, Args... args) {
        return ax->binscatter(args...);
    }

    template <class T1, class... Args>
    auto hist2(NotAxesHandle<T1> x, Args... args) {
        return gca()->hist2(x, args...);
    }

    template <class... Args> auto hist2(axes_handle ax, Args... args) {
        return ax->hist2(args...);
    }

    template <class T1, class... Args>
    auto bar(NotAxesHandle<T1> x, Args... args) {
        return gca()->bar(x, args...);
    }

    template <class... Args> auto bar(axes_handle ax, Args... args) {
        return ax->bar(args...);
    }

    template <class T1, class... Args>
    auto barstacked(NotAxesHandle<T1> x, Args... args) {
        return gca()->barstacked(x, args...);
    }

    template <class... Args> auto barstacked(axes_handle ax, Args... args) {
        return ax->barstacked(args...);
    }

    template <class T1, class... Args>
    auto heatmap(NotAxesHandle<T1> x, Args... args) {
        return gca()->heatmap(x, args...);
    }

    template <class... Args> auto heatmap(axes_handle ax, Args... args) {
        return ax->heatmap(args...);
    }

    template <class T1, class... Args>
    auto pcolor(NotAxesHandle<T1> x, Args... args) {
        return gca()->pcolor(x, args...);
    }

    template <class... Args> auto pcolor(axes_handle ax, Args... args) {
        return ax->pcolor(args...);
    }

    template <class T1, class... Args>
    auto parallelplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->parallelplot(x, args...);
    }

    template <class... Args> auto parallelplot(axes_handle ax, Args... args) {
        return ax->parallelplot(args...);
    }

    template <class T1, class... Args>
    auto pie(NotAxesHandle<T1> x, Args... args) {
        return gca()->pie(x, args...);
    }

    template <class... Args> auto pie(axes_handle ax, Args... args) {
        return ax->pie(args...);
    }

    template <class T1, class... Args>
    auto plotmatrix(NotFigureHandle<T1> x, Args... args) {
        return gcf()->plotmatrix(x, args...);
    }

    template <class... Args> auto plotmatrix(figure_handle f, Args... args) {
        return f->plotmatrix(args...);
    }

    template <class T1, class... Args>
    auto scatter(NotAxesHandle<T1> x, Args... args) {
        return gca()->scatter(x, args...);
    }

    template <class... Args> auto scatter(axes_handle ax, Args... args) {
        return ax->scatter(args...);
    }

    template <class T1, class... Args>
    auto scatter3(NotAxesHandle<T1> x, Args... args) {
        return gca()->scatter3(x, args...);
    }

    template <class... Args> auto scatter3(axes_handle ax, Args... args) {
        return ax->scatter3(args...);
    }

    template <class T1, class... Args>
    auto wordcloud(NotAxesHandle<T1> x, Args... args) {
        return gca()->wordcloud(x, args...);
    }

    template <class... Args> auto wordcloud(axes_handle ax, Args... args) {
        return ax->wordcloud(args...);
    }

    template <class T1, class... Args>
    auto pareto(NotAxesHandle<T1> x, Args... args) {
        return gca()->pareto(x, args...);
    }

    template <class... Args> auto pareto(axes_handle ax, Args... args) {
        return ax->pareto(args...);
    }

    template <class T1, class... Args>
    auto stem(NotAxesHandle<T1> x, Args... args) {
        return gca()->stem(x, args...);
    }

    template <class... Args> auto stem(axes_handle ax, Args... args) {
        return ax->stem(args...);
    }

    template <class T1, class... Args>
    auto stem3(NotAxesHandle<T1> x, Args... args) {
        return gca()->stem3(x, args...);
    }

    template <class... Args> auto stem3(axes_handle ax, Args... args) {
        return ax->stem3(args...);
    }

    template <class T1, class... Args>
    auto geobubble(NotAxesHandle<T1> x, Args... args) {
        return gca()->geobubble(x, args...);
    }

    template <class... Args> auto geobubble(axes_handle ax, Args... args) {
        return ax->geobubble(args...);
    }

    template <class T1, class... Args>
    auto geodensityplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->geodensityplot(x, args...);
    }

    template <class... Args> auto geodensityplot(axes_handle ax, Args... args) {
        return ax->geodensityplot(args...);
    }

    template <class T1, class... Args>
    auto geoplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->geoplot(x, args...);
    }

    template <class... Args> auto geoplot(axes_handle ax, Args... args) {
        return ax->geoplot(args...);
    }

    inline auto geoplot() { return gca()->geoplot(); }

    template <class T1, class... Args>
    void geolimits(NotAxesHandle<T1> x, Args... args) {
        gca()->geolimits(x, args...);
    }

    template <class... Args> void geolimits(axes_handle ax, Args... args) {
        ax->geolimits(args...);
    }

    inline void geolimits(const std::array<double, 2> &latitude,
                          const std::array<double, 2> &longitude) {
        gca()->geolimits(latitude, longitude);
    }

    inline void geolimits(axes_handle ax, const std::array<double, 2> &latitude,
                          const std::array<double, 2> &longitude) {
        ax->geolimits(latitude, longitude);
    }

    template <class T1, class... Args>
    auto geoscatter(NotAxesHandle<T1> x, Args... args) {
        return gca()->geoscatter(x, args...);
    }

    template <class... Args> auto geoscatter(axes_handle ax, Args... args) {
        return ax->geoscatter(args...);
    }

    template <class T1, class... Args>
    auto compass(NotAxesHandle<T1> x, Args... args) {
        return gca()->compass(x, args...);
    }

    template <class... Args> auto compass(axes_handle ax, Args... args) {
        return ax->compass(args...);
    }

    template <class T1, class... Args>
    auto ezpolar(NotAxesHandle<T1> x, Args... args) {
        return gca()->ezpolar(x, args...);
    }

    template <class... Args> auto ezpolar(axes_handle ax, Args... args) {
        return ax->ezpolar(args...);
    }

    template <class T1, class... Args>
    auto polarhistogram(NotAxesHandle<T1> x, Args... args) {
        return gca()->polarhistogram(x, args...);
    }

    template <class... Args> auto polarhistogram(axes_handle ax, Args... args) {
        return ax->polarhistogram(args...);
    }

    template <class T1, class... Args>
    auto polarplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->polarplot(x, args...);
    }

    template <class... Args> auto polarplot(axes_handle ax, Args... args) {
        return ax->polarplot(args...);
    }

    template <class T1, class... Args>
    auto polarscatter(NotAxesHandle<T1> x, Args... args) {
        return gca()->polarscatter(x, args...);
    }

    template <class... Args> auto polarscatter(axes_handle ax, Args... args) {
        return ax->polarscatter(args...);
    }

    template <class... Args> auto contour(axes_handle ax, Args... args) {
        return ax->contour(args...);
    }

    template <class T1, class T2, class T3>
    auto contour(axes_handle ax, T1 v1, T2 v2, T3 v3,
                 const std::vector<double> &il) {
        return ax->contour(v1, v2, v3, il);
    }

    template <class T1, class... Args>
    auto contour(NotAxesHandle<T1> x, Args... args) {
        return contour(gca(), x, args...);
    }

    template <class... Args> auto contourf(axes_handle ax, Args... args) {
        return ax->contourf(args...);
    }

    template <class T1, class T2, class T3>
    auto contourf(axes_handle ax, T1 v1, T2 v2, T3 v3,
                  const std::vector<double> &il) {
        return ax->contourf(v1, v2, v3, il);
    }

    template <class T1, class T2, class T3>
    auto contourf(T1 v1, T2 v2, T3 v3, const std::vector<double> &il) {
        return gca()->contourf(v1, v2, v3, il);
    }

    template <class T1, class... Args>
    auto contourf(NotAxesHandle<T1> x, Args... args) {
        return contourf(gca(), x, args...);
    }

    template <class... Args> auto fcontour(axes_handle ax, Args... args) {
        return ax->fcontour(args...);
    }

    template <class T1, class... Args>
    auto fcontour(NotAxesHandle<T1> x, Args... args) {
        return gca()->fcontour(x, args...);
    }

    inline contours_handle fcontour(axes::fcontour_function_type fn,
                                    const std::array<double, 4> &xy_range,
                                    std::vector<double> levels,
                                    const std::string &line_spec = "") {
        return gca()->fcontour(fn, xy_range, levels, line_spec);
    }

    inline contours_handle fcontour(axes::fcontour_function_type fn,
                                    const std::string &line_spec) {
        return gca()->fcontour(fn, line_spec);
    }

    inline contours_handle fcontour(axes_handle ax,
                                    axes::fcontour_function_type fn,
                                    const std::array<double, 4> &xy_range,
                                    std::vector<double> levels,
                                    const std::string &line_spec = "") {
        return ax->fcontour(fn, xy_range, levels, line_spec);
    }

    inline contours_handle fcontour(axes_handle ax,
                                    axes::fcontour_function_type fn,
                                    const std::string &line_spec) {
        return ax->fcontour(fn, line_spec);
    }

    template <class T1, class... Args>
    auto feather(NotAxesHandle<T1> x, Args... args) {
        return gca()->feather(x, args...);
    }

    template <class... Args> auto feather(axes_handle ax, Args... args) {
        return ax->feather(args...);
    }

    template <class T1, class... Args>
    auto quiver(NotAxesHandle<T1> x, Args... args) {
        return gca()->quiver(x, args...);
    }

    template <class... Args> auto quiver(axes_handle ax, Args... args) {
        return ax->quiver(args...);
    }

    template <class T1, class... Args>
    auto quiver3(NotAxesHandle<T1> x, Args... args) {
        return gca()->quiver3(x, args...);
    }

    template <class... Args> auto quiver3(axes_handle ax, Args... args) {
        return ax->quiver3(args...);
    }

    template <class T1, class... Args>
    auto fence(NotAxesHandle<T1> x, Args... args) {
        return gca()->fence(x, args...);
    }

    template <class... Args> auto fence(axes_handle ax, Args... args) {
        return ax->fence(args...);
    }

    template <class T1, class... Args>
    auto fmesh(NotAxesHandle<T1> x, Args... args) {
        return gca()->fmesh(x, args...);
    }

    template <class... Args> auto fmesh(axes_handle ax, Args... args) {
        return ax->fmesh(args...);
    }

    template <class T1, class... Args>
    auto fsurf(NotAxesHandle<T1> x, Args... args) {
        return gca()->fsurf(x, args...);
    }

    template <class... Args> auto fsurf(axes_handle ax, Args... args) {
        return ax->fsurf(args...);
    }

    inline surface_handle fsurf(axes::fcontour_function_type fn,
                                const std::array<double, 2> &x_range,
                                const std::array<double, 2> &y_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(fn, x_range, y_range, line_spec, mesh_density);
    }

    inline surface_handle fsurf(axes::fcontour_function_type funx,
                                axes::fcontour_function_type funy,
                                axes::fcontour_function_type funz,
                                const std::array<double, 2> &u_range,
                                const std::array<double, 2> &v_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(funx, funy, funz, u_range, v_range, line_spec,
                            mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 4
    inline surface_handle fsurf(axes::fcontour_function_type fn,
                                const std::array<double, 4> &xy_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(fn, xy_range, line_spec, mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 4
    inline surface_handle fsurf(axes::fcontour_function_type fn,
                                std::initializer_list<double> xy_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(fn, xy_range, line_spec, mesh_density);
    }

    /// Parametric / Both ranges in the same array size 4
    inline surface_handle fsurf(axes::fcontour_function_type funx,
                                axes::fcontour_function_type funy,
                                axes::fcontour_function_type funz,
                                const std::array<double, 4> &uv_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(funx, funy, funz, uv_range, line_spec,
                            mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 2
    inline surface_handle
    fsurf(axes::fcontour_function_type fn,
          const std::array<double, 2> &xy_range = {-5, +5},
          std::string line_spec = "", double mesh_density = 40) {
        return gca()->fsurf(fn, xy_range, line_spec, mesh_density);
    }

    /// Function surf
    /// Parametric / Both ranges in the same array size 2
    inline surface_handle
    fsurf(axes::fcontour_function_type funx, axes::fcontour_function_type funy,
          axes::fcontour_function_type funz,
          const std::array<double, 2> &uv_range = {-5, +5},
          std::string line_spec = "", double mesh_density = 40) {
        return gca()->fsurf(funx, funy, funz, uv_range, line_spec,
                            mesh_density);
    }

    inline surface_handle fsurf(axes::fcontour_function_type funx,
                                axes::fcontour_function_type funy,
                                axes::fcontour_function_type funz,
                                std::initializer_list<double> &uv_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return gca()->fsurf(funx, funy, funz, uv_range, line_spec,
                            mesh_density);
    }

    /// Function surf - Core function
    inline surface_handle fsurf(axes_handle ax, axes::fcontour_function_type fn,
                                const std::array<double, 2> &x_range,
                                const std::array<double, 2> &y_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return ax->fsurf(fn, x_range, y_range, line_spec, mesh_density);
    }

    /// Function surf - Parametric
    /// Core parametric function
    inline surface_handle
    fsurf(axes_handle ax, axes::fcontour_function_type funx,
          axes::fcontour_function_type funy, axes::fcontour_function_type funz,
          const std::array<double, 2> &u_range,
          const std::array<double, 2> &v_range, std::string line_spec = "",
          double mesh_density = 40) {
        return ax->fsurf(funx, funy, funz, u_range, v_range, line_spec,
                         mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 4
    inline surface_handle fsurf(axes_handle ax, axes::fcontour_function_type fn,
                                const std::array<double, 4> &xy_range,
                                std::string line_spec = "",
                                double mesh_density = 40) {
        return ax->fsurf(fn, xy_range, line_spec, mesh_density);
    }

    /// Parametric / Both ranges in the same array size 4
    inline surface_handle
    fsurf(axes_handle ax, axes::fcontour_function_type funx,
          axes::fcontour_function_type funy, axes::fcontour_function_type funz,
          const std::array<double, 4> &uv_range, std::string line_spec = "",
          double mesh_density = 40) {
        return ax->fsurf(funx, funy, funz, uv_range, line_spec, mesh_density);
    }

    /// Function surf
    /// Grid / Both ranges in the same array size 2
    inline surface_handle
    fsurf(axes_handle ax, axes::fcontour_function_type fn,
          const std::array<double, 2> &xy_range = {-5, +5},
          std::string line_spec = "", double mesh_density = 40) {
        return ax->fsurf(fn, xy_range, line_spec, mesh_density);
    }

    /// Function surf
    /// Parametric / Both ranges in the same array size 2
    inline surface_handle
    fsurf(axes_handle ax, axes::fcontour_function_type funx,
          axes::fcontour_function_type funy, axes::fcontour_function_type funz,
          const std::array<double, 2> &uv_range = {-5, +5},
          std::string line_spec = "", double mesh_density = 40) {
        return ax->fsurf(funx, funy, funz, uv_range, line_spec, mesh_density);
    }

    inline surface_handle
    fsurf(axes_handle ax, axes::fcontour_function_type funx,
          axes::fcontour_function_type funy, axes::fcontour_function_type funz,
          std::initializer_list<double> &uv_range, std::string line_spec = "",
          double mesh_density = 40) {
        return ax->fsurf(funx, funy, funz, uv_range, line_spec, mesh_density);
    }

    template <class T1, class... Args>
    auto mesh(NotAxesHandle<T1> x, Args... args) {
        return gca()->mesh(x, args...);
    }

    template <class... Args> auto mesh(axes_handle ax, Args... args) {
        return ax->mesh(args...);
    }

    template <class T1, class... Args>
    auto meshc(NotAxesHandle<T1> x, Args... args) {
        return gca()->meshc(x, args...);
    }

    template <class... Args> auto meshc(axes_handle ax, Args... args) {
        return ax->meshc(args...);
    }

    template <class T1, class... Args>
    auto meshz(NotAxesHandle<T1> x, Args... args) {
        return gca()->meshz(x, args...);
    }

    template <class... Args> auto meshz(axes_handle ax, Args... args) {
        return ax->meshz(args...);
    }

    template <class T1, class... Args>
    auto ribbon(NotAxesHandle<T1> x, Args... args) {
        return gca()->ribbon(x, args...);
    }

    template <class... Args> auto ribbon(axes_handle ax, Args... args) {
        return ax->ribbon(args...);
    }

    template <class T1, class... Args>
    auto surf(NotAxesHandle<T1> x, Args... args) {
        return gca()->surf(x, args...);
    }

    template <class... Args> auto surf(axes_handle ax, Args... args) {
        return ax->surf(args...);
    }

    template <class T1, class... Args>
    auto surfc(NotAxesHandle<T1> x, Args... args) {
        return gca()->surfc(x, args...);
    }

    template <class... Args> auto surfc(axes_handle ax, Args... args) {
        return ax->surfc(args...);
    }

    template <class T1, class... Args>
    auto waterfall(NotAxesHandle<T1> x, Args... args) {
        return gca()->waterfall(x, args...);
    }

    template <class... Args> auto waterfall(axes_handle ax, Args... args) {
        return ax->waterfall(args...);
    }

    template <class T1, class... Args>
    auto graph(NotAxesHandle<T1> x, Args... args) {
        return gca()->graph(x, args...);
    }

    template <class... Args> auto graph(axes_handle ax, Args... args) {
        return ax->graph(args...);
    }

    template <class T1, class... Args>
    auto imshow(NotAxesHandle<T1> x, Args... args) {
        return gca()->imshow(x, args...);
    }

    template <class... Args> auto imshow(axes_handle ax, Args... args) {
        return ax->imshow(args...);
    }

    template <class T1, class... Args>
    auto image(NotAxesHandle<T1> x, Args... args) {
        return gca()->image(x, args...);
    }

    template <class... Args> auto image(axes_handle ax, Args... args) {
        return ax->image(args...);
    }

    template <class T1, class... Args>
    auto text(NotAxesHandle<T1> x, Args... args) {
        return gca()->text(x, args...);
    }

    template <class... Args> auto text(axes_handle ax, Args... args) {
        return ax->text(args...);
    }

    inline labels_handle text(const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::vector<std::string> &texts) {
        return gca()->text(x, y, texts);
    }

    inline labels_handle text(const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::string &str) {
        return gca()->text(x, y, str);
    }

    inline labels_handle text(axes_handle ax, const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::vector<std::string> &texts) {
        return ax->text(x, y, texts);
    }

    inline labels_handle text(axes_handle ax, const std::vector<double> &x,
                              const std::vector<double> &y,
                              const std::string &str) {
        return ax->text(x, y, str);
    }

    template <class T1, class... Args>
    auto arrow(NotAxesHandle<T1> x, Args... args) {
        return gca()->arrow(x, args...);
    }

    template <class... Args> auto arrow(axes_handle ax, Args... args) {
        return ax->arrow(args...);
    }

    inline auto line(double x1, double y1, double x2, double y2) {
        return gca()->line(x1, x1, x2, y2);
    }

    inline auto line(axes_handle ax, double x1, double y1, double x2,
                     double y2) {
        return ax->line(x1, x1, x2, y2);
    }

    template <class T1, class... Args>
    auto textarrow(NotAxesHandle<T1> x, Args... args) {
        return gca()->textarrow(x, args...);
    }

    template <class... Args> auto textarrow(axes_handle ax, Args... args) {
        return ax->textarrow(args...);
    }

    template <class T1, class... Args>
    auto rectangle(NotAxesHandle<T1> x, Args... args) {
        return gca()->rectangle(x, args...);
    }

    template <class... Args> auto rectangle(axes_handle ax, Args... args) {
        return ax->rectangle(args...);
    }

    template <class T1, class... Args>
    auto textbox(NotAxesHandle<T1> x, Args... args) {
        return gca()->textbox(x, args...);
    }

    template <class... Args> auto textbox(axes_handle ax, Args... args) {
        return ax->textbox(args...);
    }

    template <class T1, class... Args>
    auto fill(NotAxesHandle<T1> x, Args... args) {
        return gca()->fill(x, args...);
    }

    template <class... Args> auto fill(axes_handle ax, Args... args) {
        return ax->fill(args...);
    }

    template <class T1, class... Args>
    auto polygon(NotAxesHandle<T1> x, Args... args) {
        return gca()->polygon(x, args...);
    }

    template <class... Args> auto polygon(axes_handle ax, Args... args) {
        return ax->polygon(args...);
    }

    template <class T1, class... Args>
    auto ellipse(NotAxesHandle<T1> x, Args... args) {
        return gca()->ellipse(x, args...);
    }

    template <class... Args> auto ellipse(axes_handle ax, Args... args) {
        return ax->ellipse(args...);
    }

    template <class T1, class... Args>
    auto digraph(NotAxesHandle<T1> x, Args... args) {
        return gca()->digraph(x, args...);
    }

    template <class... Args> auto digraph(axes_handle ax, Args... args) {
        return ax->digraph(args...);
    }

    template <class T1, class... Args>
    auto imagesc(NotAxesHandle<T1> x, Args... args) {
        return gca()->imagesc(x, args...);
    }

    template <class... Args> auto imagesc(axes_handle ax, Args... args) {
        return ax->imagesc(args...);
    }

    template <class T1, class... Args>
    auto boxplot(NotAxesHandle<T1> x, Args... args) {
        return gca()->boxplot(x, args...);
    }

    template <class... Args> auto boxplot(axes_handle ax, Args... args) {
        return ax->boxplot(args...);
    }

    template <class T1, class... Args>
    auto boxchart(NotAxesHandle<T1> x, Args... args) {
        return gca()->boxchart(x, args...);
    }

    template <class... Args> auto boxchart(axes_handle ax, Args... args) {
        return ax->boxchart(args...);
    }

} // namespace matplot

#endif // MATPLOTPLUSPLUS_PLOT_H
