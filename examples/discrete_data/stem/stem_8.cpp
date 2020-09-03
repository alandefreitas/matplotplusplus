#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    std::vector<double> x = iota(0, 25);
    std::vector<double> y1 = transform(x, [](auto x) { return exp(0.1 * x); });
    std::vector<double> y2 =
        transform(x, [](auto x) { return -exp(0.05 * x); });

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    stem(ax1, x, y1);

    auto ax2 = nexttile();
    stem(ax2, x, y2);

    show();
    return 0;
}