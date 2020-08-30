#pragma once
#include "nodesoup.hpp"
#include <utility>
#include <vector>

namespace nodesoup {
class FruchtermanReingold {
public:
    FruchtermanReingold(const adj_list_t& g, double k = 15.0);
    void operator()(std::vector<Point2D>& positions);

private:
    const adj_list_t& g_;
    const double k_;
    const double k_squared_;
    double temp_;
    std::vector<std::pair<vertex_id_t, vertex_id_t>> edges_;
    std::vector<Vector2D> mvmts_;
};
}
