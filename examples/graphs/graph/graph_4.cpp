#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 5},
        {2, 5}, {3, 4}, {3, 6}, {4, 6}, {5, 6}};
    std::vector<double> weights = {50, 10, 20, 80, 90, 90, 30, 20, 100, 40, 60};
    auto g = graph(edges);
    g->edge_labels(weights);

    double m_weight = max(weights);
    std::vector<double> line_widths =
        transform(weights, [&](double w) { return 5. * w / m_weight; });
    g->line_widths(line_widths);

    show();
    return 0;
}