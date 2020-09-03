#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x = linspace(0, 10);

    auto y1 = transform(x, [](double x) { return sin(x); });
    auto y2 = transform(x, [](double x) { return cos(x); });

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    plot(ax1, x, y1);

    auto ax2 = nexttile();
    plot(ax2, x, y2);

    hold(ax1, on);
    auto y3 = transform(x, [](double x) { return sin(2 * x); });
    plot(ax1, x, y3);
    hold(ax1, off);

    show();
    return 0;
}