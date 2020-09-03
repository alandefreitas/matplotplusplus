#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {1, 4}, {1, 5}, {2, 4},
        {2, 6}, {3, 5}, {3, 6}, {4, 7}, {5, 7}, {6, 7}};
    auto g = digraph(edges);
    g->edge_labels(
        {"x", "y", "z", "y", "z", "x", "z", "x", "y", "z", "y", "x"});
    g->node_labels(
        {"{0}", "{x}", "{y}", "{z}", "{x,y}", "{x,z}", "{y,z}", "{x,y,z}"});

    show();
    return 0;
}