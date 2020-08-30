#pragma once
#include <cmath>
#include <functional>
#include <vector>

namespace nodesoup {

/** Simple adjaceny list graph structure */

using vertex_id_t = std::size_t;
using adj_list_t = std::vector<std::vector<vertex_id_t>>;

/** Algebra types */

struct Vector2D;

struct Point2D {
    double x;
    double y;

    explicit operator Vector2D() const;
    Point2D& operator+=(const Vector2D& vector);
    Point2D& operator-=(const Vector2D& vector);
};

struct Vector2D {
    double dx;
    double dy;

    double norm() const {
        return sqrt(dx * dx + dy * dy);
    }

    explicit operator Point2D() const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(double scalar);
    Vector2D& operator/=(double scalar);
};

/** Main library functions */

using iter_callback_t = std::function<void(const std::vector<Point2D>&, int)>;

/**
Applies the Freuchterman Reingold algorithm to layout graph @p in a frame of dimensions
@p width and @p height, in @p iter-count iterations
*/
std::vector<Point2D> fruchterman_reingold(
    const adj_list_t& g,
    unsigned int width,
    unsigned int height,
    unsigned int iters_count = 300,
    double k = 15.0,
    iter_callback_t iter_cb = nullptr);

std::vector<Point2D> kamada_kawai(
    const adj_list_t& g,
    unsigned int width,
    unsigned int height,
    double k = 300.0,
    double energy_threshold = 1e-2);

/** Assigns diameters to vertices based on their degree */
std::vector<double> size_radiuses(const adj_list_t& g, double min_radius = 4.0, double k = 300.0);
}
