//
// Adapted from gnuplot: src/contour.c

#ifndef MATPLOTPLUSPLUS_CONTOURC_H
#define MATPLOTPLUSPLUS_CONTOURC_H

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <matplot/core/axes_type.h>
#include <matplot/util/common.h>
#include <matplot/util/handle_types.h>
#include <numeric>
#include <stdint.h>
#include <vector>

namespace matplot::detail {
    template <typename T> class array_2d {
        using value_type = T;
        using size_type = size_t;
        using data_type = std::vector<value_type>;

      public:
        array_2d() : rows_(0), cols_(0), data_() {}
        array_2d(size_type rows, size_type cols) : array_2d() {
            resize(rows, cols);
        }

        [[nodiscard]] auto begin() noexcept { return data_.begin(); }
        [[nodiscard]] auto begin() const noexcept { return data_.begin(); }
        [[nodiscard]] auto end() noexcept { return data_.end(); }
        [[nodiscard]] auto end() const noexcept { return data_.end(); }

        void resize(size_type rows, size_type cols) {
            rows_ = rows * cols > 0 ? rows : 0;
            cols_ = rows * cols > 0 ? cols : 0;
            data_.resize(rows_ * cols_);
        }

        [[nodiscard]] size_type index(size_type r, size_type c) const noexcept {
            assert(r < rows_ && c < cols_);
            return r * cols_ + c;
        }

        [[nodiscard]] bool empty() const noexcept { return data_.empty(); }

        [[nodiscard]] size_type cols() const noexcept { return cols_; }
        [[nodiscard]] size_type rows() const noexcept { return rows_; }
        [[nodiscard]] size_type size() const noexcept { return data_.size(); }

        [[nodiscard]] value_type &operator()(size_type r,
                                             size_type c) noexcept {
            return data_[index(r, c)];
        }
        [[nodiscard]] const value_type &operator()(size_type r,
                                                   size_type c) const noexcept {
            return data_[index(r, c)];
        }
        [[nodiscard]] value_type &operator[](size_type i) noexcept {
            assert(i < data_.size());
            return data_[i];
        }
        [[nodiscard]] const value_type &operator[](size_type i) const noexcept {
            assert(i < data_.size());
            return data_[i];
        }

      private:
        size_type rows_, cols_;
        data_type data_;
    };

    class contour_generator {
        struct edge;

        struct contour {
            struct point {
                double x, y;
            };

            bool closed;
            std::vector<point> points;
            edge* boundary_edges[2]{};
        };

        struct triangle;

        struct edge {
            size_t p[2];
            triangle *t[2];
            struct {
                bool active : 1;
                bool on_boundary : 1;
                bool horizontal : 1;
                bool vertical : 1;
                bool diagonal : 1;
            } flags;
        };

        struct triangle {
            edge *e[3];
        };

      public:
        contour_generator() = default;
        contour_generator(const vector_2d &x, const vector_2d &y,
                          const vector_2d &z);

        using vertices_list_type = std::pair<vector_1d, vector_1d>;
        vertices_list_type create_contour(double level) const;
        vertices_list_type create_filled_contour(double lower_level, double upper_level) const;

      private:
        struct generation_parameters {
            struct {
                bool closed_contour : 1;
            } flags;
            size_t index;
            size_t num_active;
        };

        void calculate_minmax() noexcept;

        edge generate_diagonal_edge(size_t r, size_t c) const noexcept {
            assert(r < (z_.rows() - 1) && c <= (z_.cols() - 1));

            edge e{};
            e.p[0] = z_.index(r, c);
            e.p[1] = z_.index(r + 1, c + 1);
            e.t[0] = nullptr;
            e.t[1] = nullptr;
            e.flags.on_boundary = false;
            e.flags.horizontal = false;
            e.flags.vertical = false;
            e.flags.diagonal = true;

            return e;
        }
        edge generate_horizontal_edge(size_t r, size_t c) const noexcept {
            assert(r < z_.rows() && c <= (z_.cols() - 1));

            edge e{};
            e.p[0] = z_.index(r, c);
            e.p[1] = z_.index(r, c + 1);
            e.t[0] = nullptr;
            e.t[1] = nullptr;
            e.flags.on_boundary = (r == 0 || r == (z_.rows() - 1));
            e.flags.horizontal = true;
            e.flags.vertical = false;
            e.flags.diagonal = false;

            return e;
        }
        edge generate_vertical_edge(size_t r, size_t c) const noexcept {
            assert(r < (z_.rows() - 1) && c <= z_.cols());

            edge e{};
            e.p[0] = z_.index(r, c);
            e.p[1] = z_.index(r + 1, c);
            e.t[0] = nullptr;
            e.t[1] = nullptr;
            e.flags.on_boundary = (c == 0 || c == (z_.cols() - 1));
            e.flags.horizontal = false;
            e.flags.vertical = true;
            e.flags.diagonal = false;

            return e;
        }
        void generate_edges_and_triangles();

        inline bool activate_edge(double z,
                                  contour_generator::edge &e) const noexcept {
            /* use the same test at both vertices to avoid roundoff errors */
            if ((z_[e.p[0]] >= z) != (z_[e.p[1]] >= z))
                e.flags.active = true;

            return e.flags.active;
        }
        size_t activate_edges(double z) const noexcept;
        void generate_contours(double z, vertices_list_type& vertices) const;
        void generate_filled_contours(double z_lower, double z_upper, vertices_list_type& vertices) const;

        contour::point contour_point(double z, const edge &e) const noexcept {
            /* test if t is out of interval [0:1]
             * (should not happen but who knows ...) */
            double t = (z - z_[e.p[0]]) / (z_[e.p[1]] - z_[e.p[0]]);
            t = std::clamp(t, 0.0, 1.0);

            return {x_[e.p[1]] * t + x_[e.p[0]] * (1 - t),
                    y_[e.p[1]] * t + y_[e.p[0]] * (1 - t)};
        }
        contour trace_contour(double z, size_t &num_active, edge &e) const;

        array_2d<double> x_, y_, z_;
        double x_min_, x_max_, y_min_, y_max_;
        mutable std::vector<edge> edges_;
        std::vector<triangle> triangles_;
    };
} // namespace matplot::detail

namespace matplot {
    /// Segments are considered as including their end-points; i.e if the
    //            closest point on the path is a node in *xys* with index *i*,
    //            this
    //        returns ``(i-1, i)``.  For the special case where *xys* is a
    //        single
    //            point, this returns ``(0, 0)``.
    /// \param xys  Coordinates of vertices
    /// \param p    Coordinates of point
    /// \return     Minimum square distance of *p* to *xys*.
    /// \return     Projection of *p* onto *xys*.
    /// \return     Consecutive indices of vertices of segment in *xys* where
    /// *proj* is.
    std::tuple<double, std::pair<double, double>, std::pair<size_t, size_t>>
    find_closest_point_on_path(const std::vector<double> &xs,
                               const std::vector<double> &ys, double px,
                               double py);

    /// \brief Compute contour lines
    ///
    /// This will use a square tracing algorithm to find the contour points
    /// of a contour line. We look for level transitions on every
    /// grid quadrant and determine the specific point with an interpolation
    /// that considers how far the the points in the quadrant are for the
    /// contour level.
    ///
    /// We compute these lines outside gnuplot because creating contour lines
    /// in 2d plots (with the "plot" command) in gnuplot involves so many
    /// work-arounds that it is just not worth it.
    ///
    /// \see
    /// https://en.wikipedia.org/wiki/Boundary_tracing#Square_tracing_algorithm
    /// \see
    /// http://www.batesville.k12.in.us/physics/CalcNet/grapher/how_it_works.htm
    /// \see http://dx.doi.org/10.1093/comjnl/33.5.402
    /// \see http://dx.doi.org/10.1016/0097-8493%2891%2990002-Y
    /// \see http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=576861
    /// \see https://github.com/matplotlib/matplotlib/blob/master/src/_contour.h
    using contour_line_type = std::pair<vector_1d, vector_1d>;
    contour_line_type contour_line(const vector_2d &x, const vector_2d &y,
                                   const vector_2d &z, double level);

    std::vector<contour_line_type> contourc(const vector_2d &x,
                                            const vector_2d &y,
                                            const vector_2d &z,
                                            const vector_1d &levels);

    std::vector<contour_line_type> contourc(const vector_2d &x,
                                            const vector_2d &y,
                                            const vector_2d &z,
                                            size_t n_levels = 7);

    std::vector<contour_line_type> contourc(const vector_2d &z,
                                            const vector_1d &levels);

    std::vector<contour_line_type> contourc(const vector_2d &z,
                                            size_t n_levels = 7);

} // namespace matplot

#endif // MATPLOTPLUSPLUS_CONTOURC_H
