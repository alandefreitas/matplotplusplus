#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {0, 2},  {0, 3}, {0, 4}, {0, 6},  {0, 9}, {1, 5},
        {1, 6}, {2, 5},  {2, 9}, {3, 7}, {3, 9},  {4, 6}, {4, 7},
        {5, 8}, {5, 10}, {6, 8}, {7, 8}, {7, 10}, {9, 10}};
    std::vector<double> weights = {1, 1, 1, 1, 3, 3, 2,  4,  1,  6,
                                   2, 8, 8, 9, 3, 2, 10, 12, 15, 16};
    auto g = graph(edges);

    g->x_data({0, 0.5, -0.5, -0.5, 0.5, 0, 1.5, 0, 2, -1.5, -2});
    g->y_data({0, 0.5, 0.5, -0.5, -0.5, 2, 0, -2, 0, 0, 0});
    g->z_data({5, 3, 3, 3, 3, 0, 1, 0, 0, 1, 0});

    g->edge_labels(weights);
    view(2);

    show();
    return 0;
}