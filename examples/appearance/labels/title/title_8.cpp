#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    auto x1 = transform(iota(1, 10), [](double x) { return pow(x, 2); });
    auto x2 = transform(iota(1, 10), [](double x) { return pow(x, 3); });

    tiledlayout(2, 1);

    auto ax1 = nexttile();
    plot(ax1, x1);

    auto ax2 = nexttile();
    plot(ax2, x2);

    title(ax1, "Top Plot");
    title(ax2, "Bottom Plot");

    show();
    return 0;
}