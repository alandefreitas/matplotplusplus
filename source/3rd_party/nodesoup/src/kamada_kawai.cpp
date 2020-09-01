#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

#include "algebra.hpp"
#include "kamada_kawai.hpp"

namespace nodesoup {

using std::vector;

KamadaKawai::KamadaKawai(const adj_list_t& g, double k, double energy_threshold)
    : g_(g)
    , energy_threshold_(energy_threshold) {
    vector<vector<vertex_id_t>> distances = floyd_warshall_(g_);

    // find biggest distance
    unsigned int biggest_distance = 0;
    for (vertex_id_t v_id = 0; v_id < g_.size(); v_id++) {
        for (vertex_id_t other_id = 0; other_id < g_.size(); other_id++) {
            if (distances[v_id][other_id] > biggest_distance) {
                biggest_distance = static_cast<unsigned int>(distances[v_id][other_id]);
            }
        }
    }

    // Ideal length for all edges. we don't really care, the layout is going to be scaled.
    // Let's chose 1.0 as the initial positions will be on a 1.0 radius circle, so we're
    // on the same order of magnitude
    double length = 1.0 / biggest_distance;

    // init springs lengths and strengths matrices
    for (vertex_id_t v_id = 0; v_id < g_.size(); v_id++) {
        vector<Spring> v_springs;

        for (vertex_id_t other_id = 0; other_id < g_.size(); other_id++) {
            Spring spring;
            if (v_id == other_id) {
                spring.length = 0.0;
                spring.strength = 0.0;
            } else {
                unsigned int distance = static_cast<unsigned int>(distances[v_id][other_id]);
                spring.length = distance * length;
                spring.strength = k / (distance * distance);
            }

            v_springs.push_back(spring);
        }
        springs_.push_back(v_springs);
    }
}

vector<vector<vertex_id_t>> KamadaKawai::floyd_warshall_(const adj_list_t& g) {
    // build adjacency matrix (infinity = no edge, 1 = edge)
    unsigned int infinity = std::numeric_limits<unsigned int>::max() / 2;
    vector<vector<vertex_id_t>> distances(g.size(), vector<vertex_id_t>(g.size(), infinity));

    for (vertex_id_t v_id = 0; v_id < g.size(); v_id++) {
        distances[v_id][v_id] = 0;
        for (vertex_id_t adj_id : g[v_id]) {
            if (adj_id > v_id) {
                distances[v_id][adj_id] = 1;
                distances[adj_id][v_id] = 1;
            }
        }
    }

    // floyd warshall itself, find length of shortest path for each pair of vertices
    for (vertex_id_t k = 0; k < g.size(); k++) {
        for (vertex_id_t i = 0; i < g.size(); i++) {
            for (vertex_id_t j = 0; j < g.size(); j++) {
                distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }

    return distances;
}

#define MAX_VERTEX_ITERS_COUNT 50

/**
Reduce the energy of the next vertex with most energy until all the vertices have
a energy below energy_threshold
*/
void KamadaKawai::operator()(vector<Point2D>& positions) const {
    vertex_id_t v_id;
    while (find_max_vertex_energy_(positions, v_id) > energy_threshold_) {
        // move vertex step by step until its energy goes below threshold
        // (apparently this is equivalent to the newton raphson method)
        unsigned int count = 0;
        do {
            positions[v_id] = compute_next_vertex_position_(v_id, positions);
            count++;
        } while (compute_vertex_energy_(v_id, positions) > energy_threshold_ && count < MAX_VERTEX_ITERS_COUNT);
    }
}

/**
Find @p max_energy_v_id with the most potential energy and @return its energy
// https://gist.github.com/terakun/b7eff90c889c1485898ec9256ca9f91d
*/
double KamadaKawai::find_max_vertex_energy_(const vector<Point2D>& positions, vertex_id_t& max_energy_v_id) const {
    double max_energy = -1.0;
    for (vertex_id_t v_id = 0; v_id < g_.size(); v_id++) {
        double energy = compute_vertex_energy_(v_id, positions);
        if (energy > max_energy) {
            max_energy_v_id = v_id;
            max_energy = energy;
        }
    }
    assert(max_energy != -1.0);
    return max_energy;
}

/** @return the potential energies of springs between @p v_id and all other vertices */
double KamadaKawai::compute_vertex_energy_(vertex_id_t v_id, const vector<Point2D>& positions) const {
    double x_energy = 0.0;
    double y_energy = 0.0;

    for (vertex_id_t other_id = 0; other_id < g_.size(); other_id++) {
        if (v_id == other_id) {
            continue;
        }

        Vector2D delta = positions[v_id] - positions[other_id];
        double distance = delta.norm();

        // delta * k * (1 - l / distance)
        Spring spring = springs_[v_id][other_id];
        x_energy += delta.dx * spring.strength * (1.0 - spring.length / distance);
        y_energy += delta.dy * spring.strength * (1.0 - spring.length / distance);
    }

    return sqrt(x_energy * x_energy + y_energy * y_energy);
}

/**
@returns next position for @param v_id reducing its potential energy, ie the energy in the whole graph
caused by its position.
The position's delta depends on K (TODO bigger K = faster?).
This is the complicated part of the algorithm.
*/
Point2D KamadaKawai::compute_next_vertex_position_(vertex_id_t v_id, const vector<Point2D>& positions) const {
    double xx_energy = 0.0, xy_energy = 0.0, yx_energy = 0.0, yy_energy = 0.0;
    double x_energy = 0.0, y_energy = 0.0;

    for (vertex_id_t other_id = 0; other_id < g_.size(); other_id++) {
        if (v_id == other_id) {
            continue;
        }

        Vector2D delta = positions[v_id] - positions[other_id];
        double distance = delta.norm();
        double cubed_distance = distance * distance * distance;

        Spring spring = springs_[v_id][other_id];

        x_energy += delta.dx * spring.strength * (1.0 - spring.length / distance);
        y_energy += delta.dy * spring.strength * (1.0 - spring.length / distance);
        xy_energy += spring.strength * spring.length * delta.dx * delta.dy / cubed_distance;
        xx_energy += spring.strength * (1.0 - spring.length * delta.dy * delta.dy / cubed_distance);
        yy_energy += spring.strength * (1.0 - spring.length * delta.dx * delta.dx / cubed_distance);
    }
    yx_energy = xy_energy;

    Point2D position = positions[v_id];
    double denom = xx_energy * yy_energy - xy_energy * yx_energy;
    position.x += (xy_energy * y_energy - yy_energy * x_energy) / denom;
    position.y += (xy_energy * x_energy - xx_energy * y_energy) / denom;

    return position;
}
}
