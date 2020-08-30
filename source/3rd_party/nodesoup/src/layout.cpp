#include "layout.hpp"
#include "algebra.hpp"
#include <cmath>
#include <limits>

namespace nodesoup {

using std::vector;

void circle(const adj_list_t& g, vector<Point2D>& positions) {
    double angle = 2.0 * M_PI / g.size();
    for (vertex_id_t v_id = 0; v_id < g.size(); v_id++) {
        positions[v_id].x = cos(v_id * angle);
        positions[v_id].y = sin(v_id * angle);
    }
}

void center_and_scale(const adj_list_t& g, unsigned int width, unsigned int height, vector<Point2D>& positions) {
    // find current dimensions
    double x_min = std::numeric_limits<double>::max();
    double x_max = std::numeric_limits<double>::lowest();
    double y_min = std::numeric_limits<double>::max();
    double y_max = std::numeric_limits<double>::lowest();

    for (vertex_id_t v_id = 0; v_id < g.size(); v_id++) {
        if (positions[v_id].x < x_min) {
            x_min = positions[v_id].x;
        }
        if (positions[v_id].x > x_max) {
            x_max = positions[v_id].x;
        }

        if (positions[v_id].y < y_min) {
            y_min = positions[v_id].y;
        }
        if (positions[v_id].y > y_max) {
            y_max = positions[v_id].y;
        }
    }

    double cur_width = x_max - x_min;
    double cur_height = y_max - y_min;

    // compute scale factor (0.9: keep some margin)
    double x_scale = width / cur_width;
    double y_scale = height / cur_height;
    double scale = 0.9 * (x_scale < y_scale ? x_scale : y_scale);

    // compute offset and apply it to every position
    Vector2D center = { x_max + x_min, y_max + y_min };
    Vector2D offset = center / 2.0 * scale;
    for (vertex_id_t v_id = 0; v_id < g.size(); v_id++) {
        positions[v_id] = (Point2D)((Vector2D) positions[v_id] * scale - offset);
    }
}
}
