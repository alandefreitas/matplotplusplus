#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto x1 = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    auto x2 = transform(iota(1, 10), [](double x) { return pow(x, 3); });

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    plot(x1);
    xlabel(ax1, "Population");

    auto ax2 = nexttile();
    plot(x2);

    show();
    return 0;
}