#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 2 * pi);
    std::vector<double> y1 = transform(x, [](double x) { return 5 * sin(x); });
    std::vector<double> y2 = transform(x, [](double x) { return sin(5 * x); });
    tiledlayout(2, 1);

    auto ax1 = nexttile();
    stairs(ax1, x, y1);

    auto ax2 = nexttile();
    stairs(ax2, x, y2);

    show();
    return 0;
}