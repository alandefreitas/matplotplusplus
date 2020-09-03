#include <cmath>
#include <matplot/matplot.h>
#include <random>

int main() {
    using namespace matplot;

    std::vector<std::vector<double>> y = {{1, 4}, {2, 5}, {3, 6}};
    tiledlayout(2, 1);

    auto ax1 = nexttile();
    bar(ax1, y);

    auto ax2 = nexttile();
    barstacked(ax2, y);

    show();
    return 0;
}