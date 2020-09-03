#include <matplot/matplot.h>
#include <thread>

std::vector<std::pair<size_t, size_t>> get_edges();

int main() {
    using namespace matplot;
    using namespace std;

    vector<pair<size_t, size_t>> edges = {
        {0, 1}, {0, 2}, {1, 2}, {1, 4}, {2, 3}, {3, 5}, {4, 5},
        {4, 6}, {6, 8}, {8, 9}, {7, 9}, {8, 7}, {9, 10}};
    std::vector<double> weights = {4, 4, 2, 1, 1, 4, 2, 1, 3, 3, 2, 4, 1};
    graph(edges)->edge_labels(weights);

    show();
    return 0;
}