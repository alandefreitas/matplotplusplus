//
// Adapted from
// https://github.com/matplotlib/matplotlib/blob/master/src/_contour.cpp
//

// This file contains liberal use of asserts to assist code development and
// debugging.  Standard matplotlib builds disable asserts so they cause no
// performance reduction.  To enable the asserts, you need to undefine the
// NDEBUG macro, which is achieved by adding the following
//     undef_macros=['NDEBUG']
// to the appropriate make_extension call in setupext.py, and then rebuilding.
#define NO_IMPORT_ARRAY

// #include "mplutils.h"
// https://github.com/matplotlib/matplotlib/blob/master/src/mplutils.h
// https://github.com/matplotlib/matplotlib/blob/master/src/mplutils.cpp
#include <algorithm>
#include <cassert>
#include <matplot/axes_objects/contours.h>
#include <matplot/util/contourc.h>

namespace matplot::detail {
    namespace {
        constexpr double k_Epsilon = 1e-5;

        inline array_2d<double> to_array_2d(const vector_2d &v) {
            array_2d<double> a(v.size(), !v.empty() ? v[0].size() : 0);

            if (!a.empty()) {
                assert(std::all_of(v.begin(), v.end(), [&a](const auto &v) {
                    return v.size() == a.cols();
                }));

                for (size_t rr = 0; rr < a.rows(); ++rr)
                    std::copy_n(v[rr].begin(), a.cols(), &(a(rr, 0)));
            }

            return a;
        }
    } // namespace

    contour_generator::contour_generator(const matplot::vector_2d &x,
                                         const matplot::vector_2d &y,
                                         const matplot::vector_2d &z)
        : x_(detail::to_array_2d(x)), y_(detail::to_array_2d(y)),
          z_(detail::to_array_2d(z)) {
        assert(x_.rows() == y_.rows() && x_.rows() == z_.rows());
        assert(x_.cols() == y_.cols() && x_.cols() == z_.cols());
        assert(!z_.empty());

        calculate_minmax();
        generate_edges_and_triangles();
    }

    size_t contour_generator::activate_edges(double z) const noexcept {
        size_t num_active = 0;

        for (edge &e : edges_) {
            if (activate_edge(z, e))
                ++num_active;
        }

        return num_active;
    }

    void contour_generator::calculate_minmax() noexcept {
        auto [x_min, x_max] = std::minmax_element(x_.begin(), x_.end());
        x_min_ = *x_min;
        x_max_ = *x_max;

        auto [y_min, y_max] = std::minmax_element(y_.begin(), y_.end());
        y_min_ = *y_min;
        y_max_ = *y_max;
    }

    contour_generator::vertices_list_type
    contour_generator::create_contour(double level) const {
        contour_generator::vertices_list_type vertices;
        generate_contours(level, vertices);
        return vertices;
    }

    contour_generator::vertices_list_type
    contour_generator::create_filled_contour(double lower_level,
                                             double upper_level) const {
        contour_generator::vertices_list_type vertices;
        generate_contours(lower_level, vertices);

        /* to demarcate upper contour lines and lower contour lines */
        vertices.first.push_back(NaN);
        vertices.second.push_back(NaN);

        generate_contours(upper_level, vertices);

        return vertices;
    }

    void contour_generator::generate_contours(double z, contour_generator::vertices_list_type &vertices) const {
        auto fn_push_contour = [&vertices](const contour& c) {
          if(c.points.empty())
              return;

          vertices.first.reserve(vertices.first.size() + c.points.size() + 1);
          vertices.second.reserve(vertices.second.size() + c.points.size() + 1);

          for (auto point : c.points) {
              vertices.first.push_back(point.x);
              vertices.second.push_back(point.y);
          }
          vertices.first.push_back(NaN);
          vertices.second.push_back(NaN);
        };

        size_t num_active = activate_edges(z);

        /* first look for contours that end on a boundary */
        for(auto & e : edges_) {
            if(e.flags.on_boundary && e.flags.active)
                fn_push_contour(trace_contour(z, num_active, e));
            if(num_active == 0)
                break;
        }

        /* lastly look for closed contours */
        for(auto & e : edges_) {
            if(e.flags.active) {
                /* shouldn't have any left on a boundary */
                assert(!e.flags.on_boundary);
                fn_push_contour(trace_contour(z, num_active, e));
            }
            if(num_active == 0)
                break;
        }
    }

    void contour_generator::generate_edges_and_triangles() {
        /*
         * This function generates a mesh from the provided grid of values.
         *
         * Upper and lower triangles:
         *          lower           upper
         *                            1
         *          |\              ----
         *        0 | \ 1         0 \  | 2
         *          |__\             \ |
         *            2
         *      numbers indicate the order in which edges
         *      are stored in triangle
         *
         * Mesh:
         *                1
         *           -----------
         *          | \   0     |
         *          |   \       |
         *         0|1   0\1   0|1
         *          |       \   |
         *          |    1    \ |
         *           -----------
         *                0
         *      numbers indicate the order in which triangles
         *      are stored in edge
         */

        auto fn_push_lower_triangle = [this](edge *e0, edge *e1,
                                             edge *e2) -> triangle * {
            triangle t{};
            t.e[0] = e0;
            t.e[1] = e1;
            t.e[2] = e2;

            triangle *p_t = &triangles_.emplace_back(t);
            e0->t[1] = p_t;
            e1->t[0] = p_t;
            e2->t[1] = p_t;

            return p_t;
        };
        auto fn_push_upper_triangle = [this](edge *e0, edge *e1,
                                             edge *e2) -> triangle * {
            triangle t{};
            t.e[0] = e0;
            t.e[1] = e1;
            t.e[2] = e2;

            triangle *p_t = &triangles_.emplace_back(t);
            e0->t[1] = p_t;
            e1->t[0] = p_t;
            e2->t[0] = p_t;

            return p_t;
        };
        auto fn_push_diagonal_edge = [this](size_t r, size_t c) -> edge * {
            return &edges_.emplace_back(generate_diagonal_edge(r, c));
        };
        auto fn_push_horizontal_edge = [this](size_t r, size_t c) -> edge * {
            return &edges_.emplace_back(generate_horizontal_edge(r, c));
        };
        auto fn_push_vertical_edge = [this](size_t r, size_t c) -> edge * {
            return &edges_.emplace_back(generate_vertical_edge(r, c));
        };

        /* 2 triangles per vertex (except for vertices at bottom and
         * right boundaries, which don't generate any triangles)*/
        triangles_.reserve(2 * (z_.cols() - 1) * (z_.rows() - 1));

        /* at most 3 edges per vertex */
        edges_.reserve(3 * z_.size());

        /* generate first horizontal edge */
        edge *e_u = fn_push_horizontal_edge(0, 0);

        /* generate first row of horizontal edges */
        for (size_t cc = 1; cc < z_.cols() - 1; ++cc)
            fn_push_horizontal_edge(0, cc);

        for (size_t rr = 1; rr < z_.rows(); ++rr) {
            /* generate first vertical edge */
            edge *e_v = fn_push_vertical_edge(rr - 1, 0);

            /* generate row of vertical and diagonal edges */
            for (size_t cc = 0; cc < z_.cols() - 1; ++cc) {
                fn_push_diagonal_edge(rr - 1, cc);
                fn_push_vertical_edge(rr - 1, cc + 1);
            }

            /* generate first horizontal edge of lower row */
            edge *e_l = fn_push_horizontal_edge(rr, 0);

            /* generate lower row of horizontal edges */
            for (size_t cc = 1; cc < z_.cols() - 1; ++cc)
                fn_push_horizontal_edge(rr, cc);

            /* generate row of triangles */
            for (size_t cc = 0; cc < z_.cols() - 1; ++cc) {
                fn_push_lower_triangle(e_v + 2 * cc, e_v + 2 * cc + 1,
                                       e_l + cc);
                fn_push_upper_triangle(e_v + 2 * cc + 1, e_u + cc,
                                       e_v + 2 * (cc + 1));
            }

            /* lower row of h edges becomes upper row of h edges
             * for next mesh row */
            e_u = e_l;
        }
    }

    contour_generator::contour
    contour_generator::trace_contour(double z, size_t &num_active,
                                     edge &e) const {
        contour ctr;
        ctr.closed = !e.flags.on_boundary;

        auto fn_deactivate_edge = [&num_active](edge &e) {
            assert(e.flags.active);
            e.flags.active = false;
            --num_active;
        };

        auto fn_fuzzy_equals = [this](contour::point p1, contour::point p2) {
            double unit_x = std::abs(x_max_ - x_min_),
                   unit_y = std::abs(y_max_ - y_min_);

            return ((std::abs(p1.x - p2.x) < unit_x * k_Epsilon) &&
                    (std::abs(p1.y - p2.y) < unit_y * k_Epsilon));
        };

        assert(e.t[0] || e.t[1]);

        edge *e_start = &e, *e_cur = &e;

        /* push back first point in edge (we intentionally not deactivate
         * this edge so that we can return back to it if this contour
         * is closed) */
        ctr.points.emplace_back(contour_point(z, *e_cur));

        triangle *t_last = nullptr;
        do {
            /* find triangle to continue (but not whence we came) */
            triangle *t_cur;
            if (e_cur->t[0] == t_last)
                t_cur = e_cur->t[1];
            else
                t_cur = e_cur->t[0];

            /* find edge to continue (but not whence we came) */
            edge *e_next = nullptr;
            for (edge *e_t : t_cur->e) {
                if (e_t != e_cur && e_t->flags.active)
                    e_next = e_t;
            }

            if (!e_next) {
                /* contour ended unexpectedly */
                return contour();
            }

            e_cur = e_next;
            t_last = t_cur;

            fn_deactivate_edge(*e_cur);

            /* do not include points at diagonal edges or we get awful
             * zigzag patterns */
            if (!e_cur->flags.diagonal) {
                contour::point p = contour_point(z, *e_cur);

                /* do not include points that are too close to each other */
                if (!fn_fuzzy_equals(ctr.points.back(), p))
                    ctr.points.emplace_back(contour_point(z, *e_cur));
            }
        } while ((e_start != e_cur) && (!e_cur->flags.on_boundary));

        if (e_start == e_cur) {
            ctr.closed = true;

            /* for a closed contour the first and last point should be equal */
            ctr.points.front() = ctr.points.back();
        } else {
            ctr.closed = false;

            /* for an open contour we must deactivate the starting edge (we
             * didn't do it initially so we have to do it now) */
            fn_deactivate_edge(*e_start);
        }

        return ctr;
    }
} // namespace matplot::detail

namespace matplot {
    std::tuple<double, std::pair<double, double>, std::pair<size_t, size_t>>
    find_closest_point_on_path(const std::vector<double> &xs,
                               const std::vector<double> &ys, double px,
                               double py) {
        if (xs.size() == 1) {
            double m = pow(px - xs[0], 2) + pow(py - ys[0], 2);
            return std::make_tuple(m, std::make_pair(xs[0], ys[0]),
                                   std::make_pair(0, 0));
        }
        // Individual segment vectors.
        vector_1d dxs(xs.size() - 1);
        for (size_t i = 0; i < dxs.size(); ++i) {
            dxs[i] = xs[i + 1] - xs[i];
        }
        vector_1d dys(ys.size() - 1);
        for (size_t i = 0; i < dys.size(); ++i) {
            dys[i] = ys[i + 1] - ys[i];
        }

        vector_1d norms(dxs.size());
        for (size_t i = 0; i < dxs.size(); ++i) {
            norms[i] = pow(dxs[i], 2) + pow(dys[i], 2);
            // For zero-length segment, replace 0/0 by 0/1.
            if (norms[i] == 0) {
                norms[i] = 1;
            }
        }

        // Project onto each segment in relative 0-1 coords.
        // numpy.clip(a, a_min, a_max, out=None, **kwargs)[source]
        vector_1d a(dxs.size());
        for (size_t i = 0; i < dxs.size(); ++i) {
            a[i] = ((px - xs[i] * dxs[i]) + (py - ys[i] * dys[i])) / norms[i];
        }
        vector_1d rel_projs =
            transform(a, [](double a) { return truncate(a, 0, 1); });

        // Projs. onto each segment, in (x, y).
        vector_1d projxs(rel_projs.size());
        vector_1d projys(rel_projs.size());
        for (size_t i = 0; i < projxs.size(); ++i) {
            projxs[i] = xs[i] + rel_projs[i] * dxs[i];
            projys[i] = ys[i] + rel_projs[i] * dys[i];
        }

        // Squared distances.
        vector_1d d2s = transform(projxs, projys, [&](double x, double y) {
            return pow(x - px, 2) + pow(y - py, 2);
        });

        size_t imin = std::min_element(d2s.begin(), d2s.end()) - d2s.begin();
        return std::make_tuple(d2s[imin],
                               std::make_pair(projxs[imin], projys[imin]),
                               std::make_pair(imin, imin + 1));
    }

    contour_line_type contour_line(const vector_2d &X, const vector_2d &Y,
                                   const vector_2d &Z, double level) {
        // create_contour is an external algorithm that seems to be generating
        // incoherent contour lines sometimes. So we regenerate the contour
        // until we get sometime that makes sense (something at least not out of
        // range). We then make sure this contour line matches the next contour
        // line.
        double x_min = min(X);
        double x_max = max(X);
        double y_min = min(X);
        double y_max = max(X);

        auto contour_is_in_bounds = [&](const contour_line_type &c) {
            double cx_min = min(c.first);
            double cx_max = max(c.first);
            double cy_min = min(c.second);
            double cy_max = max(c.second);
            const bool xminok = cx_min >= x_min;
            const bool xmaxok = cx_max <= x_max;
            const bool yminok = cy_min >= y_min;
            const bool ymaxok = cy_max <= y_max;
            return (xminok && xmaxok && yminok && ymaxok);
        };

        auto contour_line_in_bounds = [&]() {
            detail::contour_generator contour_generator(X, Y, Z);
            auto c = contour_generator.create_contour(level);
            size_t attempts = 0;
            while (!contour_is_in_bounds(c) && attempts < 10) {
                std::cerr << "Contour out of bounds" << std::endl;
                detail::contour_generator contour_generator2(X, Y, Z);
                c = contour_generator2.create_contour(level);
                ++attempts;
            }
            return c;
        };

        auto c = contour_line_in_bounds();

        return c;
    }

    std::vector<contour_line_type> contourc(const vector_2d &x,
                                            const vector_2d &y,
                                            const vector_2d &z,
                                            const vector_1d &levels) {
        std::vector<contour_line_type> lines;
        for (const double &level : levels) {
            lines.emplace_back(contour_line(x, y, z, level));
        }
        return lines;
    }

    std::vector<contour_line_type> contourc(const vector_2d &x,
                                            const vector_2d &y,
                                            const vector_2d &z,
                                            size_t n_levels) {
        double zmin = z[0][0];
        double zmax = z[0][0];
        for (const auto &z_row : z) {
            auto [minit, maxit] = std::minmax(z_row.begin(), z_row.end());
            if (*minit < zmin) {
                zmin = *minit;
            }
            if (*maxit < zmax) {
                zmax = *maxit;
            }
        }
        auto levels = contours::determine_contour_levels(zmin, zmax, n_levels);
        return contourc(x, y, z, levels);
    }

    std::vector<contour_line_type> contourc(const vector_2d &z,
                                            const vector_1d &levels) {
        auto [x, y] = meshgrid(iota(1, 1, z[0].size()), iota(1, 1, z.size()));
        return contourc(x, y, z, levels);
    }

    std::vector<contour_line_type> contourc(const vector_2d &z,
                                            size_t n_levels) {
        auto [x, y] = meshgrid(iota(1, 1, z[0].size()), iota(1, 1, z.size()));
        return contourc(x, y, z, n_levels);
    }

} // namespace matplot
