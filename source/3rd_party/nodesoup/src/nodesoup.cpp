#include "nodesoup.hpp"
#include "fruchterman_reingold.hpp"
#include "kamada_kawai.hpp"
#include "layout.hpp"
#include <algorithm>
#include <cmath>

namespace nodesoup {

using std::vector;

vector<Point2D> fruchterman_reingold(
    const adj_list_t& g,
    unsigned int width,
    unsigned int height,
    unsigned int iters_count,
    double k,
    iter_callback_t iter_cb) {
    vector<Point2D> positions(g.size());
    // Initial layout on a circle
    circle(g, positions);

    FruchtermanReingold fr(g, k);
    for (unsigned int i = 0; i < iters_count; i++) {
        fr(positions);

        if (iter_cb != nullptr) {
            vector<Point2D> scaled_positions = positions;
            center_and_scale(g, width, height, scaled_positions);
            iter_cb(scaled_positions, i);
        }
    }

    center_and_scale(g, width, height, positions);
    return positions;
}

vector<Point2D> kamada_kawai(
    const adj_list_t& g,
    unsigned int width,
    unsigned int height,
    double k,
    double energy_threshold) {
    vector<Point2D> positions(g.size());
    // Initial layout on a circle
    circle(g, positions);
    KamadaKawai kk(g, k, energy_threshold);
    kk(positions);
    center_and_scale(g, width, height, positions);

    return positions;
}

vector<double> size_radiuses(const adj_list_t& g, double min_radius, double k) {
    vector<double> radiuses;
    radiuses.reserve(g.size());
    for (vertex_id_t v_id = 0; v_id < g.size(); v_id++) {
        double delta = log2(k * (double) g[v_id].size() / g.size());
        double radius = min_radius + std::max(0.0, delta);
        radiuses.push_back(radius);
    }
    return radiuses;
}
}
