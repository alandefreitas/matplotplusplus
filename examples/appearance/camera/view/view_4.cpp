#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    auto t = iota(0, pi / 20, 10 * pi);
    auto xt1 = transform(t, [](double t) { return sin(t); });
    auto yt1 = transform(t, [](double t) { return cos(t); });
    tiledlayout(1, 2);

    auto ax1 = nexttile();
    plot3(ax1, xt1, yt1, t);
    xlabel("x");
    ylabel("y");
    zlabel("z");

    auto ax2 = nexttile();
    plot3(ax2, xt1, yt1, t);
    xlabel("x");
    ylabel("y");
    zlabel("z");
    view(ax2, 90, 0);

    show();
    return 0;
}