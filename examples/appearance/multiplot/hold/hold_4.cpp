#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(1, 2);
    auto ax1 = nexttile();
    auto ax2 = nexttile();

    plot(ax1, {0, 1, 0, 1});
    auto s1 = scatter(ax2, rand(10, 0, 1), rand(10, 0, 1));
    s1->marker_face(true);

    hold({ax1, ax2}, on);
    plot(ax1, {.5, .2, .5, .2});
    auto s2 = scatter(ax2, rand(10, 0, 1), rand(10, 0, 1));
    s2->marker_face(true);

    show();
    return 0;
}