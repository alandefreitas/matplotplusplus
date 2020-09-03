#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    std::vector<double> x = logspace(-1, 2);
    std::vector<double> y1 = transform(x, [](auto x) { return pow(10, x); });
    loglog(ax1, x, y1);

    auto ax2 = nexttile();
    std::vector<double> y2 =
        transform(x, [](auto x) { return 1. / pow(10, x); });
    loglog(ax2, x, y2);

    show();
    return 0;
}