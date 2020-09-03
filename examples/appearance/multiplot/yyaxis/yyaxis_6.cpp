#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    vector_1d x = linspace(1, 10);

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    plot(ax1, x, transform(x, [](double x) { return sin(x); }));
    hold(on);
    plot(ax1, x, transform(x, [](double x) { return exp(x); }))->use_y2(true);

    auto ax2 = nexttile();
    plot(ax2, iota(1, 10));

    show();
    return 0;
}