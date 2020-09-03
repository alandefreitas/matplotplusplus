#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    std::vector<double> x = linspace(0, 3);
    std::vector<double> y1 = transform(x, [](auto x) { return sin(5 * x); });
    std::vector<double> y2 = transform(x, [](auto x) { return sin(15 * x); });

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(ax1, x, y1);
    title(ax1, "Top Plot");
    ylabel(ax1, "sin(5x)");

    auto ax2 = nexttile();
    plot(ax2, x, y2);
    title(ax2, "Bottom Plot");
    ylabel(ax2, "sin(15x)");

    show();
    return 0;
}