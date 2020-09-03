#include <cmath>
#include <matplot/matplot.h>

int main() {
    using namespace matplot;
    tiledlayout(1, 2);

    auto ax1 = nexttile();
    auto t = iota(0, pi / 20, 10 * pi);
    auto xt1 = transform(t, [](auto t) { return sin(t); });
    auto yt1 = transform(t, [](auto t) { return cos(t); });
    plot3(ax1, xt1, yt1, t);
    title(ax1, "Helix with 5 Turns");

    auto ax2 = nexttile();
    t = iota(0, pi / 40, 10 * pi);
    auto xt2 = transform(t, [](auto t) { return sin(2 * t); });
    auto yt2 = transform(t, [](auto t) { return cos(2 * t); });
    plot3(ax2, xt2, yt2, t);
    ax2->box(false);
    title(ax2, "Helix with 10 Turns");

    show();
    return 0;
}