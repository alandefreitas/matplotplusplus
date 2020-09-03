#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {0, 2}, {0, 9},  {1, 3},  {1, 11}, {2, 3},
        {2, 4}, {3, 5}, {4, 5},  {4, 6},  {5, 8},  {6, 7},
        {6, 9}, {7, 8}, {7, 10}, {8, 11}, {9, 10}, {10, 11}};
    auto g = digraph(edges);
    g->marker("s");
    g->node_color("red");
    g->marker_size(7);
    g->line_style("--");

    g->x_data({2, 4, 1.5, 3.5, 1, 3, 1, 2.1, 3, 2, 3.1, 4});
    g->y_data({3, 3, 3.5, 3.5, 4, 4, 2, 2, 2, 1, 1, 1});

    show();
    return 0;
}