#include <cmath>
#include <matplot/matplot.h>

std::vector<std::pair<size_t, size_t>> get_edges();

int main() {
    using namespace matplot;
    std::vector<std::pair<size_t, size_t>> edges = {
        {0, 1},   {0, 2},   {0, 3},   {0, 4},   {1, 5},  {1, 6},  {1, 7},
        {1, 8},   {1, 9},   {1, 10},  {1, 11},  {1, 12}, {1, 13}, {1, 14},
        {14, 15}, {14, 16}, {14, 17}, {14, 18}, {14, 19}};
    digraph(edges);

    show();
    return 0;
}
