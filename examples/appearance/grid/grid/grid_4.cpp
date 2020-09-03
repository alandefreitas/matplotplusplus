#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);
    auto y1 = transform(x, [](double x) { return sin(x); });
    auto y2 = transform(x, [](double x) { return sin(3 * x); });

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(ax1, x, y1);

    auto ax2 = nexttile();
    plot(ax2, x, y2);
    grid(ax2, on);

    show();
    return 0;
}