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
#include <type_traits>

namespace matplot {

// 'kind' codes.
#define MOVETO 1
#define LINETO 2
#define CLOSEPOLY 79

// Point indices from current quad index.
#define POINT_SW (quad)
#define POINT_SE (quad + 1)
#define POINT_NW (quad + _nx)
#define POINT_NE (quad + _nx + 1)

// CacheItem masks, only accessed directly to set.  To read, use accessors
// detailed below.  1 and 2 refer to level indices (lower and upper).
#define MASK_Z_LEVEL 0x0003   // Combines the following two.
#define MASK_Z_LEVEL_1 0x0001 // z > lower_level.
#define MASK_Z_LEVEL_2 0x0002 // z > upper_level.
#define MASK_VISITED_1 0x0004 // Algorithm has visited this quad.
#define MASK_VISITED_2 0x0008
#define MASK_SADDLE_1 0x0010 // quad is a saddle quad.
#define MASK_SADDLE_2 0x0020
#define MASK_SADDLE_LEFT_1 0x0040 // Contours turn left at saddle quad.
#define MASK_SADDLE_LEFT_2 0x0080
#define MASK_SADDLE_START_SW_1 0x0100 // Next visit starts on S or W edge.
#define MASK_SADDLE_START_SW_2 0x0200
#define MASK_BOUNDARY_S 0x0400 // S edge of quad is a boundary.
#define MASK_BOUNDARY_W 0x0800 // W edge of quad is a boundary.
// EXISTS_QUAD bit is always used, but the 4 EXISTS_CORNER are only used if
// _corner_mask is true.  Only one of EXISTS_QUAD or EXISTS_??_CORNER is ever
// set per quad, hence not using unique bits for each; care is needed when
// testing for these flags as they overlap.
#define MASK_EXISTS_QUAD 0x1000      // All of quad exists (is not masked).
#define MASK_EXISTS_SW_CORNER 0x2000 // SW corner exists, NE corner is masked.
#define MASK_EXISTS_SE_CORNER 0x3000
#define MASK_EXISTS_NW_CORNER 0x4000
#define MASK_EXISTS_NE_CORNER 0x5000
#define MASK_EXISTS 0x7000 // Combines all 5 EXISTS masks.

// The following are only needed for filled contours.
#define MASK_VISITED_S 0x10000      // Algorithm has visited S boundary.
#define MASK_VISITED_W 0x20000      // Algorithm has visited W boundary.
#define MASK_VISITED_CORNER 0x40000 // Algorithm has visited corner edge.

// Accessors for various CacheItem masks.  li is shorthand for level_index.
#define Z_LEVEL(quad) (_cache[quad] & MASK_Z_LEVEL)
#define Z_NE Z_LEVEL(POINT_NE)
#define Z_NW Z_LEVEL(POINT_NW)
#define Z_SE Z_LEVEL(POINT_SE)
#define Z_SW Z_LEVEL(POINT_SW)
#define VISITED(quad, li)                                                      \
    ((_cache[quad] & (li == 1 ? MASK_VISITED_1 : MASK_VISITED_2)) != 0)
#define VISITED_S(quad) ((_cache[quad] & MASK_VISITED_S) != 0)
#define VISITED_W(quad) ((_cache[quad] & MASK_VISITED_W) != 0)
#define VISITED_CORNER(quad) ((_cache[quad] & MASK_VISITED_CORNER) != 0)
#define SADDLE(quad, li)                                                       \
    ((_cache[quad] & (li == 1 ? MASK_SADDLE_1 : MASK_SADDLE_2)) != 0)
#define SADDLE_LEFT(quad, li)                                                  \
    ((_cache[quad] & (li == 1 ? MASK_SADDLE_LEFT_1 : MASK_SADDLE_LEFT_2)) != 0)
#define SADDLE_START_SW(quad, li)                                              \
    ((_cache[quad] &                                                           \
      (li == 1 ? MASK_SADDLE_START_SW_1 : MASK_SADDLE_START_SW_2)) != 0)
#define BOUNDARY_S(quad) ((_cache[quad] & MASK_BOUNDARY_S) != 0)
#define BOUNDARY_W(quad) ((_cache[quad] & MASK_BOUNDARY_W) != 0)
#define BOUNDARY_N(quad) BOUNDARY_S(quad + _nx)
#define BOUNDARY_E(quad) BOUNDARY_W(quad + 1)
#define EXISTS_QUAD(quad) ((_cache[quad] & MASK_EXISTS) == MASK_EXISTS_QUAD)
#define EXISTS_NONE(quad) ((_cache[quad] & MASK_EXISTS) == 0)
// The following are only used if _corner_mask is true.
#define EXISTS_SW_CORNER(quad)                                                 \
    ((_cache[quad] & MASK_EXISTS) == MASK_EXISTS_SW_CORNER)
#define EXISTS_SE_CORNER(quad)                                                 \
    ((_cache[quad] & MASK_EXISTS) == MASK_EXISTS_SE_CORNER)
#define EXISTS_NW_CORNER(quad)                                                 \
    ((_cache[quad] & MASK_EXISTS) == MASK_EXISTS_NW_CORNER)
#define EXISTS_NE_CORNER(quad)                                                 \
    ((_cache[quad] & MASK_EXISTS) == MASK_EXISTS_NE_CORNER)
#define EXISTS_ANY_CORNER(quad) (!EXISTS_NONE(quad) && !EXISTS_QUAD(quad))
#define EXISTS_W_EDGE(quad)                                                    \
    (EXISTS_QUAD(quad) || EXISTS_SW_CORNER(quad) || EXISTS_NW_CORNER(quad))
#define EXISTS_E_EDGE(quad)                                                    \
    (EXISTS_QUAD(quad) || EXISTS_SE_CORNER(quad) || EXISTS_NE_CORNER(quad))
#define EXISTS_S_EDGE(quad)                                                    \
    (EXISTS_QUAD(quad) || EXISTS_SW_CORNER(quad) || EXISTS_SE_CORNER(quad))
#define EXISTS_N_EDGE(quad)                                                    \
    (EXISTS_QUAD(quad) || EXISTS_NW_CORNER(quad) || EXISTS_NE_CORNER(quad))
    // Note that EXISTS_NE_CORNER(quad) is equivalent to BOUNDARY_SW(quad), etc.

    QuadEdge::QuadEdge() : quad(-1), edge(Edge::Edge_None) {}

    QuadEdge::QuadEdge(long quad_, Edge edge_) : quad(quad_), edge(edge_) {}

    bool QuadEdge::operator<(const QuadEdge &other) const {
        if (quad != other.quad)
            return quad < other.quad;
        else
            return edge < other.edge;
    }

    bool QuadEdge::operator==(const QuadEdge &other) const {
        return quad == other.quad && edge == other.edge;
    }

    bool QuadEdge::operator!=(const QuadEdge &other) const {
        return !operator==(other);
    }

    std::ostream &operator<<(std::ostream &os, const QuadEdge &quad_edge) {
        return os << quad_edge.quad << ' '
                  << static_cast<std::underlying_type<Edge>::type>(
                         quad_edge.edge);
    }

    XY::XY() : x(0), y(0) {}

    XY::XY(const double &x_, const double &y_) : x(x_), y(y_) {}

    bool XY::operator==(const XY &other) const {
        return x == other.x && y == other.y;
    }

    bool XY::operator!=(const XY &other) const {
        return x != other.x || y != other.y;
    }

    XY XY::operator*(const double &multiplier) const {
        return XY(x * multiplier, y * multiplier);
    }

    const XY &XY::operator+=(const XY &other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    const XY &XY::operator-=(const XY &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    XY XY::operator+(const XY &other) const {
        return XY(x + other.x, y + other.y);
    }

    XY XY::operator-(const XY &other) const {
        return XY(x - other.x, y - other.y);
    }

    std::ostream &operator<<(std::ostream &os, const XY &xy) {
        return os << '(' << xy.x << ' ' << xy.y << ')';
    }

    ContourLine::ContourLine(bool is_hole)
        : std::vector<XY>(), _is_hole(is_hole), _parent(0) {}

    void ContourLine::add_child(ContourLine *child) {
        assert(!_is_hole && "Cannot add_child to a hole");
        assert(child != 0 && "Null child ContourLine");
        _children.push_back(child);
    }

    void ContourLine::clear_parent() {
        assert(is_hole() && "Cannot clear parent of non-hole");
        assert(_parent != 0 && "Null parent ContourLine");
        _parent = 0;
    }

    const ContourLine::Children &ContourLine::get_children() const {
        assert(!_is_hole && "Cannot get_children of a hole");
        return _children;
    }

    const ContourLine *ContourLine::get_parent() const {
        assert(_is_hole && "Cannot get_parent of a non-hole");
        return _parent;
    }

    ContourLine *ContourLine::get_parent() {
        assert(_is_hole && "Cannot get_parent of a non-hole");
        return _parent;
    }

    bool ContourLine::is_hole() const { return _is_hole; }

    void ContourLine::push_back(const XY &point) {
        if (empty() || point != back())
            std::vector<XY>::push_back(point);
    }

    void ContourLine::set_parent(ContourLine *parent) {
        assert(_is_hole && "Cannot set parent of a non-hole");
        assert(parent != 0 && "Null parent ContourLine");
        _parent = parent;
    }

    void ContourLine::write() const {
        std::cout << "ContourLine " << this << " of " << size() << " points:";
        for (const_iterator it = begin(); it != end(); ++it)
            std::cout << ' ' << *it;
        if (is_hole())
            std::cout << " hole, parent=" << get_parent();
        else {
            std::cout << " not hole";
            if (!_children.empty()) {
                std::cout << ", children=";
                for (Children::const_iterator it = _children.begin();
                     it != _children.end(); ++it)
                    std::cout << *it << ' ';
            }
        }
        std::cout << std::endl;
    }

    Contour::Contour() {}

    Contour::~Contour() { delete_contour_lines(); }

    void Contour::delete_contour_lines() {
        for (iterator line_it = begin(); line_it != end(); ++line_it) {
            delete *line_it;
            *line_it = 0;
        }
        std::vector<ContourLine *>::clear();
    }

    void Contour::write() const {
        std::cout << "Contour of " << size() << " lines." << std::endl;
        for (const_iterator it = begin(); it != end(); ++it)
            (*it)->write();
    }

    ParentCache::ParentCache(long nx, long x_chunk_points, long y_chunk_points)
        : _nx(nx), _x_chunk_points(x_chunk_points),
          _y_chunk_points(y_chunk_points),
          _lines(0), // Initialised when first needed.
          _istart(0), _jstart(0) {
        assert(_x_chunk_points > 0 && _y_chunk_points > 0 &&
               "Chunk sizes must be positive");
    }

    ContourLine *ParentCache::get_parent(long quad) {
        long index = quad_to_index(quad);
        ContourLine *parent = _lines[index];
        while (parent == 0) {
            index -= _x_chunk_points;
            assert(index >= 0 && "Failed to find parent in chunk ParentCache");
            parent = _lines[index];
        }
        assert(parent != 0 && "Failed to find parent in chunk ParentCache");
        return parent;
    }

    long ParentCache::quad_to_index(long quad) const {
        long i = quad % _nx;
        long j = quad / _nx;
        long index = (i - _istart) + (j - _jstart) * _x_chunk_points;

        assert(i >= _istart && i < _istart + _x_chunk_points &&
               "i-index outside chunk");
        assert(j >= _jstart && j < _jstart + _y_chunk_points &&
               "j-index outside chunk");
        assert(index >= 0 && index < static_cast<long>(_lines.size()) &&
               "ParentCache index outside chunk");

        return index;
    }

    void ParentCache::set_chunk_starts(long istart, long jstart) {
        assert(istart >= 0 && jstart >= 0 &&
               "Chunk start indices cannot be negative");
        _istart = istart;
        _jstart = jstart;
        if (_lines.empty())
            _lines.resize(_x_chunk_points * _y_chunk_points, 0);
        else
            std::fill(_lines.begin(), _lines.end(), (ContourLine *)0);
    }

    void ParentCache::set_parent(long quad, ContourLine &contour_line) {
        assert(!_lines.empty() &&
               "Accessing ParentCache before it has been initialised");
        long index = quad_to_index(quad);
        if (_lines[index] == 0)
            _lines[index] = (contour_line.is_hole() ? contour_line.get_parent()
                                                    : &contour_line);
    }

    QuadContourGenerator::QuadContourGenerator(const CoordinateArray &x,
                                               const CoordinateArray &y,
                                               const CoordinateArray &z,
                                               bool corner_mask,
                                               long chunk_size)
        : _x(x), _y(y), _z(z), _nx(static_cast<long>(_x[0].size())),
          _ny(static_cast<long>(_x.size())), _n(_nx * _ny),
          _corner_mask(corner_mask),
          _chunk_size(chunk_size > 0
                          ? std::min(chunk_size, std::max(_nx, _ny) - 1)
                          : std::max(_nx, _ny) - 1),
          _nxchunk(calc_chunk_count(_nx)), _nychunk(calc_chunk_count(_ny)),
          _chunk_count(_nxchunk * _nychunk), _cache(std::vector<CacheItem>(_n)),
          _parent_cache(_nx, chunk_size > 0 ? chunk_size + 1 : _nx,
                        chunk_size > 0 ? chunk_size + 1 : _ny) {
        assert(!_x.empty() && !_y.empty() && !_z.empty() && "Empty array");
        assert(_y.size() == _x.size() && _y[0].size() == _x[0].size() &&
               "Different-sized y and x arrays");
        assert(_z.size() == _x.size() && _z[0].size() == _x[0].size() &&
               "Different-sized z and x arrays");

        init_cache_grid();
    }

    void QuadContourGenerator::append_contour_line_to_vertices(
        ContourLine &contour_line, vertices_list_type &vertices_list) const {
        double x_diff = std::abs(_x[0][1] - _x[0][0]);
        double y_diff = std::abs(_y[1][0] - _y[0][0]);
        // Convert ContourLine to vertices_list
        size_t i = 0;
        size_t inserted = 0;
        for (ContourLine::const_iterator point = contour_line.begin();
             point != contour_line.end(); ++point, ++i) {
            bool is_origin = point->x == 0. && point->y == 0.;
            bool is_x_jump_to_origin =
                is_origin && inserted != 0 &&
                std::abs(vertices_list.first.back()) > 3 * x_diff;
            bool is_y_jump_to_origin =
                is_origin && inserted != 0 &&
                std::abs(vertices_list.second.back()) > 3 * y_diff;
            bool is_end_of_segment = is_x_jump_to_origin && is_y_jump_to_origin;
            if (!is_end_of_segment) {
                vertices_list.first.emplace_back(point->x);
                vertices_list.second.emplace_back(point->y);
                ++inserted;
            } else {
                bool last_segment_is_not_empty =
                    inserted > 0 && std::isfinite(vertices_list.first.back()) &&
                    std::isfinite(vertices_list.first.back());
                if (last_segment_is_not_empty) {
                    vertices_list.first.emplace_back(NaN);
                    vertices_list.second.emplace_back(NaN);
                }
            }
        }
        bool last_segment_is_not_empty =
            inserted > 0 && std::isfinite(vertices_list.first.back()) &&
            std::isfinite(vertices_list.first.back());
        if (last_segment_is_not_empty) {
            vertices_list.first.emplace_back(NaN);
            vertices_list.second.emplace_back(NaN);
        }
        contour_line.clear();
    }

    void QuadContourGenerator::append_contour_to_vertices_and_codes(
        Contour &contour, vertices_list_type &vertices_list,
        codes_list_type &codes_list) const {
        // Convert Contour to vertices and clear it.
        // For each line in the contour
        for (Contour::iterator line_it = contour.begin();
             line_it != contour.end(); ++line_it) {
            ContourLine &line = **line_it;
            if (line.is_hole()) {
                // If hole has already been converted to python its parent will
                // be set to 0 and it can be deleted.
                if (line.get_parent() != 0) {
                    delete *line_it;
                    *line_it = 0;
                }
            } else {
                // Non-holes are converted to python together with their child
                // holes so that they are rendered correctly.
                ContourLine::const_iterator point;
                ContourLine::Children::const_iterator children_it;

                const ContourLine::Children &children = line.get_children();
                // number of points in the line itself
                size_t npoints = static_cast<size_t>(line.size() + 1);
                // accumulate the number of points for each line child
                for (children_it = children.begin();
                     children_it != children.end(); ++children_it) {
                    npoints += static_cast<size_t>((*children_it)->size() + 1);
                }

                auto is_valid_point = [](double x, double y) {
                    return std::isfinite(x) && std::isfinite(y) && x != 0. &&
                           y != 0.;
                };

                // for each point in this line
                bool first = true;
                for (point = line.begin(); point != line.end(); ++point) {
                    if (is_valid_point(point->x, point->y)) {
                        vertices_list.first.emplace_back(point->x);
                        vertices_list.second.emplace_back(point->y);
                        codes_list.emplace_back(static_cast<unsigned char>(
                            first ? MOVETO : LINETO));
                        first = false;
                    }
                }

                // Close polygon
                // Emplace NaN instead of the first point again
                // Because that's how gnuplot works.
                // point = line.begin();
                // vertices_list.first.emplace_back(point->x);
                // vertices_list.second.emplace_back(point->y);
                if (!first) {
                    vertices_list.first.emplace_back(NaN);
                    vertices_list.second.emplace_back(NaN);
                    codes_list.emplace_back(
                        static_cast<unsigned char>(CLOSEPOLY));
                } else {
                    // We inserted no points
                    // ignore children (empty parents cannot have children)
                    for (children_it = children.begin();
                         children_it != children.end(); ++children_it) {
                        ContourLine &child = **children_it;
                        child.clear_parent();
                    }
                    // free parent
                    delete *line_it;
                    *line_it = 0;
                    // and go to next line
                    continue;
                }

                // Add the points from the children segments / holes
                for (children_it = children.begin();
                     children_it != children.end(); ++children_it) {
                    ContourLine &child = **children_it;
                    bool never_emplaced_anything = true;
                    for (point = child.begin(); point != child.end(); ++point) {
                        if (is_valid_point(point->x, point->y)) {
                            vertices_list.first.emplace_back(point->x);
                            vertices_list.second.emplace_back(point->y);
                            codes_list.emplace_back(static_cast<unsigned char>(
                                (never_emplaced_anything ? MOVETO : LINETO)));
                            never_emplaced_anything = false;
                        }
                    }

                    // Close the polygon
                    // Emplace NaN instead of the first point again
                    // That's how gnuplot works
                    // point = child.begin();
                    // vertices_list.first.emplace_back(point->x);
                    // vertices_list.second.emplace_back(point->y);
                    if (!never_emplaced_anything) {
                        vertices_list.first.emplace_back(NaN);
                        vertices_list.second.emplace_back(NaN);
                        codes_list.emplace_back(
                            static_cast<unsigned char>(CLOSEPOLY));
                    }

                    // To indicate the parent can be deleted.
                    child.clear_parent();
                }

                // Include extra NaNs after all children
                // This indicates the children are over
                // and that the next polygon is another parent
                vertices_list.first.emplace_back(NaN);
                vertices_list.second.emplace_back(NaN);
                codes_list.emplace_back(static_cast<unsigned char>(CLOSEPOLY));

                delete *line_it;
                *line_it = 0;
            }
        }

        // Delete remaining contour lines.
        contour.delete_contour_lines();
    }

    long QuadContourGenerator::calc_chunk_count(long point_count) const {
        assert(point_count > 0 && "point count must be positive");
        assert(_chunk_size > 0 && "Chunk size must be positive");

        if (_chunk_size > 0) {
            long count = (point_count - 1) / _chunk_size;
            if (count * _chunk_size < point_count - 1)
                ++count;

            assert(count >= 1 && "Invalid chunk count");
            return count;
        } else
            return 1;
    }

    QuadContourGenerator::vertices_list_type
    QuadContourGenerator::create_contour(const double &level) {
        init_cache_levels(level, level);

        vertices_list_type vertices_list;

        // Lines that start and end on boundaries.
        long ichunk, jchunk, istart, iend, jstart, jend;
        for (long ijchunk = 0; ijchunk < _chunk_count; ++ijchunk) {
            get_chunk_limits(ijchunk, ichunk, jchunk, istart, iend, jstart,
                             jend);

            for (long j = jstart; j < jend; ++j) {
                long quad_end = iend + j * _nx;
                for (long quad = istart + j * _nx; quad < quad_end; ++quad) {
                    if (EXISTS_NONE(quad) || VISITED(quad, 1))
                        continue;

                    if (BOUNDARY_S(quad) && Z_SW >= 1 && Z_SE < 1 &&
                        start_line(vertices_list, quad, Edge::Edge_S, level))
                        continue;

                    if (BOUNDARY_W(quad) && Z_NW >= 1 && Z_SW < 1 &&
                        start_line(vertices_list, quad, Edge::Edge_W, level))
                        continue;

                    if (BOUNDARY_N(quad) && Z_NE >= 1 && Z_NW < 1 &&
                        start_line(vertices_list, quad, Edge::Edge_N, level))
                        continue;

                    if (BOUNDARY_E(quad) && Z_SE >= 1 && Z_NE < 1 &&
                        start_line(vertices_list, quad, Edge::Edge_E, level))
                        continue;

                    if (_corner_mask) {
                        // Equates to NE boundary.
                        if (EXISTS_SW_CORNER(quad) && Z_SE >= 1 && Z_NW < 1 &&
                            start_line(vertices_list, quad, Edge::Edge_NE,
                                       level))
                            continue;

                        // Equates to NW boundary.
                        if (EXISTS_SE_CORNER(quad) && Z_NE >= 1 && Z_SW < 1 &&
                            start_line(vertices_list, quad, Edge::Edge_NW,
                                       level))
                            continue;

                        // Equates to SE boundary.
                        if (EXISTS_NW_CORNER(quad) && Z_SW >= 1 && Z_NE < 1 &&
                            start_line(vertices_list, quad, Edge::Edge_SE,
                                       level))
                            continue;

                        // Equates to SW boundary.
                        if (EXISTS_NE_CORNER(quad) && Z_NW >= 1 && Z_SE < 1 &&
                            start_line(vertices_list, quad, Edge::Edge_SW,
                                       level))
                            continue;
                    }
                }
            }
        }

        // Internal loops.
        ContourLine contour_line(false); // Reused for each contour line.
        for (long ijchunk = 0; ijchunk < _chunk_count; ++ijchunk) {
            get_chunk_limits(ijchunk, ichunk, jchunk, istart, iend, jstart,
                             jend);

            for (long j = jstart; j < jend; ++j) {
                long quad_end = iend + j * _nx;
                for (long quad = istart + j * _nx; quad < quad_end; ++quad) {
                    if (EXISTS_NONE(quad) || VISITED(quad, 1))
                        continue;

                    Edge start_edge = get_start_edge(quad, 1);
                    if (start_edge == Edge::Edge_None)
                        continue;

                    QuadEdge quad_edge(quad, start_edge);
                    QuadEdge start_quad_edge(quad_edge);

                    // To obtain output identical to that produced by legacy
                    // code, sometimes need to ignore the first point and add it
                    // on the end instead.
                    bool ignore_first = (start_edge == Edge::Edge_N);
                    follow_interior(contour_line, quad_edge, 1, level,
                                    !ignore_first, &start_quad_edge, 1, false);
                    if (ignore_first && !contour_line.empty())
                        contour_line.push_back(contour_line.front());
                    append_contour_line_to_vertices(contour_line,
                                                    vertices_list);

                    // Repeat if saddle point but not visited.
                    if (SADDLE(quad, 1) && !VISITED(quad, 1))
                        --quad;
                }
            }
        }

        return vertices_list;
    }

    std::pair<QuadContourGenerator::vertices_list_type,
              QuadContourGenerator::codes_list_type>
    QuadContourGenerator::create_filled_contour(const double &lower_level,
                                                const double &upper_level) {
        init_cache_levels(lower_level, upper_level);

        Contour contour;

        vertices_list_type vertices;
        codes_list_type codes;

        // for each chunk
        long ichunk, jchunk, istart, iend, jstart, jend;
        for (long ijchunk = 0; ijchunk < _chunk_count; ++ijchunk) {
            get_chunk_limits(ijchunk, ichunk, jchunk, istart, iend, jstart,
                             jend);
            _parent_cache.set_chunk_starts(istart, jstart);

            // iterate quads
            for (long j = jstart; j < jend; ++j) {
                long quad_end = iend + j * _nx;
                for (long quad = istart + j * _nx; quad < quad_end; ++quad) {
                    if (!EXISTS_NONE(quad))
                        single_quad_filled(contour, quad, lower_level,
                                           upper_level);
                }
            }

            // Clear VISITED_W and VISITED_S flags that are reused by later
            // chunks.
            if (jchunk < _nychunk - 1) {
                long quad_end = iend + jend * _nx;
                for (long quad = istart + jend * _nx; quad < quad_end; ++quad)
                    _cache[quad] &= ~MASK_VISITED_S;
            }

            if (ichunk < _nxchunk - 1) {
                long quad_end = iend + jend * _nx;
                for (long quad = iend + jstart * _nx; quad < quad_end;
                     quad += _nx)
                    _cache[quad] &= ~MASK_VISITED_W;
            }

            // Create python objects to return for this chunk.
            append_contour_to_vertices_and_codes(contour, vertices, codes);
        }

        return std::make_pair(vertices, codes);
    }

    XY QuadContourGenerator::edge_interp(const QuadEdge &quad_edge,
                                         const double &level) {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");
        return interp(get_edge_point_index(quad_edge, true),
                      get_edge_point_index(quad_edge, false), level);
    }

    unsigned int QuadContourGenerator::follow_boundary(
        ContourLine &contour_line, QuadEdge &quad_edge,
        const double &lower_level, const double &upper_level,
        unsigned int level_index, const QuadEdge &start_quad_edge) {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");
        assert(is_edge_a_boundary(quad_edge) && "Not a boundary edge");
        assert((level_index == 1 || level_index == 2) &&
               "level index must be 1 or 2");
        assert(start_quad_edge.quad >= 0 && start_quad_edge.quad < _n &&
               "Start quad index out of bounds");
        assert(start_quad_edge.edge != Edge::Edge_None && "Invalid start edge");

        // Only called for filled contours, so always updates _parent_cache.
        unsigned int end_level = 0;
        bool first_edge = true;
        bool stop = false;
        long &quad = quad_edge.quad;

        while (true) {
            // Levels of start and end points of quad_edge.
            unsigned int start_level =
                (first_edge ? Z_LEVEL(get_edge_point_index(quad_edge, true))
                            : end_level);
            long end_point = get_edge_point_index(quad_edge, false);
            end_level = Z_LEVEL(end_point);

            if (level_index == 1) {
                if (start_level <= level_index && end_level == 2) {
                    // Increasing z, switching levels from 1 to 2.
                    level_index = 2;
                    stop = true;
                } else if (start_level >= 1 && end_level == 0) {
                    // Decreasing z, keeping same level.
                    stop = true;
                }
            } else { // level_index == 2
                if (start_level <= level_index && end_level == 2) {
                    // Increasing z, keeping same level.
                    stop = true;
                } else if (start_level >= 1 && end_level == 0) {
                    // Decreasing z, switching levels from 2 to 1.
                    level_index = 1;
                    stop = true;
                }
            }

            if (!first_edge && !stop && quad_edge == start_quad_edge)
                // Return if reached start point of contour line.  Do this
                // before checking/setting VISITED flags as will already have
                // been visited.
                break;

            switch (quad_edge.edge) {
            case Edge::Edge_E:
                assert(!VISITED_W(quad + 1) && "Already visited");
                _cache[quad + 1] |= MASK_VISITED_W;
                break;
            case Edge::Edge_N:
                assert(!VISITED_S(quad + _nx) && "Already visited");
                _cache[quad + _nx] |= MASK_VISITED_S;
                break;
            case Edge::Edge_W:
                assert(!VISITED_W(quad) && "Already visited");
                _cache[quad] |= MASK_VISITED_W;
                break;
            case Edge::Edge_S:
                assert(!VISITED_S(quad) && "Already visited");
                _cache[quad] |= MASK_VISITED_S;
                break;
            case Edge::Edge_NE:
            case Edge::Edge_NW:
            case Edge::Edge_SW:
            case Edge::Edge_SE:
                assert(!VISITED_CORNER(quad) && "Already visited");
                _cache[quad] |= MASK_VISITED_CORNER;
                break;
            default:
                assert(0 && "Invalid Edge");
                break;
            }

            if (stop) {
                // Exiting boundary to enter interior.
                contour_line.push_back(edge_interp(
                    quad_edge, level_index == 1 ? lower_level : upper_level));
                break;
            }

            move_to_next_boundary_edge(quad_edge);

            // Just moved to new quad edge, so label parent of start of quad
            // edge.
            switch (quad_edge.edge) {
            case Edge::Edge_W:
            case Edge::Edge_SW:
            case Edge::Edge_S:
            case Edge::Edge_SE:
                if (!EXISTS_SE_CORNER(quad))
                    _parent_cache.set_parent(quad, contour_line);
                break;
            case Edge::Edge_E:
            case Edge::Edge_NE:
            case Edge::Edge_N:
            case Edge::Edge_NW:
                if (!EXISTS_SW_CORNER(quad))
                    _parent_cache.set_parent(quad + 1, contour_line);
                break;
            default:
                assert(0 && "Invalid edge");
                break;
            }

            // Add point to contour.
            contour_line.push_back(get_point_xy(end_point));

            if (first_edge)
                first_edge = false;
        }

        return level_index;
    }

    void QuadContourGenerator::follow_interior(
        ContourLine &contour_line, QuadEdge &quad_edge,
        unsigned int level_index, const double &level, bool want_initial_point,
        const QuadEdge *start_quad_edge, unsigned int start_level_index,
        bool set_parents) {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds.");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");
        assert((level_index == 1 || level_index == 2) &&
               "level index must be 1 or 2");
        assert((start_quad_edge == 0 ||
                (start_quad_edge->quad >= 0 && start_quad_edge->quad < _n)) &&
               "Start quad index out of bounds.");
        assert((start_quad_edge == 0 ||
                start_quad_edge->edge != Edge::Edge_None) &&
               "Invalid start edge");
        assert((start_level_index == 1 || start_level_index == 2) &&
               "start level index must be 1 or 2");

        long &quad = quad_edge.quad;
        Edge &edge = quad_edge.edge;

        if (want_initial_point)
            contour_line.push_back(edge_interp(quad_edge, level));

        CacheItem visited_mask =
            (level_index == 1 ? MASK_VISITED_1 : MASK_VISITED_2);
        CacheItem saddle_mask =
            (level_index == 1 ? MASK_SADDLE_1 : MASK_SADDLE_2);
        Dir dir = Dir::Dir_Straight;

        while (true) {
            assert(!EXISTS_NONE(quad) && "Quad does not exist");
            assert(!(_cache[quad] & visited_mask) && "Quad already visited");

            // Determine direction to move to next quad.  If the quad is already
            // labelled as a saddle quad then the direction is easily read from
            // the cache.  Otherwise the direction is determined differently
            // depending on whether the quad is a corner quad or not.

            if (_cache[quad] & saddle_mask) {
                // Already identified as a saddle quad, so direction is easy.
                dir = (SADDLE_LEFT(quad, level_index) ? Dir::Dir_Left
                                                      : Dir::Dir_Right);
                _cache[quad] |= visited_mask;
            } else if (EXISTS_ANY_CORNER(quad)) {
                // Need z-level of point opposite the entry edge, as that
                // determines whether contour turns left or right.
                long point_opposite = -1;
                switch (edge) {
                case Edge::Edge_E:
                    point_opposite =
                        (EXISTS_SE_CORNER(quad) ? POINT_SW : POINT_NW);
                    break;
                case Edge::Edge_N:
                    point_opposite =
                        (EXISTS_NW_CORNER(quad) ? POINT_SW : POINT_SE);
                    break;
                case Edge::Edge_W:
                    point_opposite =
                        (EXISTS_SW_CORNER(quad) ? POINT_SE : POINT_NE);
                    break;
                case Edge::Edge_S:
                    point_opposite =
                        (EXISTS_SW_CORNER(quad) ? POINT_NW : POINT_NE);
                    break;
                case Edge::Edge_NE:
                    point_opposite = POINT_SW;
                    break;
                case Edge::Edge_NW:
                    point_opposite = POINT_SE;
                    break;
                case Edge::Edge_SW:
                    point_opposite = POINT_NE;
                    break;
                case Edge::Edge_SE:
                    point_opposite = POINT_NW;
                    break;
                default:
                    assert(0 && "Invalid edge");
                    break;
                }
                assert(point_opposite != -1 && "Failed to find opposite point");

                // Lower-level polygons (level_index == 1) always have higher
                // values to the left of the contour.  Upper-level contours
                // (level_index == 2) are reversed, which is what the fancy XOR
                // does below.
                if ((Z_LEVEL(point_opposite) >= level_index) ^
                    (level_index == 2))
                    dir = Dir::Dir_Right;
                else
                    dir = Dir::Dir_Left;
                _cache[quad] |= visited_mask;
            } else {
                // Calculate configuration of this quad.
                long point_left = -1, point_right = -1;
                switch (edge) {
                case Edge::Edge_E:
                    point_left = POINT_SW;
                    point_right = POINT_NW;
                    break;
                case Edge::Edge_N:
                    point_left = POINT_SE;
                    point_right = POINT_SW;
                    break;
                case Edge::Edge_W:
                    point_left = POINT_NE;
                    point_right = POINT_SE;
                    break;
                case Edge::Edge_S:
                    point_left = POINT_NW;
                    point_right = POINT_NE;
                    break;
                default:
                    assert(0 && "Invalid edge");
                    break;
                }

                unsigned int config = (Z_LEVEL(point_left) >= level_index)
                                          << 1 |
                                      (Z_LEVEL(point_right) >= level_index);

                // Upper level (level_index == 2) polygons are reversed compared
                // to lower level ones, i.e. higher values on the right rather
                // than the left.
                if (level_index == 2)
                    config = 3 - config;

                // Calculate turn direction to move to next quad along contour
                // line.
                if (config == 1) {
                    // New saddle quad, set up cache bits for it.
                    double zmid =
                        0.25 * (get_point_z(POINT_SW) + get_point_z(POINT_SE) +
                                get_point_z(POINT_NW) + get_point_z(POINT_NE));
                    _cache[quad] |=
                        (level_index == 1 ? MASK_SADDLE_1 : MASK_SADDLE_2);
                    if ((zmid > level) ^ (level_index == 2)) {
                        dir = Dir::Dir_Right;
                    } else {
                        dir = Dir::Dir_Left;
                        _cache[quad] |= (level_index == 1 ? MASK_SADDLE_LEFT_1
                                                          : MASK_SADDLE_LEFT_2);
                    }
                    if (edge == Edge::Edge_N || edge == Edge::Edge_E) {
                        // Next visit to this quad must start on S or W.
                        _cache[quad] |=
                            (level_index == 1 ? MASK_SADDLE_START_SW_1
                                              : MASK_SADDLE_START_SW_2);
                    }
                } else {
                    // Normal (non-saddle) quad.
                    dir = (config == 0 ? Dir::Dir_Left
                                       : (config == 3 ? Dir::Dir_Right
                                                      : Dir::Dir_Straight));
                    _cache[quad] |= visited_mask;
                }
            }

            // Use dir to determine exit edge.
            edge = get_exit_edge(quad_edge, dir);

            if (set_parents) {
                if (edge == Edge::Edge_E)
                    _parent_cache.set_parent(quad + 1, contour_line);
                else if (edge == Edge::Edge_W)
                    _parent_cache.set_parent(quad, contour_line);
            }

            // Add new point to contour line.
            contour_line.push_back(edge_interp(quad_edge, level));

            // Stop if reached boundary.
            if (is_edge_a_boundary(quad_edge))
                break;

            move_to_next_quad(quad_edge);
            assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
                   "Quad index out of bounds");

            // Return if reached start point of contour line.
            if (start_quad_edge != 0 && quad_edge == *start_quad_edge &&
                level_index == start_level_index)
                break;
        }
    }

    void QuadContourGenerator::get_chunk_limits(long ijchunk, long &ichunk,
                                                long &jchunk, long &istart,
                                                long &iend, long &jstart,
                                                long &jend) {
        assert(ijchunk >= 0 && ijchunk < _chunk_count &&
               "ijchunk out of bounds");
        ichunk = ijchunk % _nxchunk;
        jchunk = ijchunk / _nxchunk;
        istart = ichunk * _chunk_size;
        iend = (ichunk == _nxchunk - 1 ? _nx : (ichunk + 1) * _chunk_size);
        jstart = jchunk * _chunk_size;
        jend = (jchunk == _nychunk - 1 ? _ny : (jchunk + 1) * _chunk_size);
    }

    Edge QuadContourGenerator::get_corner_start_edge(
        long quad, unsigned int level_index) const {
        assert(quad >= 0 && quad < _n && "Quad index out of bounds");
        assert((level_index == 1 || level_index == 2) &&
               "level index must be 1 or 2");
        assert(EXISTS_ANY_CORNER(quad) && "Quad is not a corner");

        // Diagram for NE corner.  Rotate for other corners.
        //
        //           edge12
        // point1 +---------+ point2
        //          \       |
        //            \     | edge23
        //       edge31 \   |
        //                \ |
        //                  + point3
        //
        long point1, point2, point3;
        Edge edge12, edge23, edge31;
        switch (_cache[quad] & MASK_EXISTS) {
        case MASK_EXISTS_SW_CORNER:
            point1 = POINT_SE;
            point2 = POINT_SW;
            point3 = POINT_NW;
            edge12 = Edge::Edge_S;
            edge23 = Edge::Edge_W;
            edge31 = Edge::Edge_NE;
            break;
        case MASK_EXISTS_SE_CORNER:
            point1 = POINT_NE;
            point2 = POINT_SE;
            point3 = POINT_SW;
            edge12 = Edge::Edge_E;
            edge23 = Edge::Edge_S;
            edge31 = Edge::Edge_NW;
            break;
        case MASK_EXISTS_NW_CORNER:
            point1 = POINT_SW;
            point2 = POINT_NW;
            point3 = POINT_NE;
            edge12 = Edge::Edge_W;
            edge23 = Edge::Edge_N;
            edge31 = Edge::Edge_SE;
            break;
        case MASK_EXISTS_NE_CORNER:
            point1 = POINT_NW;
            point2 = POINT_NE;
            point3 = POINT_SE;
            edge12 = Edge::Edge_N;
            edge23 = Edge::Edge_E;
            edge31 = Edge::Edge_SW;
            break;
        default:
            assert(0 && "Invalid EXISTS for quad");
            return Edge::Edge_None;
        }

        unsigned int config = (Z_LEVEL(point1) >= level_index) << 2 |
                              (Z_LEVEL(point2) >= level_index) << 1 |
                              (Z_LEVEL(point3) >= level_index);

        // Upper level (level_index == 2) polygons are reversed compared to
        // lower level ones, i.e. higher values on the right rather than the
        // left.
        if (level_index == 2)
            config = 7 - config;

        switch (config) {
        case 0:
            return Edge::Edge_None;
        case 1:
            return edge23;
        case 2:
            return edge12;
        case 3:
            return edge12;
        case 4:
            return edge31;
        case 5:
            return edge23;
        case 6:
            return edge31;
        case 7:
            return Edge::Edge_None;
        default:
            assert(0 && "Invalid config");
            return Edge::Edge_None;
        }
    }

    long QuadContourGenerator::get_edge_point_index(const QuadEdge &quad_edge,
                                                    bool start) const {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");

        // Edges are ordered anticlockwise around their quad, as indicated by
        // directions of arrows in diagrams below.
        //           Full quad                    NW corner (others similar)
        //
        //  POINT_NW   Edge_N   POINT_NE         POINT_NW   Edge_N   POINT_NE
        //          +----<-----+                         +----<-----+
        //          |          |                         |         /
        //          |          |                         | quad  /
        //  Edge_W  V   quad   ^  Edge_E         Edge_W  V     ^
        //          |          |                         |   /  Edge_SE
        //          |          |                         | /
        //          +---->-----+                         +
        //  POINT_SW   Edge_S   POINT_SE         POINT_SW
        //
        const long &quad = quad_edge.quad;
        switch (quad_edge.edge) {
        case Edge::Edge_E:
            return (start ? POINT_SE : POINT_NE);
        case Edge::Edge_N:
            return (start ? POINT_NE : POINT_NW);
        case Edge::Edge_W:
            return (start ? POINT_NW : POINT_SW);
        case Edge::Edge_S:
            return (start ? POINT_SW : POINT_SE);
        case Edge::Edge_NE:
            return (start ? POINT_SE : POINT_NW);
        case Edge::Edge_NW:
            return (start ? POINT_NE : POINT_SW);
        case Edge::Edge_SW:
            return (start ? POINT_NW : POINT_SE);
        case Edge::Edge_SE:
            return (start ? POINT_SW : POINT_NE);
        default:
            assert(0 && "Invalid edge");
            return 0;
        }
    }

    Edge QuadContourGenerator::get_exit_edge(const QuadEdge &quad_edge,
                                             Dir dir) const {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");

        const long &quad = quad_edge.quad;
        const Edge &edge = quad_edge.edge;
        if (EXISTS_ANY_CORNER(quad)) {
            // Corner directions are always left or right.  A corner is a
            // triangle, entered via one edge so the other two edges are the
            // left and right ones.
            switch (edge) {
            case Edge::Edge_E:
                return (
                    EXISTS_SE_CORNER(quad)
                        ? (dir == Dir::Dir_Left ? Edge::Edge_S : Edge::Edge_NW)
                        : (dir == Dir::Dir_Right ? Edge::Edge_N
                                                 : Edge::Edge_SW));
            case Edge::Edge_N:
                return (
                    EXISTS_NW_CORNER(quad)
                        ? (dir == Dir::Dir_Right ? Edge::Edge_W : Edge::Edge_SE)
                        : (dir == Dir::Dir_Left ? Edge::Edge_E
                                                : Edge::Edge_SW));
            case Edge::Edge_W:
                return (
                    EXISTS_SW_CORNER(quad)
                        ? (dir == Dir::Dir_Right ? Edge::Edge_S : Edge::Edge_NE)
                        : (dir == Dir::Dir_Left ? Edge::Edge_N
                                                : Edge::Edge_SE));
            case Edge::Edge_S:
                return (
                    EXISTS_SW_CORNER(quad)
                        ? (dir == Dir::Dir_Left ? Edge::Edge_W : Edge::Edge_NE)
                        : (dir == Dir::Dir_Right ? Edge::Edge_E
                                                 : Edge::Edge_NW));
            case Edge::Edge_NE:
                return (dir == Dir::Dir_Left ? Edge::Edge_S : Edge::Edge_W);
            case Edge::Edge_NW:
                return (dir == Dir::Dir_Left ? Edge::Edge_E : Edge::Edge_S);
            case Edge::Edge_SW:
                return (dir == Dir::Dir_Left ? Edge::Edge_N : Edge::Edge_E);
            case Edge::Edge_SE:
                return (dir == Dir::Dir_Left ? Edge::Edge_W : Edge::Edge_N);
            default:
                assert(0 && "Invalid edge");
                return Edge::Edge_None;
            }
        } else {
            // A full quad has four edges, entered via one edge so that other
            // three edges correspond to left, straight and right directions.
            switch (edge) {
            case Edge::Edge_E:
                return (dir == Dir::Dir_Left
                            ? Edge::Edge_S
                            : (dir == Dir::Dir_Right ? Edge::Edge_N
                                                     : Edge::Edge_W));
            case Edge::Edge_N:
                return (dir == Dir::Dir_Left
                            ? Edge::Edge_E
                            : (dir == Dir::Dir_Right ? Edge::Edge_W
                                                     : Edge::Edge_S));
            case Edge::Edge_W:
                return (dir == Dir::Dir_Left
                            ? Edge::Edge_N
                            : (dir == Dir::Dir_Right ? Edge::Edge_S
                                                     : Edge::Edge_E));
            case Edge::Edge_S:
                return (dir == Dir::Dir_Left
                            ? Edge::Edge_W
                            : (dir == Dir::Dir_Right ? Edge::Edge_E
                                                     : Edge::Edge_N));
            default:
                assert(0 && "Invalid edge");
                return Edge::Edge_None;
            }
        }
    }

    XY QuadContourGenerator::get_point_xy(long point) const {
        assert(point >= 0 && point < _n && "Point index out of bounds.");
        return XY(vector_2d_view::get_element_from_offset(
                      _x, static_cast<size_t>(point)),
                  vector_2d_view::get_element_from_offset(
                      _y, static_cast<size_t>(point)));
    }

    const double &QuadContourGenerator::get_point_z(long point) const {
        assert(point >= 0 && point < _n && "Point index out of bounds.");
        return vector_2d_view::get_element_from_offset(
            _z, static_cast<size_t>(point));
    }

    Edge
    QuadContourGenerator::get_quad_start_edge(long quad,
                                              unsigned int level_index) const {
        assert(quad >= 0 && quad < _n && "Quad index out of bounds");
        assert((level_index == 1 || level_index == 2) &&
               "level index must be 1 or 2");
        assert(EXISTS_QUAD(quad) && "Quad does not exist");

        unsigned int config =
            (Z_NW >= level_index) << 3 | (Z_NE >= level_index) << 2 |
            (Z_SW >= level_index) << 1 | (Z_SE >= level_index);

        // Upper level (level_index == 2) polygons are reversed compared to
        // lower level ones, i.e. higher values on the right rather than the
        // left.
        if (level_index == 2)
            config = 15 - config;

        switch (config) {
        case 0:
            return Edge::Edge_None;
        case 1:
            return Edge::Edge_E;
        case 2:
            return Edge::Edge_S;
        case 3:
            return Edge::Edge_E;
        case 4:
            return Edge::Edge_N;
        case 5:
            return Edge::Edge_N;
        case 6:
            // If already identified as a saddle quad then the start edge is
            // read from the cache.  Otherwise return either valid start edge
            // and the subsequent call to follow_interior() will correctly set
            // up saddle bits in cache.
            if (!SADDLE(quad, level_index) ||
                SADDLE_START_SW(quad, level_index))
                return Edge::Edge_S;
            else
                return Edge::Edge_N;
        case 7:
            return Edge::Edge_N;
        case 8:
            return Edge::Edge_W;
        case 9:
            // See comment for 6 above.
            if (!SADDLE(quad, level_index) ||
                SADDLE_START_SW(quad, level_index))
                return Edge::Edge_W;
            else
                return Edge::Edge_E;
        case 10:
            return Edge::Edge_S;
        case 11:
            return Edge::Edge_E;
        case 12:
            return Edge::Edge_W;
        case 13:
            return Edge::Edge_W;
        case 14:
            return Edge::Edge_S;
        case 15:
            return Edge::Edge_None;
        default:
            assert(0 && "Invalid config");
            return Edge::Edge_None;
        }
    }

    Edge QuadContourGenerator::get_start_edge(long quad,
                                              unsigned int level_index) const {
        if (EXISTS_ANY_CORNER(quad))
            return get_corner_start_edge(quad, level_index);
        else
            return get_quad_start_edge(quad, level_index);
    }

    void QuadContourGenerator::init_cache_grid() {
        long i, j, quad;

        quad = 0;
        for (j = 0; j < _ny; ++j) {
            for (i = 0; i < _nx; ++i, ++quad) {
                _cache[quad] = 0;

                if (i < _nx - 1 && j < _ny - 1)
                    _cache[quad] |= MASK_EXISTS_QUAD;

                if ((i % _chunk_size == 0 || i == _nx - 1) && j < _ny - 1)
                    _cache[quad] |= MASK_BOUNDARY_W;

                if ((j % _chunk_size == 0 || j == _ny - 1) && i < _nx - 1)
                    _cache[quad] |= MASK_BOUNDARY_S;
            }
        }
    }

    void QuadContourGenerator::init_cache_levels(const double &lower_level,
                                                 const double &upper_level) {
        assert(upper_level >= lower_level &&
               "upper and lower levels are wrong way round");

        bool two_levels = (lower_level != upper_level);
        CacheItem keep_mask =
            (_corner_mask
                 ? MASK_EXISTS | MASK_BOUNDARY_S | MASK_BOUNDARY_W
                 : MASK_EXISTS_QUAD | MASK_BOUNDARY_S | MASK_BOUNDARY_W);
        auto z_view = vector_2d_view::from_vector_2d(_z);
        if (two_levels) {

            auto z_ptr = z_view.begin();
            for (long quad = 0; quad < _n; ++quad, ++z_ptr) {
                _cache[quad] &= keep_mask;
                if (*z_ptr > upper_level)
                    _cache[quad] |= MASK_Z_LEVEL_2;
                else if (*z_ptr > lower_level)
                    _cache[quad] |= MASK_Z_LEVEL_1;
            }
        } else {
            auto z_ptr = z_view.begin();
            for (long quad = 0; quad < _n; ++quad, ++z_ptr) {
                _cache[quad] &= keep_mask;
                if (*z_ptr > lower_level)
                    _cache[quad] |= MASK_Z_LEVEL_1;
            }
        }
    }

    XY QuadContourGenerator::interp(long point1, long point2,
                                    const double &level) const {
        assert(point1 >= 0 && point1 < _n && "Point index 1 out of bounds.");
        assert(point2 >= 0 && point2 < _n && "Point index 2 out of bounds.");
        assert(point1 != point2 && "Identical points");
        double fraction = (get_point_z(point2) - level) /
                          (get_point_z(point2) - get_point_z(point1));
        return get_point_xy(point1) * fraction +
               get_point_xy(point2) * (1.0 - fraction);
    }

    bool
    QuadContourGenerator::is_edge_a_boundary(const QuadEdge &quad_edge) const {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");

        switch (quad_edge.edge) {
        case Edge::Edge_E:
            return BOUNDARY_E(quad_edge.quad);
        case Edge::Edge_N:
            return BOUNDARY_N(quad_edge.quad);
        case Edge::Edge_W:
            return BOUNDARY_W(quad_edge.quad);
        case Edge::Edge_S:
            return BOUNDARY_S(quad_edge.quad);
        case Edge::Edge_NE:
            return EXISTS_SW_CORNER(quad_edge.quad);
        case Edge::Edge_NW:
            return EXISTS_SE_CORNER(quad_edge.quad);
        case Edge::Edge_SW:
            return EXISTS_NE_CORNER(quad_edge.quad);
        case Edge::Edge_SE:
            return EXISTS_NW_CORNER(quad_edge.quad);
        default:
            assert(0 && "Invalid edge");
            return true;
        }
    }

    void QuadContourGenerator::move_to_next_boundary_edge(
        QuadEdge &quad_edge) const {
        assert(is_edge_a_boundary(quad_edge) && "QuadEdge is not a boundary");

        long &quad = quad_edge.quad;
        Edge &edge = quad_edge.edge;

        quad = get_edge_point_index(quad_edge, false);

        // quad is now such that POINT_SW is the end point of the quad_edge
        // passed to this function.

        // To find the next boundary edge, first attempt to turn left 135
        // degrees and if that edge is a boundary then move to it.  If not,
        // attempt to turn left 90 degrees, then left 45 degrees, then straight
        // on, etc, until can move. First determine which edge to attempt first.
        int index = 0;
        switch (edge) {
        case Edge::Edge_E:
            index = 0;
            break;
        case Edge::Edge_SE:
            index = 1;
            break;
        case Edge::Edge_S:
            index = 2;
            break;
        case Edge::Edge_SW:
            index = 3;
            break;
        case Edge::Edge_W:
            index = 4;
            break;
        case Edge::Edge_NW:
            index = 5;
            break;
        case Edge::Edge_N:
            index = 6;
            break;
        case Edge::Edge_NE:
            index = 7;
            break;
        default:
            assert(0 && "Invalid edge");
            break;
        }

        // If _corner_mask not set, only need to consider odd index in loop
        // below.
        if (!_corner_mask)
            ++index;

        // Try each edge in turn until a boundary is found.
        int start_index = index;
        do {
            switch (index) {
            case 0:
                if (EXISTS_SE_CORNER(quad - _nx -
                                     1)) { // Equivalent to BOUNDARY_NW
                    quad -= _nx + 1;
                    edge = Edge::Edge_NW;
                    return;
                }
                break;
            case 1:
                if (BOUNDARY_N(quad - _nx - 1)) {
                    quad -= _nx + 1;
                    edge = Edge::Edge_N;
                    return;
                }
                break;
            case 2:
                if (EXISTS_SW_CORNER(quad - 1)) { // Equivalent to BOUNDARY_NE
                    quad -= 1;
                    edge = Edge::Edge_NE;
                    return;
                }
                break;
            case 3:
                if (BOUNDARY_E(quad - 1)) {
                    quad -= 1;
                    edge = Edge::Edge_E;
                    return;
                }
                break;
            case 4:
                if (EXISTS_NW_CORNER(quad)) { // Equivalent to BOUNDARY_SE
                    edge = Edge::Edge_SE;
                    return;
                }
                break;
            case 5:
                if (BOUNDARY_S(quad)) {
                    edge = Edge::Edge_S;
                    return;
                }
                break;
            case 6:
                if (EXISTS_NE_CORNER(quad - _nx)) { // Equivalent to BOUNDARY_SW
                    quad -= _nx;
                    edge = Edge::Edge_SW;
                    return;
                }
                break;
            case 7:
                if (BOUNDARY_W(quad - _nx)) {
                    quad -= _nx;
                    edge = Edge::Edge_W;
                    return;
                }
                break;
            default:
                assert(0 && "Invalid index");
                break;
            }

            if (_corner_mask)
                index = (index + 1) % 8;
            else
                index = (index + 2) % 8;
        } while (index != start_index);

        assert(0 && "Failed to find next boundary edge");
    }

    void QuadContourGenerator::move_to_next_quad(QuadEdge &quad_edge) const {
        assert(quad_edge.quad >= 0 && quad_edge.quad < _n &&
               "Quad index out of bounds");
        assert(quad_edge.edge != Edge::Edge_None && "Invalid edge");

        // Move from quad_edge.quad to the neighbouring quad in the direction
        // specified by quad_edge.edge.
        switch (quad_edge.edge) {
        case Edge::Edge_E:
            quad_edge.quad += 1;
            quad_edge.edge = Edge::Edge_W;
            break;
        case Edge::Edge_N:
            quad_edge.quad += _nx;
            quad_edge.edge = Edge::Edge_S;
            break;
        case Edge::Edge_W:
            quad_edge.quad -= 1;
            quad_edge.edge = Edge::Edge_E;
            break;
        case Edge::Edge_S:
            quad_edge.quad -= _nx;
            quad_edge.edge = Edge::Edge_N;
            break;
        default:
            assert(0 && "Invalid edge");
            break;
        }
    }

    void QuadContourGenerator::single_quad_filled(Contour &contour, long quad,
                                                  const double &lower_level,
                                                  const double &upper_level) {
        assert(quad >= 0 && quad < _n && "Quad index out of bounds");

        // Order of checking is important here as can have different
        // ContourLines from both lower and upper levels in the same quad. First
        // check the S edge, then move up the quad to the N edge checking as
        // required.

        // Possible starts from S boundary.
        if (BOUNDARY_S(quad) && EXISTS_S_EDGE(quad)) {

            // Lower-level start from S boundary into interior.
            if (!VISITED_S(quad) && Z_SW >= 1 && Z_SE == 0)
                contour.push_back(start_filled(
                    quad, Edge::Edge_S, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from S boundary into interior.
            if (!VISITED_S(quad) && Z_SW < 2 && Z_SE == 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_S, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start following S boundary from W to E.
            if (!VISITED_S(quad) && Z_SW <= 1 && Z_SE == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_S, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));

            // Upper-level start following S boundary from W to E.
            if (!VISITED_S(quad) && Z_SW == 2 && Z_SE == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_S, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));
        }

        // Possible starts from W boundary.
        if (BOUNDARY_W(quad) && EXISTS_W_EDGE(quad)) {

            // Lower-level start from W boundary into interior.
            if (!VISITED_W(quad) && Z_NW >= 1 && Z_SW == 0)
                contour.push_back(start_filled(
                    quad, Edge::Edge_W, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from W boundary into interior.
            if (!VISITED_W(quad) && Z_NW < 2 && Z_SW == 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_W, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start following W boundary from N to S.
            if (!VISITED_W(quad) && Z_NW <= 1 && Z_SW == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_W, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));

            // Upper-level start following W boundary from N to S.
            if (!VISITED_W(quad) && Z_NW == 2 && Z_SW == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_W, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));
        }

        // Possible starts from NE boundary.
        if (EXISTS_SW_CORNER(quad)) { // i.e. BOUNDARY_NE

            // Lower-level start following NE boundary from SE to NW, hole.
            if (!VISITED_CORNER(quad) && Z_NW == 1 && Z_SE == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_NE, 1, HoleOrNot::Hole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));
        }
        // Possible starts from SE boundary.
        else if (EXISTS_NW_CORNER(quad)) { // i.e. BOUNDARY_SE

            // Lower-level start from N to SE.
            if (!VISITED(quad, 1) && Z_NW == 0 && Z_SW == 0 && Z_NE >= 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_N, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from SE to N, hole.
            if (!VISITED(quad, 2) && Z_NW < 2 && Z_SW < 2 && Z_NE == 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SE, 2, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from N to SE.
            if (!VISITED(quad, 2) && Z_NW == 2 && Z_SW == 2 && Z_NE < 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_N, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start from SE to N, hole.
            if (!VISITED(quad, 1) && Z_NW >= 1 && Z_SW >= 1 && Z_NE == 0)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SE, 1, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));
        }
        // Possible starts from NW boundary.
        else if (EXISTS_SE_CORNER(quad)) { // i.e. BOUNDARY_NW

            // Lower-level start from NW to E.
            if (!VISITED(quad, 1) && Z_SW == 0 && Z_SE == 0 && Z_NE >= 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_NW, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from E to NW, hole.
            if (!VISITED(quad, 2) && Z_SW < 2 && Z_SE < 2 && Z_NE == 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_E, 2, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from NW to E.
            if (!VISITED(quad, 2) && Z_SW == 2 && Z_SE == 2 && Z_NE < 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_NW, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start from E to NW, hole.
            if (!VISITED(quad, 1) && Z_SW >= 1 && Z_SE >= 1 && Z_NE == 0)
                contour.push_back(start_filled(
                    quad, Edge::Edge_E, 1, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));
        }
        // Possible starts from SW boundary.
        else if (EXISTS_NE_CORNER(quad)) { // i.e. BOUNDARY_SW

            // Lower-level start from SW boundary into interior.
            if (!VISITED_CORNER(quad) && Z_NW >= 1 && Z_SE == 0)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SW, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from SW boundary into interior.
            if (!VISITED_CORNER(quad) && Z_NW < 2 && Z_SE == 2)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SW, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start following SW boundary from NW to SE.
            if (!VISITED_CORNER(quad) && Z_NW <= 1 && Z_SE == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SW, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));

            // Upper-level start following SW boundary from NW to SE.
            if (!VISITED_CORNER(quad) && Z_NW == 2 && Z_SE == 1)
                contour.push_back(start_filled(
                    quad, Edge::Edge_SW, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Boundary, lower_level, upper_level));
        }

        // A full (unmasked) quad can only have a start on the NE corner, i.e.
        // from N to E (lower level) or E to N (upper level).  Any other start
        // will have already been created in a call to this function for a prior
        // quad so we don't need to test for it again here.
        //
        // The situation is complicated by the possibility that the quad is a
        // saddle quad, in which case a contour line starting on the N could
        // leave by either the W or the E.  We only need to consider those
        // leaving E.
        //
        // A NE corner can also have a N to E or E to N start.
        if (EXISTS_QUAD(quad) || EXISTS_NE_CORNER(quad)) {

            // Lower-level start from N to E.
            if (!VISITED(quad, 1) && Z_NW == 0 && Z_SE == 0 && Z_NE >= 1 &&
                (!SADDLE(quad, 1) || SADDLE_LEFT(quad, 1)))
                contour.push_back(start_filled(
                    quad, Edge::Edge_N, 1, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from E to N, hole.
            if (!VISITED(quad, 2) && Z_NW < 2 && Z_SE < 2 && Z_NE == 2 &&
                (!SADDLE(quad, 2) || !SADDLE_LEFT(quad, 2)))
                contour.push_back(start_filled(
                    quad, Edge::Edge_E, 2, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Upper-level start from N to E.
            if (!VISITED(quad, 2) && Z_NW == 2 && Z_SE == 2 && Z_NE < 2 &&
                (!SADDLE(quad, 2) || SADDLE_LEFT(quad, 2)))
                contour.push_back(start_filled(
                    quad, Edge::Edge_N, 2, HoleOrNot::NotHole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // Lower-level start from E to N, hole.
            if (!VISITED(quad, 1) && Z_NW >= 1 && Z_SE >= 1 && Z_NE == 0 &&
                (!SADDLE(quad, 1) || !SADDLE_LEFT(quad, 1)))
                contour.push_back(start_filled(
                    quad, Edge::Edge_E, 1, HoleOrNot::Hole,
                    BoundaryOrInterior::Interior, lower_level, upper_level));

            // All possible contours passing through the interior of this quad
            // should have already been created, so assert this.
            assert(
                (VISITED(quad, 1) ||
                 get_start_edge(quad, 1) == Edge::Edge_None) &&
                "Found start of contour that should have already been created");
            assert(
                (VISITED(quad, 2) ||
                 get_start_edge(quad, 2) == Edge::Edge_None) &&
                "Found start of contour that should have already been created");
        }

        // Lower-level start following N boundary from E to W, hole.
        // This is required for an internal masked region which is a hole in a
        // surrounding contour line.
        if (BOUNDARY_N(quad) && EXISTS_N_EDGE(quad) && !VISITED_S(quad + _nx) &&
            Z_NW == 1 && Z_NE == 1)
            contour.push_back(start_filled(
                quad, Edge::Edge_N, 1, HoleOrNot::Hole,
                BoundaryOrInterior::Boundary, lower_level, upper_level));
    }

    ContourLine *QuadContourGenerator::start_filled(
        long quad, Edge edge, unsigned int start_level_index,
        HoleOrNot hole_or_not, BoundaryOrInterior boundary_or_interior,
        const double &lower_level, const double &upper_level) {
        assert(quad >= 0 && quad < _n && "Quad index out of bounds");
        assert(edge != Edge::Edge_None && "Invalid edge");
        assert((start_level_index == 1 || start_level_index == 2) &&
               "start level index must be 1 or 2");

        ContourLine *contour_line =
            new ContourLine(hole_or_not == HoleOrNot::Hole);
        if (hole_or_not == HoleOrNot::Hole) {
            // Find and set parent ContourLine.
            ContourLine *parent = _parent_cache.get_parent(quad + 1);
            assert(parent != 0 && "Failed to find parent ContourLine");
            contour_line->set_parent(parent);
            parent->add_child(contour_line);
        }

        QuadEdge quad_edge(quad, edge);
        const QuadEdge start_quad_edge(quad_edge);
        unsigned int level_index = start_level_index;

        // If starts on interior, can only finish on interior.
        // If starts on boundary, can only finish on boundary.

        while (true) {
            if (boundary_or_interior == BoundaryOrInterior::Interior) {
                double level = (level_index == 1 ? lower_level : upper_level);
                follow_interior(*contour_line, quad_edge, level_index, level,
                                false, &start_quad_edge, start_level_index,
                                true);
            } else {
                level_index =
                    follow_boundary(*contour_line, quad_edge, lower_level,
                                    upper_level, level_index, start_quad_edge);
            }

            if (quad_edge == start_quad_edge &&
                (boundary_or_interior == BoundaryOrInterior::Boundary ||
                 level_index == start_level_index))
                break;

            if (boundary_or_interior == BoundaryOrInterior::Boundary)
                boundary_or_interior = BoundaryOrInterior::Interior;
            else
                boundary_or_interior = BoundaryOrInterior::Boundary;
        }

        return contour_line;
    }

    bool QuadContourGenerator::start_line(vertices_list_type &vertices_list,
                                          long quad, Edge edge,
                                          const double &level) {
        assert(is_edge_a_boundary(QuadEdge(quad, edge)) &&
               "QuadEdge is not a boundary");

        QuadEdge quad_edge(quad, edge);
        ContourLine contour_line(false);
        follow_interior(contour_line, quad_edge, 1, level, true, 0, 1, false);
        append_contour_line_to_vertices(contour_line, vertices_list);
        return VISITED(quad, 1);
    }

    void QuadContourGenerator::write_cache(bool grid_only) const {
        std::cout << "-----------------------------------------------"
                  << std::endl;
        for (long quad = 0; quad < _n; ++quad)
            write_cache_quad(quad, grid_only);
        std::cout << "-----------------------------------------------"
                  << std::endl;
    }

    void QuadContourGenerator::write_cache_quad(long quad,
                                                bool grid_only) const {
        long j = quad / _nx;
        long i = quad - j * _nx;
        std::cout << quad << ": i=" << i << " j=" << j
                  << " EXISTS=" << EXISTS_QUAD(quad);
        if (_corner_mask)
            std::cout << " CORNER=" << EXISTS_SW_CORNER(quad)
                      << EXISTS_SE_CORNER(quad) << EXISTS_NW_CORNER(quad)
                      << EXISTS_NE_CORNER(quad);
        std::cout << " BNDY=" << BOUNDARY_S(quad) << BOUNDARY_W(quad);
        if (!grid_only) {
            std::cout << " Z=" << Z_LEVEL(quad) << " SAD=" << SADDLE(quad, 1)
                      << SADDLE(quad, 2) << " LEFT=" << SADDLE_LEFT(quad, 1)
                      << SADDLE_LEFT(quad, 2)
                      << " NW=" << SADDLE_START_SW(quad, 1)
                      << SADDLE_START_SW(quad, 2) << " VIS=" << VISITED(quad, 1)
                      << VISITED(quad, 2) << VISITED_S(quad) << VISITED_W(quad)
                      << VISITED_CORNER(quad);
        }
        std::cout << std::endl;
    }

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
            QuadContourGenerator contour_generator(X, Y, Z, false, 0);
            auto c = contour_generator.create_contour(level);
            size_t attempts = 0;
            while (!contour_is_in_bounds(c) && attempts < 10) {
                std::cerr << "Contour out of bounds" << std::endl;
                QuadContourGenerator contour_generator2(X, Y, Z, false, 0);
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
        auto [x, y] = meshgrid(iota(1., 1., static_cast<double>(z[0].size())),
                               iota(1., 1., static_cast<double>(z.size())));
        return contourc(x, y, z, levels);
    }

    std::vector<contour_line_type> contourc(const vector_2d &z,
                                            size_t n_levels) {
        auto [x, y] = meshgrid(iota(1., 1., static_cast<double>(z[0].size())),
                               iota(1., 1., static_cast<double>(z.size())));
        return contourc(x, y, z, n_levels);
    }

} // namespace matplot
