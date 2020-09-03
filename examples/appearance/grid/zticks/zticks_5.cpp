#include <matplot/matplot.h>

int main() {
    using namespace matplot;

    tiledlayout(2, 1);
    auto ax1 = nexttile();
    stem3(ax1, rand(5, 5, 0, 4));
    zticks(ax1, {0, 1.5, 3.25});

    auto ax2 = nexttile();
    stem3(ax2, rand(5, 5, 0, 4));

    show();
    return 0;
}